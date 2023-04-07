class State {
  void enter() {}
  void exit() {}
  void handleEvent(String event, Map<String, Object> payload) {}
}

class StateMachine {
  private State currentState;

  StateMachine(State initialState) {
    currentState = initialState;
    currentState.enter();
  }

  void handleEvent(String event, Map<String, Object> payload) {
    currentState.handleEvent(event, payload);
  }
}

class StateA extends State {
  void handleEvent(String event, Map<String, Object> payload) {
    if (event.equals("EVENT_A")) {
      System.out.println("Handling event A in state A");
    } else if (event.equals("EVENT_B")) {
      System.out.println("Transitioning from state A to state B");
      exit();
      machine.currentState = new StateB();
      machine.currentState.enter();
    }
  }
}

class StateB extends State {
  void handleEvent(String event, Map<String, Object> payload) {
    if (event.equals("EVENT_A")) {
      System.out.println("Transitioning from state B to state A");
      exit();
      machine.currentState = new StateA();
      machine.currentState.enter();
    } else if (event.equals("EVENT_B")) {
      System.out.println("Handling event B in state B");
      System.out.println("Transaction payload: " + payload);
    }
  }
}

// Create a state machine with an initial state of A
StateMachine machine = new StateMachine(new StateA());

// Send some events to the state machine with payloads
Map<String, Object> payload1 = new HashMap<>();
payload1.put("amount", 100);
machine.handleEvent("EVENT_A", payload1);

Map<String, Object> payload2 = new HashMap<>();
payload2.put("customerId", "abc123");
machine.handleEvent("EVENT_B", payload2);

Map<String, Object> payload3 = new HashMap<>();
payload3.put("amount", 50);
machine.handleEvent("EVENT_A", payload3);
