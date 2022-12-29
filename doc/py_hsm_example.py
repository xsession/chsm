class State:
  def enter(self):
    pass

  def exit(self):
    pass

  def handle_event(self, event, payload):
    pass

class StateMachine:
  def __init__(self, initial_state):
    self.current_state = initial_state
    self.current_state.enter()

  def handle_event(self, event, payload):
    self.current_state.handle_event(event, payload)

class StateA(State):
  def handle_event(self, event, payload):
    if event == "EVENT_A":
      print("Handling event A in state A")
    elif event == "EVENT_B":
      print("Transitioning from state A to state B")
      self.exit()
      self.machine.current_state = StateB()
      self.machine.current_state.enter()

class StateB(State):
  def handle_event(self, event, payload):
    if event == "EVENT_A":
      print("Transitioning from state B to state A")
      self.exit()
      self.machine.current_state = StateA()
      self.machine.current_state.enter()
    elif event == "EVENT_B":
      print("Handling event B in state B")
      print("Transaction payload:", payload)

# Create a state machine with an initial state of A
machine = StateMachine(StateA())

# Send some events to the state machine with payloads
machine.handle_event("EVENT_A", {"amount": 100})
machine.handle_event("EVENT_B", {"customer_id": "abc123"})
machine.handle_
