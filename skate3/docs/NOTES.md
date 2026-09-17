# Research notes

Dated entries of what was found and fixed, newest at the bottom of each section.
Record the evidence (log lines, addresses, what a dump showed), not only the fix.

## Title info
- Title: Skate 3 (USA, Europe), English, French, German, Spanish, Italian, Dutch
- Title ID: 454108E6, media ID 5C087C2C, executable version 0.0.0.3 (retail, no title update)
- Achievements: 47, 1000 Gamerscore
- Guest DLL modules: `data/webkit/EAWebkit.xex` (4,923,392 bytes)
- Disc: 103 files, 6,404,940,920 bytes; `default.xex` is 6,615,040 bytes

## Prior work: skate3recomp

[skate3recomp](https://github.com/mchughalex/skate3recomp) runs Skate 3 on its own
ReXGlue 0.8 fork. It recompiles the Title Update 3 executables; this project, like
Skate and Skate 2, recompiles the disc's executable and skips title updates. Its
retail fixes, checked against this build:

- setjmp at 0x82F44E40 (its "exception guard" workaround): found by
  `find_setjmp.py` and mapped in `config/setjmp.toml`.
- A host page committed at guest 0x00010000 for a null-offset read: needed here
  too (see Runtime).
- 0x82EBAE4C, an alternate entry into 0x82EBAE34 referenced from 37 vtables: not
  hit yet.
- `big:` and `dlcbig:` aliased to the game partition, and empty directories for
  paths that only exist inside BIG archives (`data/scene`,
  `data/content/recipe/<alias>`, `data/livingworld/PluginDescriptor`,
  `data/state/livingworldentities/pedestrian/plugin`): the lookups fail here too
  (see Runtime), with no visible effect so far.

## Codegen

### 2026-09-16: first pass
`rexglue init --scan-dll` found `data/webkit/EAWebkit.xex`; `new_game.ps1` gave it
the executable's `functions.toml` (fixed in the framework), so it now has its own
`config/eawebkit/functions.toml`. Stabilizing seeded 2 unresolved call targets in
the executable and 1 in EAWebkit; codegen was clean in three rounds. One function
(0x823D7368) is 1.09 MB, over `max_file_size_bytes`, and six `vpkd3d128`
float16_4 packs are unhandled, as in Skate 2; both compile. skate3recomp's SDK
changed how these packs are generated, which is worth comparing if vertex data
looks wrong.

### 2026-09-16: discovery
With the image dumped: 163 functions referenced from data, 740 from code gaps and
23 built in code. Stabilizing then reported 5,208 split branches (295 seeds
disabled), then 2 (46 more); pruning disabled 2 more and a last round 1. Result: 588
seeds, 343 disabled. setjmp 0x82F44E40 / longjmp 0x82F25260 in the executable and
0x88478D00 / 0x884789E0 in EAWebkit. EAWebkit was not loaded within 120 seconds,
so it has no dump or scans yet.

## Runtime

### 2026-09-17: null-offset read on boot
The first launch died a second in on guest thread 6: `read of guest 0x00012160`
in `sub_826B57D0` (symbolized after relinking with `-Wl,/DEBUG`). The code is
`lwz r10,32200(r19)` / `ldx r27,r10,r6` with r19 = 0x83020000 and r6 = 0x12160:
it reads a 64-bit field of the object whose pointer lives at 0x83027DC8. That
global is zero in the loaded image, and its only writers are later in the same
function (0x826B62D8, after allocating the 0x44680-byte object), so the read
always sees null. skate3recomp's SDK commits the host page at guest
0x00010000-0x0001FFFF for this read. The SDK now does the same behind the
`map_low_null_offset_page` cvar (off by default, so other games are unchanged),
turned on in `settings/skate_3.toml`.

### 2026-09-17: 643-case jump table
Next: `0xC000001D` after 30 seconds, on the EA logo, in `sub_823D7368`'s switch
at 0x823D73A4. Its bounds check is `cmplwi r11,642`, but codegen stops at
`max_jump_table_entries` (512) and the targets past that are never labels, so
`find_short_switch_tables.py` did not see the table as short. The script now also
sizes a table from the unsigned bounds check on its index register (when every
extra slot points at code); it finds only this table here and nothing new in
Skate or Skate 2. The 643 labels are in `config/switch_tables.toml`.

### 2026-09-17: intro and attract demo
Ran for three minutes without errors: EA logo, the intro cinematic (60 s), then
a skater skating through the city with the HUD and no input (175 s). 48 fps
average, 7.3 fps 1% low, 9 stalls over 100 ms (worst 4.3 s, cause not yet
looked at); 2,800 draws per frame median. The GPU command thread is busy for the
whole frame, so emulation limits the frame rate. Not yet tested: menus with input,
career, free skate, EAWebkit.

Logged but harmless so far: `ResolvePath(big:\data\content\global_locators\...)`
and the same under `dlcbig:` (no such devices), `NtCreateFile` failures for
directories that only exist inside BIG archives (`d:\data\scene\`,
`d:\data\content\recipe\...`), and `d:\fileserver.ini`.

### 2026-09-17: naming the team and the player
A new save asks for a team name ("Enter New Team Name", default "New Team")
and then the player's ("Enter your name:"), both through XamShowKeyboardUI
with a 16-character buffer. They now open the framework's on-screen keyboard:
the team name went back to the game, which then asked for the player's name.
