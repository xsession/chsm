class State(object):
    '''Represents a state in a state machine.

    `enter` and `exit` handlers are called whenever a state is entered or
    exited respectively. These action names are reserved only for this purpose.

    It is encouraged to extend this class to encapsulate a state behavior,
    similarly to the State Pattern.

    Once it's extended, the preferred way of adding an event handlers is
    through the :func:`register_handlers` hook. Usually,
    there's no need to create the :func:`__init__` in a subclass.

    :param name: Human readable state name
    :type name: str

    **Example Usage:**

    .. code-block:: python

        # Extending State to encapsulate state-related behavior. Similar to the
        # State Pattern.
        class Running(State):
            def on_enter(self, state, event):
                print('Running state entered')

            def on_jump(self, state, event):
                print('Jumping')

            def on_dollar(self, state, event):
                print('Dollar found!')

            def register_handlers(self):
                self.handlers = {
                    'enter': self.on_enter,
                    'jump': self.on_jump,
                    '$': self.on_dollar
                }

    .. code-block:: python

        # Different way of attaching handlers. A handler may be any function as
        # long as it takes `state` and `event` args.
        def another_handler(state, event):
            print('Another handler')

        running = State('running')
        running.handlers = {
            'another_event': another_handler
        }

    '''
    def __init__(self, name):
        self.parent = None
        self.name = name
        # self.id = 1
        self.handlers = {}
        self.initial = False
        self.register_handlers()

    def __repr__(self):
        return '<State {0} ({1})>'.format(self.name, hex(id(self)))

    def register_handlers(self):
        '''Hook method to register event handlers.

        It is used to easily extend |State| class. The hook is called from
        within the base :func:`.State.__init__`. Usually, the
        :func:`__init__` doesn't have to be created in a subclass.

        Event handlers are kept in a `dict`, with events' names as keys,
        therefore registered events may be of any hashable type.

        Handlers take two arguments:

        - **state**: The current state that is handling an event. The same
              handler function may be attached to many states, therefore it
              is helpful to get the handling state's instance.
        - **event**: An event that triggered the handler call. If it is an
              `enter` or `exit` event, then the source event (the one that
              triggered the transition) is passed in `event`'s cargo
              property as `cargo.source_event`.

        **Example Usage:**

        .. code-block:: python

            class On(State):
                def handle_my_event(self, state, event):
                    print('Handling an event')

                def register_handlers(self):
                    self.handlers = {
                        'my_event': self.handle_my_event,
                        '&': self.handle_my_event,
                        frozenset([1, 2]): self.handle_my_event
                    }

        '''
        pass

    def is_substate(self, state):
        '''Check whether the `state` is a substate of `self`.

        Also `self` is considered a substate of `self`.

        :param state: State to verify
        :type state: |State|
        :returns: `True` if `state` is a substate of `self`, `False` otherwise
        :rtype: bool

        '''
        if state is self:
            return True
        parent = self.parent
        while parent:
            if parent is state:
                return True
            parent = parent.parent
        return False

    def _on(self, event):
        if event.name in self.handlers:
            event.propagate = False
            self.handlers[event.name](self, event)
        # Never propagate exit/enter events, even if propagate is set to True
        if (self.parent and event.propagate and
                event.name not in ('exit', 'enter')):
            self.parent._on(event)

    def _nop(self, state, event):
        del state  # Unused (silence pylint)
        del event  # Unused (silence pylint)
        return True
