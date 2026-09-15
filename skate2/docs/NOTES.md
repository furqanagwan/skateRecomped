# Research notes

## Title info
- Title: Skate 2 (Europe), English, Spanish, Italian, Dutch
- Title ID: 4541087F, media ID 6ADB5821, executable version 0.0.0.2 (built 2008-12-10)
- Achievements: 46, 1000 Gamerscore
- Guest DLL modules: none
- Disc: 142 files, 6,754,906,074 bytes; `default.xex` is 5,713,920 bytes

## Codegen

### 2026-09-15: first pass
`stabilize_codegen.py` seeded one unresolved call target and codegen was clean in
two rounds of about 20 seconds each. Six `vpkd3d128` float16_4 packs are
unhandled and one function (0x827550B8) is 1.35 MB, over `max_file_size_bytes`;
both compile. The first build linked with no missing kernel imports.

### 2026-09-15: discovery
With the image dumped: 152 functions referenced from data and 1,043 from code
gaps. Stabilizing then reported 5,193 split branches in one round (401 seeds
disabled) and 23 in the next; `prune_bad_seeds.py --image` disabled one more.

## Runtime

### 2026-09-15: illegal instruction on boot
The first launch died about a second in with `0xC000001D`. Relinking with
`-Wl,/DEBUG` and symbolizing the fault offset from the Windows event log put it
in `sub_82695938`, on a generated switch's out-of-range trap: the path
`cmplwi r11,3; beq` jumps straight to the table at 0x82696B38 with index 3, but
codegen sized the table as 3 slots from a bounds check on another path. The
table has 5. `find_short_switch_tables.py` (added to the framework for this)
found the same shape in `sub_82696F48` and `sub_82697270`; all three are in
`config/switch_tables.toml`.

### 2026-09-15: unregistered function
Next launch: `Call to invalid or unregistered function at guest address
0x827E6BE0`. Nothing pointed at it from data; the address is built with
`lis`/`addi` at 0x827E54CC while registering a group of callbacks, and it is a
label inside the stub before it (`mr r4,r5; b 0x827E6BE0` at 0x827E6BD8).
`find_missing_functions.py --code-refs` (also new) seeded it and 20 more
callbacks built nearby; codegen stayed clean.

### 2026-09-15: main menu
Boots, compiles shaders and shows the main menu with the 3D city backdrop within
30 seconds (window screenshots at 30 and 100 seconds). Ran for two minutes
without errors. Not yet tested: controller input in menus, career, free skate.

Logged but harmless so far: `dlcbig:\data\audio\french\` fails to resolve (no DLC
installed) and many `XFileSectorInformation` stub calls while streaming.
