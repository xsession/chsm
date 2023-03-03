class TransitionsContainer(object):
    def __init__(self, machine):
        self._machine = machine
        self._transitions = defaultdict(list)

    def add(self, key, transition):
        self._transitions[key].append(transition)

    def get(self, event):
        key = (self._machine.state, event.name, event.input)
        return self._get_transition_matching_condition(key, event)

    def _get_transition_matching_condition(self, key, event):
        from_state = self._machine.leaf_state
        for transition in self._transitions[key]:
            if transition['condition'](from_state, event) is True:
                return transition
        key = (self._machine.state, any_event, event.input)
        for transition in self._transitions[key]:
            if transition['condition'](from_state, event) is True:
                return transition
        return None
    
    