/*globals module, require, console, exports */

if (typeof module === "object" && typeof require === "function") {
    var buster = require("buster");
    var HSM = require("../StateMachine.js");
    // set up logging to console
    HSM.Logger.debug = console.log;
    // HSM.Logger.trace = console.log;
}

var assert = buster.referee.assert;
// HSM.Logger.debug = console.log;
// HSM.Logger.trace = console.log;

buster.testCase("testAdvanced", {
    setUp: function() {
        var _ = this;
        // execution log
        _.log = [];

        // a mixin for HSM.State which logs
        // entry and exit calls
        var asLogging = function() {
            this.on_entry = function(source) {
                _.log.push(this._id+":entered(source:"+(source ? source.id : "null") +")");
            };
            this.on_exit = function(target) {
                _.log.push(this._id+":exited(target:"+(target ? target.id : "null")+")");
            };
            return this;
        };
        asLogging.call(HSM.State.prototype);

        // State Machine 'a'
        var a1 = new HSM.State("a1");
        var a2 = new HSM.State("a2");
        var a3 = new HSM.State("a3");

        a1.handler.T1 = [
            {
                target: a2,
                guard: function (_,_,theData) {
                    return theData;
                }
            },
            {
                target: a3,
                guard: function (_,_,theData) {
                    return !theData;
                }
            }
        ];
        a2.handler.T2 = {
            target: a3,
            action: function (theData) {
                this.emit("T3");
            }
        };
        a3.handler.T3 = {
            target: a1
        };


        var a = new HSM.Sub("a", new HSM.StateMachine([a1, a2, a3]));

        // State Machine 'b2'
        var b21 = new HSM.State("b21");
        var b22 = new HSM.State("b22");

        // State Machine 'b2'
        var b1 = new HSM.State("b1");
        var b2 = new HSM.Sub("b2", new HSM.StateMachine([b21, b22]));

        var b = new HSM.Sub("b", new HSM.StateMachine([b1, b2]));

        // State Machine c1
        var c11 = new HSM.State("c11");
        var c12 = new HSM.State("c12");

        var c21 = new HSM.State("c21");
        var c22 = new HSM.State("c22");

        var c = new HSM.Parallel("c", new HSM.StateMachine([c11, c12]),
                                      new HSM.StateMachine([c21, c22]));

        a.handler.T1 = { target: b };
        a3.handler.T4 = {
            target: b2
        };
        a3.handler.T5 = {
            target: b22,
            kind: 'local' // just to test this - should be irrelevant
        };
        a.handler.T6 = {
            target: a2
        };
        a2.handler.T7 = {
            target: a
        };
        b.handler.T8 = {
            target: b22,
            kind: 'local'
        };
        b22.handler.T9 = {
            target: b,
            kind: 'local'
        };
        b1.handler.T10 = {
            target: b1,
            kind: 'internal',
            action: function (theData) {
                _.log.push("T10:internalAction()");
            }
        };
        b1.handler.T11 = {
            target: null,
            kind: 'internal',
            action: function (theData) {
                _.log.push("T11:internalAction()");
            }
        };
        b1.handler.T12 = {
            target: b2, // invalid definition of an internal transition
            kind: 'internal',
            action: function (theData) {
                _.log.push("T12:invalid()");
            }
        };
        
        // Top State Machine
        _.sm = new HSM.StateMachine([a, b, c]).init();

    },
    "testEnter": function() {
        var _ = this;
        assert.equals(_.log, ["a:entered(source:null)", "a1:entered(source:null)"]);
    },
    "testFirstGuard": function() {
        var _ = this;
        _.log = [];
        _.sm.handleEvent("T1", true);
        assert.equals(_.log, ["a1:exited(target:a2)","a2:entered(source:a1)"]);
    },
    "testSecondGuard": function() {
        var _ = this;
        _.log = [];
        _.sm.handleEvent("T1", false);
        assert.equals(_.log, ["a1:exited(target:a3)","a3:entered(source:a1)"]);
    },
    "testRunToCompletion": function() {
        var _ = this;
        _.sm.handleEvent("T1", true);
        _.log = []; // start test at a2
        _.sm.handleEvent("T2", true);
        assert.equals(_.log, ["a2:exited(target:a3)", "a3:entered(source:a2)", "a3:exited(target:a1)", "a1:entered(source:a3)"]);
    },
    "testBubbleUp": function() {
        var _ = this;
        _.sm.handleEvent("T1", false);
        assert.equals(_.sm.state.subState.id, "a3", 'substate "a1" processed T1, so superstate "a" didn\'t get it');
        _.sm.handleEvent("T1");
        assert.equals(_.sm.state.subState.id, "b1", 'substate "a3" didn\'t handle T1, so it bubbled up to "a"');
    },
    "testLowestCommonAncestorT4": function() {
        var _ = this;
        _.sm.handleEvent("T1", false);
        _.log = []; // start in a3
        _.sm.handleEvent("T4");
        assert.equals(_.sm.state.id, "b");
        assert.equals(_.log, ["a3:exited(target:b2)", "a:exited(target:b2)", "b:entered(source:a3)", "b2:entered(source:a3)", "b21:entered(source:a3)"]);
    },
    "testLowestCommonAncestorT5": function() {
        var _ = this;
        _.sm.handleEvent("T1", false);
        _.log = []; // start in a3
        _.sm.handleEvent("T5");
        assert.equals(_.sm.state.id, "b");
        assert.equals(_.log, ["a3:exited(target:b22)", "a:exited(target:b22)", "b:entered(source:a3)", "b2:entered(source:a3)", "b22:entered(source:a3)"]);
    },
    "testExternalTransition": function() {
        var _ = this;
        _.log = [];
        _.sm.handleEvent("T6");
        assert.equals(_.log, ["a1:exited(target:a2)", "a:exited(target:a2)", "a:entered(source:a)", "a2:entered(source:a)"]);
        _.log = [];
        _.sm.handleEvent("T7");
        assert.equals(_.log, ["a2:exited(target:a)", "a:exited(target:a)", "a:entered(source:a2)", "a1:entered(source:a2)"]);
    },
    "testLocalTransition": function() {
        var _ = this;
        _.sm.handleEvent("T1", true);
        _.sm.handleEvent("T1");
        _.log = []; // start in b1
        _.sm.handleEvent("T8");
        assert.equals(_.log, ["b1:exited(target:b22)", "b2:entered(source:b)", "b22:entered(source:b)"]);
        _.log = [];
        _.sm.handleEvent("T9");
        assert.equals(_.log, ["b22:exited(target:b)", "b2:exited(target:b)", "b1:entered(source:b22)"]);
    },
    "testInternalTransitionWithExplicitTarget": function() {
        var _ = this;
        _.sm.handleEvent();
        var _ = this;
        _.sm.handleEvent("T1", true);
        _.sm.handleEvent("T1");
        _.log = []; // start in b1
        _.sm.handleEvent("T10");
        assert.equals(_.log, ["T10:internalAction()"]);
    },
    "testInternalTransitionWithOmittedTarget": function() {
        var _ = this;
        _.sm.handleEvent();
        var _ = this;
        _.sm.handleEvent("T1", true);
        _.sm.handleEvent("T1");
        _.log = []; // start in b1
        _.sm.handleEvent("T11");
        assert.equals(_.log, ["T11:internalAction()"]);
    },
    "testInvalidInternalTransition": function() {
        var _ = this;
        _.sm.handleEvent();
        var _ = this;
        _.sm.handleEvent("T1", true);
        _.sm.handleEvent("T1");
        _.log = []; // start in b1
        _.sm.handleEvent("T12");
        assert.equals(_.log, []);
    }
});
