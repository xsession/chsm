= Configuration

== CMake Build System

The C runtime uses CMake with helper macros from `cmake_utils/cmake_utils.cmake`.

=== Key Macros

- *`add_module_lib`* — Create a static library from module sources with correct includes and link deps.
- *`add_module_test`* — Create a Unity test executable linked to the module under test.

=== Build Commands

```bash
cd languages/c
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

=== Cross-Compilation

```bash
cmake -B build -S . \
    -DCMAKE_TOOLCHAIN_FILE=cmake_utils/compilers/arm-none-eabi.cmake \
    -DCMAKE_BUILD_TYPE=MinSizeRel
```

== Runtime Tuning

=== Profiles

#table(
  columns: (1fr, 2fr, 2fr),
  table.header([*Profile*], [*Flags*], [*Use Case*]),
  [Development], [_(none)_], [Full diagnostics, debug logging, fault counters.],
  [Release], [`CHSM_CFG_LITE`], [Reduced RAM; overflow diagnostics retained.],
  [Minimal], [`LITE + NO_FAULT_CNT + NO_DEBUG`], [Smallest footprint (≤ 8 KB RAM MCUs).],
)

=== RAM Savings (Cortex-M, 32-bit)

#table(
  columns: (2fr, 1fr, 1fr, 1fr),
  table.header([*Config*], [*Queue ×2*], [*Pool*], [*Framework*]),
  [Default], [56 B], [20 B], [48 B],
  [`CHSM_CFG_LITE`], [24 B], [10 B], [12 B],
  [`+ NO_FAULT_CNT`], [16 B], [10 B], [12 B],
  [`+ NO_DEBUG`], [16 B], [10 B], [12 B],
)

Total savings with all three flags: ~86 bytes per HSM system.

=== Queue Sizing

- *Event queue*: 8–16 entries (power of 2). Covers ISR burst between `crf_step` calls.
- *Defer queue*: 4 entries. Rarely more than 2–3 deferred simultaneously.

=== Pool Sizing

+ Identify the largest derived event type.
+ Count peak in-flight events (queued + deferred + dispatching).
+ Add 2–4 blocks margin.

== Project Settings

=== `settings.json`

Code generation defaults stored at `gui/c_gen/templates/settings.json`:

#table(
  columns: (1fr, 1fr, 2fr),
  table.header([*Key*], [*Type*], [*Description*]),
  [`output_dir`], [string], [Relative path for generated files.],
  [`language`], [string], [Default target: `c`, `python`, `js`, `java`, `vhdl`.],
  [`indent`], [int], [Indentation width (spaces).],
  [`line_ending`], [string], [`lf` or `crlf`.],
  [`header_guard_style`], [string], [`pragma_once` or `ifndef`.],
)

=== Themes

CSS files in `gui/web/themes/`. Create a custom theme by copying `default.css` and editing CSS custom properties (`--bg-primary`, `--accent`, `--state-fill`, etc.).
