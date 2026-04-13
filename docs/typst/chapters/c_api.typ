= C API Reference

This chapter documents every public struct, function, and macro in the CRF runtime library.

== `cevent` — Event Base <cevent>

=== `cevent_tst`

Base event structure. All derived events must embed this as their first member.

```c
typedef struct cevent_tst {
    signal_t    sig;       // event type identifier
    gc_info_tst gc_info;   // pool id + reference count
} cevent_tst;
```

Size: 4 bytes (with default `signal_t = uint16_t`).

=== `gc_info_tst`

```c
typedef struct {
    uint16_t ref_cnt : 12;   // 0 – 4095
    uint16_t pool_id :  4;   // 0 – 15
} gc_info_tst;
```

- *ref_cnt* — Incremented on queue insertion, decremented after dispatch. Zero triggers deallocation.
- *pool_id* — Identifies the source pool. Pool 0 (`CEVENT_INVALID_POOL`) marks static events.

=== Signal Classes

#table(
  columns: (2fr, 1fr, 2fr),
  table.header([*Constant*], [*Value*], [*Purpose*]),
  [`CEVENT_INVALID_POOL`], [0], [Static event marker.],
  [`CRF_SIGNAL_CLASS_MOD_INTERNAL`], [1], [Module-internal signals.],
  [`CRF_SIGNAL_CLASS_START`], [2], [First application class.],
  [`CRF_SIGNAL_CLASS_SIZE`], [100], [Signals per class.],
)

Macro: `SIGNAL_FROM_CLASS(CLASS)` → `CLASS × 100`.

=== Functions

- `cevent_ref_cnt_inc(e)` — Increment reference count (no-op for static events).
- `cevent_ref_cnt_dec(e)` — Decrement reference count (no-op for static events).

== `cqueue` — Event Queue <cqueue>

=== `cqueue_tst`

Circular FIFO of event pointers. Capacity must be a power of two.

Key fields:
- `events` — Pre-allocated pointer array.
- `max` — Capacity.
- `head`, `tail` — Atomic read/write indices.
- `mask` — `max - 1` for efficient wrapping.
- `fault_cnt` — Overflow counter (omitted with `CHSM_CFG_NO_FAULT_CNT`).

=== Functions

#table(
  columns: (2fr, 1fr, 3fr),
  table.header([*Function*], [*Returns*], [*Description*]),
  [`cqueue_init(q, events, max)`], [`int32_t`], [Initialise queue. Returns 0.],
  [`cqueue_put(q, e)`], [`int32_t`], [Enqueue right. ISR-safe. Returns −1 on overflow.],
  [`cqueue_put_left(q, e)`], [`int32_t`], [Enqueue left (priority). ISR-safe.],
  [`cqueue_get(q)`], [`cevent*`], [Dequeue left. Main-thread only. NULL if empty.],
  [`cqueue_get_right(q)`], [`cevent*`], [Dequeue right. Used by `chsm_recall`.],
)

=== Dispatch Macros

`CQUEUE_PUT`, `CQUEUE_PUT_LEFT`, `CQUEUE_GET`, `CQUEUE_GET_RIGHT` — resolve to function-pointer calls normally, or direct calls with `CHSM_CFG_LITE`.

== `cpool` — Memory Pool <cpool>

=== `cpool_tst`

Lock-free fixed-size block allocator using atomic CAS.

Key fields:
- `pool` — Byte buffer.
- `esize` — Bytes per block.
- `ecnt` — Total blocks.
- `head` — Atomic offset to first free block.

=== Functions

#table(
  columns: (2fr, 1fr, 3fr),
  table.header([*Function*], [*Returns*], [*Description*]),
  [`cpool_init(p, buf, size, cnt)`], [void], [Thread free-list through buffer.],
  [`cpool_new(p)`], [`void*`], [CAS pop. ISR-safe. NULL if exhausted.],
  [`cpool_gc(p, e)`], [`bool`], [CAS push. ISR-safe. False if wrong pool.],
)

Dispatch macros: `CPOOL_NEW(p)`, `CPOOL_GC(p, e)`.

== `chsm` — State Machine <chsm>

=== `chsm_tst`

State machine instance. Embeds two queues (event + defer), a state handler pointer, a send callback, and a linked-list next pointer.

=== Result Enum

#table(
  columns: (1fr, 2fr),
  table.header([*Value*], [*Meaning*]),
  [`C_RES_HANDLED`], [Event processed.],
  [`C_RES_TRANS`], [State transition triggered.],
  [`C_RES_PARENT`], [Delegate to parent state.],
  [`C_RES_IGNORED`], [Event intentionally skipped.],
  [`C_RES_GUARDS`], [Guard conditions — none true.],
)

=== Functions

#table(
  columns: (2fr, 3fr),
  table.header([*Function*], [*Description*]),
  [`chsm_ctor(self, init, events, qlen, dlen)`], [Construct HSM with initial state and queue buffers.],
  [`chsm_init(self)`], [Send `C_SIG_INIT` and execute initial transition.],
  [`chsm_dispatch(self, e)`], [Dispatch event to current state; propagate on `C_RES_PARENT`.],
  [`chsm_defer(self, e)`], [Move event to defer queue.],
  [`chsm_recall(self, e)`], [Move all deferred events to front of event queue.],
)

=== Inline Helpers

- `chsm_transition(self, target)` — Record transition, return `C_RES_TRANS`.
- `chsm_handled(self)` — Return `C_RES_HANDLED`.
- `chsm_ignored(self)` — Return `C_RES_IGNORED`.

== `crf` — Reactive Framework <crf>

=== `crf_tst`

Top-level framework. Owns the HSM array and pool array. Provides the event loop.

=== Functions

#table(
  columns: (2fr, 3fr),
  table.header([*Function*], [*Description*]),
  [`crf_init(self, hsms, pools, cnt)`], [Initialise framework with HSM and pool arrays.],
  [`crf_new_event(self, size, sig)`], [Allocate event from matching pool. NULL on failure.],
  [`crf_publish(self, e)`], [Post event to all registered HSMs.],
  [`crf_post(self, e, q)`], [Post event to specific queue.],
  [`crf_step(self)`], [Process one event per HSM. Returns true if work done.],
  [`crf_gc(self, e)`], [Decrement ref_cnt; free to pool if zero.],
)

=== Convenience Macros

#table(
  columns: (1fr, 2fr),
  table.header([*Macro*], [*Purpose*]),
  [`CRF_NEW(SIGNAL)`], [Allocate event by signal name.],
  [`CRF_POST(e, q)`], [Post event to queue.],
  [`CRF_EMIT(e)`], [Send via active HSM's callback.],
  [`CRF_STEP()`], [Run one event cycle.],
  [`CRF_GC(e)`], [Garbage-collect event.],
  [`CRF_SIG_VAR(SIG, var, e)`], [Declare and cast event pointer.],
  [`CRF_POST_TO_SELF(e)`], [Post to own queue with priority.],
)

== `chsm_cfg` — Build Configuration <chsm_cfg>

Three compile-time flags control the runtime footprint:

#table(
  columns: (2fr, 3fr),
  table.header([*Flag*], [*Effect*]),
  [`CHSM_CFG_LITE`], [Replace function-pointer dispatch with direct calls. Enables inlining.],
  [`CHSM_CFG_NO_FAULT_CNT`], [Remove `fault_cnt` from queues (−4 B each).],
  [`CHSM_CFG_NO_DEBUG`], [Strip debug instrumentation (−20 B per module).],
)

Define in `chsm_cfg.h` (included before all CRF headers) or pass as compiler flags.
