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

### 2026-09-15: latent unresolved branches
After the framework learned to read leftover `REX_FATAL` stubs from the
generated code (see Skate's notes), it found eight branches into the middle of
functions that no run had reached yet, caused by nine gap seeds; all nine are
disabled. Result: 802 seeds, no unresolved stubs.

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

### 2026-09-17: recompiled from Title Update 2
The build targets Title Update 2, from `TU_12K223V_0000008000000.00000000000G2`
(title ID 4541087F, media ID 6ADB5821 matching the disc, version 00000002). The
folder also holds updates 1 and 4, whose media IDs are 151D8210 and 7B479EFC, so
they belong to other discs. The runtime installs it and reports `XEX patch applied
successfully: base version: 0.0.0.2, new version: 0.0.2.2`.

The first attempt reused this folder's disc-build seeds and generated 908
`REX_FATAL("Unresolved ...")` stubs, for the same reason as Skate 3: an update
moves code, so those seeds split its functions.

The second attempt, with an empty `config/tu2/`, looked clean at 1 seed - but the
image was never dumped (`default was not dumped: the game did not load it within
120 s`), because with no update installed the build waits on the title-update
dialog, so the data, gap and code-built scans never ran. Installing the update
first and rerunning discovery gave the real config: 145 functions referenced from
data, 1,048 from code gaps, 21 built in code, 799 seeds, 417 disabled, three
under-counted jump tables, no stubs. setjmp/longjmp are at 0x82CE2E60 /
0x82C89C10, where the disc build has 0x82CDE7C0 / 0x82C85590.

Ran three minutes at the title screen ("Press START to continue", with the
warehouse scene behind it) without errors: 31.7 fps average, 26.1 fps 1% low,
5,079 draws per frame median, the GPU command thread busy 99% of the frame.
