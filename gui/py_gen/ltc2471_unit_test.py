import unittest
import queue

class Signal:
    def __init__(self, name, value):
        self.name = name
        self.value = value
        self.listeners = []

    def add_listener(self, listener):
        self.listeners.append(listener)

    def remove_listener(self, listener):
        self.listeners.remove(listener)

    def emit(self):
        for listener in self.listeners:
            listener.handle_signal(self)


class StateMachine:
    def __init__(self, state_data, signals):
        self.states = state_data
        self.current_state = ["__top__"]
        self.signals = signals
        self.signal_queue = queue.Queue()
        self.deferred_signals = []

    def handle_signal(self, signal):
        if signal in self.deferred_signals:
            self.deferred_signals.remove(signal)
            self.signal_queue.put(signal)
        else:
            self._handle_signal(signal)

    def _handle_signal(self, signal):
        transitions = self.states[self.current_state[-1]]["transitions"]
        for transition in transitions:
            if (
                transition["signal"] == signal.name
                and transition["value"] == signal.value
            ):
                self.current_state[-1] = transition["next_state"]
                return

        for parent_state in self.current_state[:-1][::-1]:
            transitions = self.states[parent_state]["transitions"]
            for transition in transitions:
                if (
                    transition["signal"] == signal.name
                    and transition["value"] == signal.value
                ):
                    self.current_state.pop()
                    self.current_state.append(transition["next_state"])
                    return

    def add_signal_listener(self, signal_name):
        self.signals[signal_name].add_listener(self)

    def handle_signals(self):
        while not self.signal_queue.empty():
            signal = self.signal_queue.get()
            self._handle_signal(signal)

    def defer_signal(self, signal):
        self.deferred_signals.append(signal)

    def queue_signal(self, signal):
        self.signal_queue.put(signal)

    def run(self):
        while True:
            self.handle_signals()


# Example usage of the StateMachine class and unit tests

class LTC2471StateMachineTest(unittest.TestCase):
    def setUp(self):
        self.state_data = {
            "__top__": {
                "transitions": [
                    {"signal": "SIG_SYS_TICK_1ms", "value": "", "next_state": "s_ltc2471"},
                ],
            },
            "s_ltc2471": {
                "transitions": [
                    {"signal": "SIG_START_READ", "value": "", "next_state": "s_reading"},
                    {"signal": "SIG_I2C_RESULT_SUCCESS", "value": "", "next_state": "s_idle"},
                ],
            },
            "s_idle": {
                "transitions": [
                    {"signal": "SIG_RESTART_TIMER", "value": "", "next_state": "s_idle"},
                    {"signal": "SIG_START_READ", "value": "", "next_state": "s_reading"},
                ],
            },
            "s_reading": {
                "transitions": [
                    {"signal": "SIG_I2C_RESULT_SUCCESS", "value": "", "next_state": "s_idle"},
                    {"signal": "SIG_I2C_RESULT_TIMEOUT", "value": "", "next_state": "s_reading"},
                ],
            },
        }

        self.signals = {
            "SIG_SYS_TICK_1ms": Signal("SIG_SYS_TICK_1ms", ""),
            "SIG_START_READ": Signal("SIG_START_READ", ""),
            "SIG_I2C_RESULT_SUCCESS": Signal("SIG_I2C_RESULT_SUCCESS", ""),
            "SIG_RESTART_TIMER": Signal("SIG_RESTART_TIMER", ""),
            "SIG_I2C_RESULT_TIMEOUT": Signal("SIG_I2C_RESULT_TIMEOUT", ""),
        }

        self.state_machine = StateMachine(self.state_data, self.signals)

    def tearDown(self):
        self.state_machine = None

    def test_state_transitions(self):
        # Test initial state
        self.assertEqual(self.state_machine.current_state, "__top__")

    #     # Emit SIG_SYS_TICK_1ms signal
    #     self.signals["SIG_SYS_TICK_1ms"].emit()
    #     self.assertEqual(self.state_machine.current_state, "s_ltc2471")

    #     # Emit SIG_START_READ signal
    #     self.signals["SIG_START_READ"].emit()
    #     self.assertEqual(self.state_machine.current_state, "s_reading")

    #     # Emit SIG_I2C_RESULT_SUCCESS signal
    #     self.signals["SIG_I2C_RESULT_SUCCESS"].emit()
    #     self.assertEqual(self.state_machine.current_state, "s_idle")

    #     # Emit SIG_RESTART_TIMER signal
    #     self.signals["SIG_RESTART_TIMER"].emit()
    #     self.assertEqual(self.state_machine.current_state, "s_idle")

    #     # Emit SIG_START_READ signal again
    #     self.signals["SIG_START_READ"].emit()
    #     self.assertEqual(self.state_machine.current_state, "s_reading")

    #     # Emit SIG_I2C_RESULT_TIMEOUT signal
    #     self.signals["SIG_I2C_RESULT_TIMEOUT"].emit()
    #     self.assertEqual(self.state_machine.current_state, "s_reading")

    #     # Emit SIG_I2C_RESULT_SUCCESS signal
    #     self.signals["SIG_I2C_RESULT_SUCCESS"].emit()
    #     self.assertEqual(self.state_machine.current_state, "s_idle")

    # def test_signal_deferral(self):
    #     # Emit SIG_START_READ signal and defer it
    #     self.signals["SIG_START_READ"].emit()
    #     self.state_machine.defer_signal(self.signals["SIG_START_READ"])

    #     # Emit SIG_SYS_TICK_1ms signal
    #     self.signals["SIG_SYS_TICK_1ms"].emit()
    #     self.assertEqual(self.state_machine.current_state, ["s_ltc2471"])

    #     # The deferred SIG_START_READ signal should now be handled
    #     self.state_machine.handle_signals()
    #     self.assertEqual(self.state_machine.current_state, ["s_reading"])

    # def test_signal_queuing(self):
    #     # Emit SIG_START_READ signal and queue it
    #     self.signals["SIG_START_READ"].emit()
    #     self.state_machine.queue_signal(self.signals["SIG_START_READ"])

    #     # Emit SIG_SYS_TICK_1ms signal
    #     self.signals["SIG_SYS_TICK_1ms"].emit()
    #     self.assertEqual(self.state_machine.current_state, ["s_ltc2471"])

    #     # The queued SIG_START_READ signal should now be handled
    #     self.state_machine.handle_signals()
    #     self.assertEqual(self.state_machine.current_state, ["s_reading"])


if __name__ == "__main__":
    unittest.main()
