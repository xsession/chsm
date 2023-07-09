class State {
    enter() {}
    exit() {}
    handleEvent(event, payload) {}
  }
  
  class StateMachine {
    constructor(initialState) {
      this.currentState = initialState;
      this.currentState.enter();
    }
  
    handleEvent(event, payload) {
      this.currentState.handleEvent(event, payload);
    }
  }
  
  class StateA extends State {
    handleEvent(event, payload) {
      if (event === "EVENT_A") {
        console.log("Handling event A in state A");
      } else if (event === "EVENT_B") {
        console.log("Transitioning from state A to state B");
        this.exit();
        this.machine.currentState = new StateB();
        this.machine.currentState.enter();
      }
    }
  }
  
  class StateB extends State {
    handleEvent(event, payload) {
      if (event === "EVENT_A") {
        console.log("Transitioning from state B to state A");
        this.exit();
        this.machine.currentState = new StateA();
        this.machine.currentState.enter();
      } else if (event === "EVENT_B") {
        console.log("Handling event B in state B");
        console.log("Transaction payload:", payload);
      }
    }
  }
  
  // Create a state machine with an initial state of A
  const machine = new StateMachine(new StateA());
  
  // Send some events to the state machine with payloads
  machine.handleEvent("EVENT_A", { amount: 100 });
  machine.handleEvent("EVENT_B", { customerId: "abc123" });
  machine.handleEvent("EVENT_A", { amount: 50 });
  