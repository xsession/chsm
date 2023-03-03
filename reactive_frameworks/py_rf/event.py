class Event(object):
    r'''Triggers actions and transition in |StateMachine|.

    Events are also used to control the flow of data propagated to states
    within the states hierarchy.

    Event objects have the following attributes set after an event has been
    dispatched:

    **Attributes:**

        .. attribute:: state_machine

            A |StateMachine| instance that is handling the event (the one whose
            :func:`pysm.pysm.StateMachine.dispatch` method is called)

        .. attribute:: propagate

            An event is propagated from a current leaf state up in the states
            hierarchy until it encounters a handler that can handle the event.
            To propagate it further, it has to be set to `True` in a handler.

    :param name: Name of an event. It may be anything as long as it's hashable.
    :type name: |Hashable|
    :param input: Optional input. Anything hashable.
    :type input: |Hashable|
    :param \*\*cargo: Keyword arguments for an event, used to transport data to
        handlers.  It's added to an event as a `cargo` property of type `dict`.
        For `enter` and `exit` events, the original event that triggered a
        transition is passed in cargo as `source_event` entry.

    .. note`:

        `enter` and `exit` events are never propagated, even if the `propagate`
        flag is set to `True` in a handler.

    **Example Usage:**

    .. code-block:: python

        state_machine.dispatch(Event('start'))
        state_machine.dispatch(Event('start', key='value'))
        state_machine.dispatch(Event('parse', input='#', entity=my_object))
        state_machine.dispatch(Event('%'))
        state_machine.dispatch(Event(frozenset([1, 2])))

    '''
    def __init__(self, name, input=None, **cargo):
        self.name = name
        self.input = input
        self.propagate = True
        self.cargo = cargo
        # This must be always the root machine
        self.state_machine = None

    def __repr__(self):
        return '<Event {0}, input={1}, cargo={2} ({3})>'.format(
            self.name, self.input, self.cargo, hex(id(self)))

