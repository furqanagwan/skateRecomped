# Research notes

## Title info
- Title: Skate (Europe), English, Spanish, Italian
- Title ID: 45410813, media ID 1F6E4912, executable version 0.0.0.4 (built 2007-08-22)
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
