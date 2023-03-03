class StateMachine(State):
    '''State machine controls actions and transitions.

    To provide the State Pattern-like behavior, the formal state machine rules
    may be slightly broken, and instead of creating an `internal transition
    <https://en.wikipedia.org/wiki/UML_state_machine #Internal_transitions>`_
    for every action that doesn't require a state change, event handlers may be
    added to states. These are handled first when an event occurs. After that
    the actual transition is called, calling `enter`/`exit` actions and other
    transition actions. Nevertheless, internal transitions are also supported.

    So the order of calls on an event is as follows:

        1. State's event handler
        2. `condition` callback
        3. `before` callback
        4. `exit` handlers
        5. `action` callback
        6. `enter` handlers
        7. `after` callback

    If there's no handler in states or transition for an event, it is silently
    ignored.

    If using nested state machines, all events should be sent to the root state
    machine.

    **Attributes:**

        .. attribute:: state

            Current, local state (instance of |State|) in a state machine.

        .. attribute:: stack

            Stack that can be used if the `Pushdown Automaton (PDA)
            <https://en.wikipedia.org/wiki/Pushdown_automaton>`_ functionality
            is needed.

        .. attribute:: state_stack

            Stack of previous local states in a state machine. With every
            transition, a previous state (instance of |State|) is pushed to the
            `state_stack`. Only :attr:`.StateMachine.STACK_SIZE` (32
            by default) are stored and old values are removed from the stack.

        .. attribute:: leaf_state_stack

            Stack of previous leaf states in a state machine. With every
            transition, a previous leaf state (instance of |State|) is pushed
            to the `leaf_state_stack`. Only
            :attr:`.StateMachine.STACK_SIZE` (32 by default) are
            stored and old values are removed from the stack.

        **leaf_state**
            See the :attr:`~.StateMachine.leaf_state` property.

        **root_machine**
            See the :attr:`~.StateMachine.root_machine` property.

    :param name: Human readable state machine name
    :type name: str

    .. note ::

        |StateMachine| extends |State| and therefore it is possible to always
        use a |StateMachine| instance instead of the |State|. This wouldn't
        be a good practice though, as the |State| class is designed to be as
        small as possible memory-wise and thus it's more memory efficient. It
        is valid to replace a |State| with a |StateMachine| later on if there's
        a need to extend a state with internal states.

    .. note::

        For the sake of speed thread safety isn't guaranteed.

    **Example Usage:**

    .. code-block:: python

        state_machine = StateMachine('root_machine')
        state_on = State('On')
        state_off = State('Off')
        state_machine.add_state('Off', initial=True)
        state_machine.add_state('On')
        state_machine.add_transition(state_on, state_off, events=['off'])
        state_machine.add_transition(state_off, state_on, events=['on'])
        state_machine.initialize()
        state_machine.dispatch(Event('on'))

    '''
    STACK_SIZE = 32

    def __init__(self, name):
        super(StateMachine, self).__init__(name)
        self.states = set()
        self.state = None
        self._transitions = TransitionsContainer(self)
        self.state_stack = Stack(maxlen=StateMachine.STACK_SIZE)
        self.leaf_state_stack = Stack(maxlen=StateMachine.STACK_SIZE)
        self.stack = Stack()
        self._leaf_state = None

    def add_state(self, state, initial=False):
        '''Add a state to a state machine.

        If states are added, one (and only one) of them has to be declared as
        `initial`.

        :param state: State to be added. It may be an another |StateMachine|
        :type state: |State|
        :param initial: Declare a state as initial
        :type initial: bool

        '''
        Validator(self).validate_add_state(state, initial)
        state.initial = initial
        state.parent = self
        self.states.add(state)

    def add_states(self, *states):
        '''Add `states` to the |StateMachine|.

        To set the initial state use
        :func:`set_initial_state`.

        :param states: A list of states to be added
        :type states: |State|

        '''
        for state in states:
            self.add_state(state)

    def set_initial_state(self, state):
        '''Set an initial state in a state machine.

        :param state: Set this state as initial in a state machine
        :type state: |State|

        '''
        Validator(self).validate_set_initial(state)
        state.initial = True

    @property
    def initial_state(self):
        '''Get the initial state in a state machine.

        :returns: Initial state in a state machine
        :rtype: |State|

        '''
        for state in self.states:
            if state.initial:
                return state
        return None

    @property
    def root_machine(self):
        '''Get the root state machine in a states hierarchy.

        :returns: Root state in the states hierarchy
        :rtype: |StateMachine|

        '''
        machine = self
        while machine.parent:
            machine = machine.parent
        return machine

    def add_transition(
            self, from_state, to_state, events, input=None, action=None,
            condition=None, before=None, after=None):
        '''Add a transition to a state machine.

        All callbacks take two arguments - `state` and `event`. See parameters
        description for details.

        It is possible to create conditional if/elif/else-like logic for
        transitions. To do so, add many same transition rules with different
        condition callbacks. First met condition will trigger a transition, if
        no condition is met, no transition is performed.

        :param from_state: Source state
        :type from_state: |State|
        :param to_state: Target state. If `None`, then it's an `internal
            transition <https://en.wikipedia.org/wiki/UML_state_machine
            #Internal_transitions>`_
        :type to_state: |State|, `None`
        :param events: List of events that trigger the transition
        :type events: |Iterable| of |Hashable|
        :param input: List of inputs that trigger the transition. A transition
            event may be associated with a specific input. i.e.: An event may
            be ``parse`` and an input associated with it may be ``$``. May be
            `None` (default), then every matched event name triggers a
            transition.
        :type input: `None`, |Iterable| of |Hashable|
        :param action: Action callback that is called during the transition
            after all states have been left but before the new one is entered.

            `action` callback takes two arguments:

                - state: Leaf state before transition
                - event: Event that triggered the transition

        :type action: |Callable|
        :param condition: Condition callback - if returns `True` transition may
            be initiated.

            `condition` callback takes two arguments:

                - state: Leaf state before transition
                - event: Event that triggered the transition

        :type condition: |Callable|
        :param before: Action callback that is called right before the
            transition.

            `before` callback takes two arguments:

                - state: Leaf state before transition
                - event: Event that triggered the transition

        :type before: |Callable|
        :param after: Action callback that is called just after the transition

            `after` callback takes two arguments:

                - state: Leaf state after transition
                - event: Event that triggered the transition

        :type after: |Callable|

        '''
        # Rather than adding some if statements later on, let's just declare a
        # neutral items that will do nothing if called. It simplifies the logic
        # a lot.
        if input is None:
            input = tuple([None])
        if action is None:
            action = self._nop
        if before is None:
            before = self._nop
        if after is None:
            after = self._nop
        if condition is None:
            condition = self._nop

        Validator(self).validate_add_transition(
            from_state, to_state, events, input)

        for input_value in input:
            for event in events:
                key = (from_state, event, input_value)
                transition = {
                    'from_state': from_state,
                    'to_state': to_state,
                    'action': action,
                    'condition': condition,
                    'before': before,
                    'after': after,
                }
                self._transitions.add(key, transition)

    def _get_transition(self, event):
        machine = self.leaf_state.parent
        while machine:
            transition = machine._transitions.get(event)
            if transition:
                return transition
            machine = machine.parent
        return None

    @property
    def leaf_state(self):
        '''Get the current leaf state.

        The :attr:`~.StateMachine.state` property gives the current,
        local state in a state machine. The `leaf_state` goes to the bottom in
        a hierarchy of states. In most cases, this is the property that should
        be used to get the current state in a state machine, even in a flat
        FSM, to keep the consistency in the code and to avoid confusion.

        :returns: Leaf state in a hierarchical state machine
        :rtype: |State|

        '''
        return self.root_machine._leaf_state
        #  return self._get_leaf_state(self)

    def _get_leaf_state(self, state):
        while hasattr(state, 'state') and state.state is not None:
            state = state.state
        return state

    def initialize(self):
        '''Initialize states in the state machine.

        After a state machine has been created and all states are added to it,
        :func:`initialize` has to be called.

        If using nested state machines (HSM),
        :func:`initialize` has to be called on a root
        state machine in the hierarchy.

        '''
        machines = deque()
        machines.append(self)
        while machines:
            machine = machines.popleft()
            Validator(self).validate_initial_state(machine)
            machine.state = machine.initial_state
            for child_state in machine.states:
                if isinstance(child_state, StateMachine):
                    machines.append(child_state)

        self._leaf_state = self._get_leaf_state(self)

    def dispatch(self, event):
        '''Dispatch an event to a state machine.

        If using nested state machines (HSM), it has to be called on a root
        state machine in the hierarchy.

        :param event: Event to be dispatched
        :type event: :class:`.Event`

        '''
        event.state_machine = self
        leaf_state_before = self.leaf_state
        leaf_state_before._on(event)
        transition = self._get_transition(event)
        if transition is None:
            return
        to_state = transition['to_state']
        from_state = transition['from_state']

        transition['before'](leaf_state_before, event)
        top_state = self._exit_states(event, from_state, to_state)
        transition['action'](leaf_state_before, event)
        self._enter_states(event, top_state, to_state)
        transition['after'](self.leaf_state, event)

    def _exit_states(self, event, from_state, to_state):
        if to_state is None:
            return None
        state = self.leaf_state
        self.leaf_state_stack.push(state)
        while (state.parent and
                not (from_state.is_substate(state) and
                     to_state.is_substate(state)) or
                (state == from_state == to_state)):
            logger.debug('exiting %s', state.name)
            exit_event = Event('exit', propagate=False, source_event=event)
            exit_event.state_machine = self
            self.root_machine._leaf_state = state
            state._on(exit_event)
            state.parent.state_stack.push(state)
            state.parent.state = state.parent.initial_state
            state = state.parent
        return state

    def _enter_states(self, event, top_state, to_state):
        if to_state is None:
            return
        path = []
        state = self._get_leaf_state(to_state)

        while state.parent and state != top_state:
            path.append(state)
            state = state.parent
        for state in reversed(path):
            logger.debug('entering %s', state.name)
            enter_event = Event('enter', propagate=False, source_event=event)
            enter_event.state_machine = self
            self.root_machine._leaf_state = state
            state._on(enter_event)
            state.parent.state = state

    def set_previous_leaf_state(self, event=None):
        '''Transition to a previous leaf state. This makes a dynamic transition
        to a historical state. The current `leaf_state` is saved on the stack
        of historical leaf states when calling this method.

        :param event: (Optional) event that is passed to states involved in the
            transition
        :type event: :class:`.Event`

        '''
        if event is not None:
            event.state_machine = self
        from_state = self.leaf_state
        try:
            to_state = self.leaf_state_stack.peek()
        except IndexError:
            return
        top_state = self._exit_states(event, from_state, to_state)
        self._enter_states(event, top_state, to_state)

    def revert_to_previous_leaf_state(self, event=None):
        '''Similar to :func:`set_previous_leaf_state`
        but the current leaf_state is not saved on the stack of states. It
        allows to perform transitions further in the history of states.

        '''
        self.set_previous_leaf_state(event)
        try:
            self.leaf_state_stack.pop()
            self.leaf_state_stack.pop()
        except IndexError:
            return
