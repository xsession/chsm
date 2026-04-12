"""
Jinja-compatible StateMachine class.
Produces a data dict suitable for Jinja template rendering.
Used by the job-based code generation system alongside the existing AST-based SM.
"""
import json
import re
import logging
from copy import deepcopy
from .parser import Parser, ParserException, NOSIG, NOFUNC, NOPARAM, NOGUARD, NULLFUNC

VERSION_STRING = 'Generated with CHSM v0.0.2'

SYS_SIGNALS = ('entry', 'exit', 'init')


class JinjaStateMachine:
    def __init__(self, data):

        self.user_funcs = set()
        self.user_guards = set()
        self.user_signals = set()
        self.user_inc_funcs = set()
        self.user_inc_guards = set()

        data['states']['__top__']['parent'] = ""
        self.states = self.get_states(data)                 # Extract states from the data
        self.add_transitions_to_states(self.states, data)   # Extract transition info from the data and add it to the states
        self.add_parent_signals(self.states)
        self.process_transitions(self.states)               # Calculate transition exit and entry paths and the exact end state

        self.delete_non_leaf_states(self.states)

        self.notes = data['notes']

        self.resolve_parent_titles(self.states)

        self.remove_unnecessary_transitions(self.states)

        self.remove_initial_states(self.states)

        self.clean_up_signals()

        self.clean_up_notes()

        self.data = {
            'states': self.states,
            'user_funcs': self.user_funcs,
            'user_guards': self.user_guards,
            'user_signals': self.user_signals,
            'user_inc_funcs': self.user_inc_funcs,
            'user_inc_guards': self.user_inc_guards,
            'notes': self.notes,
        }

    def clean_up_notes(self):
        notes = {}
        for k, v in self.notes.items():
            if v:
                if k.endswith('()'):
                    notes[k[:-2]] = v
                else:
                    notes[k] = v.strip()
        self.notes = notes

    def clean_up_signals(self):
        self.user_signals = {i for i in self.user_signals if i not in ('entry', 'exit', 'init')}

    def get_transition_funcs(self, start, end, lca):
        path = self.get_transition_path(start, end, lca)
        return self.path_to_funcs(path), path[-1][0]

    def process_guard_transition(self, s_id, g, states):
        if g['target']:
            lca = g.get('lca', '')
            if not lca:
                logging.warning(f'process_guard_transition: guard in state {s_id} has no lca, target={g["target"]}')
                return
            tfuncs, target = self.get_transition_funcs(s_id, g['target'], lca)
            g['funcs'].extend(tfuncs)
            g['target_title'] = states[target]['title']
            g['target_type'] = states[target]['type']
            if 'params' in states[target]:
                g['target_params'] = states[target]['params']

    def process_transitions(self, states):
        for s_id, s in states.items():
            if s['children']:
                continue
            for sig_id, sig in s['signals'].items():
                for g in sig['guards'].values():
                    self.process_guard_transition(s_id, g, states)

            for g in s['guards'].values():
                self.process_guard_transition(s_id, g, states)

        # The only place where transition functions are needed for an init signal is the __top__
        g = states['__top__']['sys_signals']['init']
        lca = g.get('lca', '__top__') or '__top__'
        tfuncs, target = self.get_transition_funcs('__top__', g['target'], lca)
        g['funcs'].extend(tfuncs)
        g['target_title'] = states[target]['title']

    def delete_non_leaf_states(self, states):
        for s_id in [*states.keys()]:
            s = states[s_id]
            if s['children'] and s['parent']:
                del states[s_id]
            elif s['parent']:
                s['parent'] = '__top__'

    def add_parent_signals(self, states):
        for s_id, s in states.items():
            if not s['children'] and s['type'] == 'normal':
                self.add_parent_signals_to_state(s_id, states)

    def add_parent_signals_to_state(self, state_id, states):
        """
            Take a state, go through its parent states and add signal handlers
            that are not already in the original state.
            If the signal already has some handlers in the original state, then
            go through the guards one by one and copy those that are not in the
            original state.
        """
        state = states[state_id]
        signals = state['signals']

        parent_id = state['parent']
        while parent_id:
            parent = states[parent_id]
            p_signals = parent['signals']
            for sig_id, sig in p_signals.items():

                if not sig_id in signals:
                    signals[sig_id] = deepcopy(sig)

                else:
                    guards = signals[sig_id]['guards']
                    for guard_id, p_guard in sig['guards'].items():
                        if not guard_id in guards:
                            guards[guard_id] = deepcopy(p_guard)

            # Go through the parent state guards and copy those as well
            guards = state['guards']
            for guard_id, p_guard in parent['guards'].items():
                if not guard_id in guards:
                    guards[guard_id] = deepcopy(p_guard)

            parent_id = states[parent_id]['parent']

    def add_signal_to_state(self, state, signal):
        signal_name = signal['name']

        if signal_name == '':
            for g_fn, g in signal['guards'].items():
                if g_fn in state['guards']:
                    orig_guard = state['guards'][g_fn]

                    # Add functions to the original guard
                    orig_guard['funcs'].extend(g['funcs'])

                    # Set the target
                    if g['target']:
                        if orig_guard['target'] and orig_guard['target'] != g['target']:
                            logging.error(f"Guard target mismatch for {g_fn} in {state['title']}: {orig_guard['target']} != {g['target']}")
                        else:
                            orig_guard['target'] = g['target']
                else:
                    state['guards'][g_fn] = g

        elif signal_name in SYS_SIGNALS:
            if not state['sys_signals'][signal_name]:
                state['sys_signals'][signal_name]['funcs'] = signal['guards']['']['funcs']
            else:
                state['sys_signals'][signal_name]['funcs'].extend(signal['guards']['']['funcs'])
            state['sys_signals'][signal_name]['target'] = signal['guards']['']['target']
            state['sys_signals'][signal_name]['target_title'] = signal['guards']['']['target_title']
            state['sys_signals'][signal_name]['lca'] = signal['guards']['']['lca']
        elif signal_name not in state['signals']:
            state['signals'][signal_name] = signal
        else:
            orig_signal = state['signals'][signal_name]

            for g_fn, g in signal['guards'].items():
                if g_fn in orig_signal['guards']:
                    orig_guard = orig_signal['guards'][g_fn]

                    # Add functions to the original guard
                    orig_guard['funcs'].extend(g['funcs'])

                    # Set the target
                    if g['target']:
                        if orig_guard['target'] and orig_guard['target'] != g['target']:
                            logging.error(f"Guard target mismatch for {g_fn} in {state['title']}: {orig_guard['target']} != {g['target']}")
                        else:
                            orig_guard['target'] = g['target']

                else:
                    orig_signal['guards'][g_fn] = g

    def process_state_title(self, title):
        """Parse the title string. A state title can be a function identifier
        or a function call.
        Return: (function_name, params, is_call), where:
            function: name of the function or id
            params: parameter string,
            is_call: True, if the title is actually a function call."""
        title = title.lstrip()

        m = re.match(r'[a-zA-z_]+\w*', title)
        if m:
            fname = m[0]
            rem = title[m.span()[1]:]
        else:
            raise ParserException(f'Expected function identifier in "{title}"')

        rem = rem.lstrip()

        if len(rem) == 0 or rem[0] != '(':
            return fname, "", False

        rem = rem[1:]

        params, c_brace, rem = rem.partition(')')

        params = params.strip()
        if params == '':
            params = ""
        else:
            for p in params.split(','):
                ps = p.strip()
                if not ps.isidentifier() and not ps.isalnum():
                    raise ParserException(f'Expected identifier as function param but found: "{p}"')

        if c_brace == '':
            raise ParserException(f'Expected ")" in "{title}"')

        return fname, params, True

    def get_states(self, data):
        """Extract state info from drawing data into a dict"""
        states = {}

        for s_id, s in data['states'].items():
            if s['type'] == 'initial':
                title = s_id
                params = ''
                is_call = False
            else:
                title, params, is_call = self.process_state_title(s['title'])

            state = {
                'signals': {},
                'sys_signals': {
                    'entry': {},
                    'exit': {},
                    'init': {}
                },
                'guards': {},
                'parent': s['parent'],
                'children': s['children'],
                'title': title,
                'type': s['type'],
            }

            if is_call:
                state['type'] = 'call'
                state['params'] = params
            else:
                if s_id.startswith('state_'):
                    state['num'] = int(re.findall(r'\d+', s_id)[0])

            p = Parser()
            for text_entry in s['text']:
                text_entry = text_entry.strip()
                if not text_entry:
                    continue
                try:
                    sigs = p.parse(text_entry)
                except ParserException as e:
                    logging.info(f'Exception {str(e)} in state "{s["title"]}". Text:\n{text_entry}')
                    sigs = []

                for sig in sigs:
                    self.add_signal_to_state(state, sig)

            self.user_inc_funcs.update(p.funcs_w_args)
            self.user_inc_guards.update(p.guards_w_args)
            self.user_funcs.update(p.funcs_wo_args)
            self.user_signals.update(p.user_signals)
            self.user_guards.update(p.guards_wo_args)

            states[s_id] = state

        return states

    def resolve_parent_titles(self, states):
        for s in states.values():
            if s['parent']:
                s['parent_title'] = states[s['parent']]['title']

    def remove_initial_states(self, states):
        keys = tuple(states.keys())
        for k in keys:
            if states[k]['type'] == 'initial':
                del states[k]

    def remove_unnecessary_transitions(self, states):
        """Remove transition targets pointing to the current state."""
        for state in states.values():
            for signal in state['signals'].values():
                for guard in signal['guards'].values():
                    if guard.get("target_title", "") == state['title']:
                        guard['target'] = ""
                        guard["target_title"] = ""

    def resolve_transition(self, tr, data):
        """Return the label, the start and the end states of a transition"""
        start_conn = data['connectors'][tr['start']]
        end_conn = data['connectors'][tr['end']]

        start_state = start_conn['parent']
        end_state = end_conn['parent']
        event = tr['label']

        return start_state, end_state, event

    def add_transitions_to_states(self, states, data):
        """Go through the transitions in the state machine, and add them
        to the start states"""

        for tr in data['transitions'].values():
            start, target, label = self.resolve_transition(tr, data)

            if target:
                if self.is_in(start, target):
                    lca = target
                else:
                    lca = self.get_LCA(start, target)
            else:
                lca = ""

            p = Parser()

            if states[start]['type'] == 'initial':
                start = states[start]['parent']
                states[start]['initial'] = target

                signals = p.parse(label, target=target, target_title=states[target]['title'], initial=True)
            else:
                signals = p.parse(label, target, target_title=states[target]['title'], initial=False, lca=lca)

            try:
                if signals:
                    self.add_signal_to_state(states[start], signals[0])
            except:
                print(signals)
                raise

            self.user_inc_funcs.update(p.funcs_w_args)
            self.user_inc_guards.update(p.guards_w_args)
            self.user_funcs.update(p.funcs_wo_args)
            self.user_signals.update(p.user_signals)
            self.user_guards.update(p.guards_wo_args)

    def get_entry_path(self, start_state_id, target_state_id):

        if start_state_id == target_state_id:
            return tuple()

        state_id = target_state_id
        path = []
        while True:
            path.append((state_id, 'entry'))

            if state_id not in self.states:
                return path

            super_id = self.states[state_id]['parent']

            if super_id == start_state_id:
                return reversed(path)
            else:
                state_id = super_id

    def is_in(self, state_id, composit_state_id):
        if composit_state_id == '__top__':
            return True
        elif state_id == '__top__':
            return False

        while True:
            super_id = self.states[state_id]['parent']

            if super_id == '__top__':
                return False
            elif super_id == composit_state_id:
                return True
            else:
                state_id = super_id

    def get_LCA(self, start_state_id, target_state_id):
        """Find Lowest Common Ancestor"""

        super_id = start_state_id
        while True:
            if self.is_in(target_state_id, super_id):
                return super_id
            else:
                super_id = self.states[super_id]['parent']

    def get_exit_path(self, start_state_id, target_state_id):
        path = []
        s = start_state_id

        if s == "__top__":
            return path

        while s != target_state_id:
            path.append((s, 'exit'))
            s = self.states[s]['parent']

        return path

    def get_transition_path(self, start_state_id, target_state_id, lca):
        if target_state_id == '__top__':
            return self.get_exit_path(start_state_id, '__top__')
        else:
            exit_path = self.get_exit_path(start_state_id, lca)
            init_path = self.get_init_path(lca, target_state_id)
            return exit_path + init_path

    def get_init_path(self, start_state_id, target_state_id):
        '''This func enters the target state and then enter the init state of the
        target, if defined'''
        path = []
        while True:
            entry_path = self.get_entry_path(start_state_id, target_state_id)
            path.extend(entry_path)

            path.append((target_state_id, 'init'))
            init_state_id = self.states[target_state_id].get('initial', "")
            if init_state_id == "":
                return path
            else:
                start_state_id = target_state_id
                target_state_id = init_state_id

    def path_to_funcs(self, path):
        funcs = []
        for step in path:
            state_id, event_id = step
            try:
                funcs.extend(self.states[state_id]['sys_signals'][event_id]['funcs'])
            except KeyError:
                pass

        funcs = [f for f in funcs if f != NULLFUNC]

        return tuple(funcs)
