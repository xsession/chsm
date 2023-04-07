'''Python State Machine

The goal of this library is to give you a close to the State Pattern
simplicity with much more flexibility. And, if needed, the full state machine
functionality, including `FSM
<https://en.wikipedia.org/wiki/Finite-state_machine>`_, `HSM
<https://en.wikipedia.org/wiki/UML_state_machine
#Hierarchically_nested_states>`_, `PDA
<https://en.wikipedia.org/wiki/Pushdown_automaton>`_ and other tasty things.

Goals:
    - Provide a State Pattern-like behavior with more flexibility
    - Be explicit and don't add any code to objects
    - Handle directly any kind of event (not only strings) - parsing strings is
      cool again!
    - Keep it simple, even for someone who's not very familiar with the FSM
      terminology

----

.. |StateMachine| replace:: :class:`~.StateMachine`
.. |State| replace:: :class:`~.State`
.. |Hashable| replace:: :class:`~collections.Hashable`
.. |Iterable| replace:: :class:`~collections.Iterable`
.. |Callable| replace:: :class:`~collections.Callable`

'''
import logging
import sys
from collections import defaultdict, deque


# Required to make it Micropython compatible
if str(type(defaultdict)).find('module') > 0:
    # pylint: disable=no-member
    defaultdict = defaultdict.defaultdict


# Required to make it Micropython compatible
def patch_deque(deque_module):
    class deque_maxlen(object):
        def __init__(self, iterable=None, maxlen=0):
            # pylint: disable=no-member
            if iterable is None:
                iterable = []
            if maxlen in [None, 0]:
                maxlen = float('Inf')
            self.q = deque_module.deque(iterable)
            self.maxlen = maxlen

        def pop(self):
            return self.q.pop()

        def append(self, item):
            if self.maxlen > 0 and len(self.q) >= self.maxlen:
                self.q.popleft()
            self.q.append(item)

        def __getattr__(self, name):
            return getattr(self.q, name)

        def __bool__(self):
            return len(self.q) > 0

        def __len__(self):
            return len(self.q)

        def __iter__(self):
            return iter(self.q)

        def __getitem__(self, key):
            return self.q[key]

    return deque_maxlen


# Required to make it Micropython compatible
try:
    test_deque = deque(maxlen=1)
except TypeError:
    # TypeError: unexpected keyword argument 'maxlen'
    if hasattr(deque, 'deque'):
        deque = patch_deque(deque)
    else:
        class MockDequeModule(object):
            deque = deque
        deque = patch_deque(MockDequeModule)
else:
    del test_deque


logger = logging.getLogger(__name__)
handler = logging.StreamHandler(sys.stdout)
logger.addHandler(handler)
logger.setLevel(logging.INFO)


class AnyEvent(object):
    '''
    hash(object()) doesn't work in MicroPython therefore the need for this
    class.
    '''
    pass

any_event = AnyEvent()


def is_iterable(obj):
    try:
        iter(obj)
    except TypeError:
        return False
    return True


class StateMachineException(Exception):
    '''All |StateMachine| exceptions are of this type. '''
    pass

class Stack(object):
    def __init__(self, maxlen=None):
        self.deque = deque(maxlen=maxlen)

    def pop(self):
        return self.deque.pop()

    def push(self, value):
        self.deque.append(value)

    def peek(self):
        return self.deque[-1]

    def __repr__(self):
        return str(list(self.deque))

