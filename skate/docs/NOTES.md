# Research notes

## Title info
- Title: Skate (Europe), English, Spanish, Italian
- Title ID: 45410813, media ID 1F6E4912, executable version 0.0.0.4 (built 2007-08-22)
  on the disc; this build is recompiled from Title Update 2 (0.0.2.4)
- Achievements: 44, 1000 Gamerscore
- Guest DLL modules: none
- Disc: 148 files, 5,344,017,481 bytes; `default.xex` is 6,291,456 bytes

## Codegen

Each codegen pass takes 5 to 7 minutes, against about 20 seconds for Skate 2;
most of it is analysis, not writing.

### 2026-09-15: split leaf functions
`stabilize_codegen.py` stalled on four identical sites (0x83024DDC, 0x8302D57C,
0x8302FF1C, 0x8303541C). Seeding each target split a branch from 30 bytes later,
and disabling the seed left `b <target>` from further on unresolved, so the
script looped between the two.

Forcing codegen through the errors (`rexglue --force codegen`, with the build's
codegen command edited the same way so the build didn't regenerate) produced a
build good enough to dump the image. Disassembling it showed each site is a
pair of functions:

- 0x83024968-0x83024D40, ending in a `b`
- 0x83024D40-0x83024E98, a leaf whose last block sits after its `blr` (at
  0x83024E04) and branches back into the body

Codegen ended the second function at the `blr`, so the block after it looked
like a separate function branching into the middle of another. Explicit bounds
for all eight functions in `config/functions.toml` fixed it. A first attempt
with one range per pair failed at runtime with `Call to invalid or unregistered
function at 0x83024D40`: the second functions are called through pointers in
data around 0x830610D4.

The framework's gap scan then seeded addresses inside the explicit ranges,
which codegen rejects as overlapping boundaries; `find_missing_functions.py`
now skips them.

### 2026-09-15: discovery
With the image dumped: 232 functions referenced from data, 1,969 from code gaps
and 17 whose address is only built in code. Stabilizing reported 2,854 split
branches (407 seeds disabled), then 69 and 17; `prune_bad_seeds.py --image`
disabled one more. No under-counted jump tables. Result: 1,817 seeds, 413
disabled, clean codegen and build.

### 2026-09-15: branches codegen reported but never fixed
Two more classes of problem survived a "clean" stabilize:

- **Seven gap seeds right after a `bctr`** (0x8232F3A4, 0x82483610, 0x824B3EA4,
  0x82596828, 0x82596890, 0x825B06DC, 0x82698234) cut off the rest of the
  function before them. Codegen logs these as `Unresolved b target X from Y`,
  a wording `prune_bad_seeds.py` didn't match, so they were never disabled. It
  matches now, and `stabilize_codegen.py` also reads leftover `REX_FATAL` stubs
  from the generated sources, since codegen only logs warnings for files it
  rewrites.
- **A switch whose cases each return** (bctr 0x82BC20D4): codegen ended the
  function at the first case's `blr`, so the later table targets were outside
  it. Explicit bounds `0x82BC20B8`-`0x82BC2218` fix it. 0x82426668, a small
  function right after a `blr`, had been folded into the one before it and
  needed a plain seed.

Result: 1,816 seeds, 421 disabled, no unresolved stubs in the generated code.

## Runtime

### 2026-09-15: first launches
- `Call to invalid or unregistered function at 0x83024D40` a few seconds in: the
  first explicit bounds covered each function pair as one range (see above).
- `Jump target 0x82BC2164 unresolved at bctr 0x82BC20D4`: the switch above.
- With both fixed: boots, compiles shaders and plays the intro cinematic (the
  San Vanelona fly-over, EA VP6 video with correct colour) with no errors for
  three minutes. The main menu wasn't reached; the intro is still running at
  2.5 minutes and probably needs a button press to skip.
- Loose-file misses such as `d:\maincasthdr.big` and
  `d:\data\state\MotionGraphIncludes\...` look like optional loose-file
  overrides checked before the `.big` archives; loading carries on after them.

### 2026-09-18: recompiled from Title Update 2
The build now targets Title Update 2. Two packages were to hand; the archive and
their own headers agree on which is which:

| Package | Media ID | Patches | This disc |
| --- | --- | --- | --- |
| `TU_12K220J_000000G000000.00000000000G4` | 1F6E4912 | 0.0.0.4 to 0.0.2.4 | yes |
| `TU_12K220J_000000C000000.00000000000G3` | 21D6D331 | 0.0.0.3 to 0.0.2.3 | no, another release |

The update holds one code patch, `default.xexp` (772,096 bytes), and no data
files at all, so all of it is machine code. `config/tu2/` is its own config, and
`config/` is left as the disc build's.

The disc build's three hand-written fixes all reappear, at addresses the update
moves by different amounts. None of it could be derived from a single offset:

| Fix | Disc | Update | Moved |
| --- | --- | --- | --- |
| Four leaf-function pairs | 0x83024968, 0x8302D108, 0x8302FAA8, 0x83034FA8 | 0x830265B8, 0x8302ED58, 0x830316F8, 0x83036BF8 | 0x1C50 |
| Switch whose cases each return | 0x82BC20B8 | 0x82BC3C68 | 0x1BB0 |
| Function folded in as unreachable | 0x82426668 | 0x82426668 | not at all |
| CRT setjmp / longjmp | 0x82E50F20 / 0x82E48B60 | 0x82E52B40 / 0x82E4A780 | 0x1C20 |

A first pass guessed the pairs from the unresolved branches, taking each
`b <target>` for the second function's start. It is not: the target is inside
the body, where the block after the leaf's `blr` branches back to. Reading the
region in the dumped image settled it - a prologue (`mflr r12` then `bl
0x82E4FDF8`) at 0x830265B8, the tail call that returns at 0x83026690, the
trailing blocks branching back, then the next prologue at 0x83026AE8. Both
halves keep the disc build's lengths, 0x3D8 and 0x158, and every bound ends on
the next function's prologue. The scans independently seeded all four second
halves at the same addresses.

Two smaller traps cost a round each: bounds written for an address a scan had
already seeded make invalid TOML, and a seed that then falls inside a bounded
function is rejected as an overlapping boundary (here the jump table itself, at
0x82BC3C88).

`0x82426668` needed bounds rather than the bare seed the disc build gives it.
Its own loop branches back to its first block and its address is never taken
from data, so the `--image` prune removed it as a local branch target.

The pruner also deleted the bounds themselves, twice, because it treated an
entry with `end =` as an ordinary seed. Fixed in the framework, with a test;
`skateRecomped/framework` is a submodule, so the first fix had no effect until
it was moved onto the commit carrying it.

Result: 1,820 seeds, 419 disabled, ten functions with explicit bounds, no
under-counted jump tables, no stubs left.

### 2026-09-18: the menu and free skate, at last
Three minutes with a controller on the update build: the title screen ("Press
START to continue"), the main menu, then skating around Downtown with the HUD,
the minimap, pedestrians, traffic and line scoring. The guide was opened with
View + Menu part way through and closed again. 59.9 fps average, 43.5 fps 1%
low, worst frame 83 ms, no stalls over 100 ms, 382 draws per frame median. No
errors and no stubs called.

This is the first time this game has been past its intro here; the disc build
had only ever shown the fly-over. Whether the disc build also gets there has not
been retested.

Logged but harmless: `NtCreateFile` misses for loose-file overrides checked
before the `.big` archives (`d:\data\content\livingworld\texture\*.rx2`,
`d:\data\state\MotionGraphIncludes\...`), as on the disc build.
