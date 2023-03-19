/*globals module, require, console, exports */

if (typeof module === "object" && typeof require === "function") {
    var buster = require("buster");
    var HSM = require("../StateMachine.js");
    // set up logging to console
    HSM.Logger.debug = console.log;
}

var assert = buster.referee.assert;

buster.testCase("testSubStates", {
    setUp: function() {
        var _ = this;
        // Sub State Machine
        var quietState = new HSM.State("Quiet");
        var loudState = new HSM.State("Loud");
    
        quietState.handler.volume_up = { target: loudState };
        loudState.handler.volume_down = { target: quietState };
        _.volumeStateMachine = new HSM.StateMachine([quietState, loudState]);

        // Main State Machine
        _.offState = new HSM.State("OffState");
        _.onState = new HSM.Sub("OnState", _.volumeStateMachine);
    
        _.offState.handler.switched_on = { target: _.onState };
        _.onState.handler.switched_off = { target: _.offState };

        _.sm = new HSM.StateMachine([_.offState, _.onState]).init();
    },
    "testPath": function() {
        var _ = this;
        assert.equals([_.sm], _.sm.path);
        assert.equals([_.sm, _.volumeStateMachine], _.volumeStateMachine.path);
    },
    "testInheritance": function() {
        var _ = this;
        assert(_.offState instanceof HSM.State);
        assert(_.onState instanceof HSM.State);
        assert(_.onState instanceof HSM.Sub);
    },
    "testSubMachine": function() {
        var _ = this;
        assert.equals("OffState", _.sm.state.toString());

        _.sm.handleEvent("switched_on");
        assert.equals("OnState/(Quiet)", _.sm.toString());

        _.sm.handleEvent("volume_up");
        assert.equals("OnState/(Loud)", _.sm.toString());
    
        _.sm.handleEvent("switched_off");
        assert.equals("OffState", _.sm.toString());
    
        _.sm.handleEvent("switched_on");
        assert.equals("OnState", _.sm.state.id);
        assert.equals("Quiet", _.sm.state.subMachine.state.id);
        assert.equals("Quiet", _.sm.state.subState.id);
        assert.equals("OnState/(Quiet)", _.sm.toString());
    }
});
