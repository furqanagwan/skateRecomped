# Research notes

Dated entries of what was found and fixed, newest at the bottom of each section.
Record the evidence (log lines, addresses, what a dump showed), not only the fix.

## Title info
- Title: Skate 3 (USA, Europe), English, French, German, Spanish, Italian, Dutch
- Title ID: 454108E6, media ID 5C087C2C, executable version 0.0.0.3 on the disc; this
  build is recompiled from Title Update 3 (0.0.3.3)
- Achievements: 47, 1000 Gamerscore
- Guest DLL modules: `data/webkit/EAWebkit.xex` (4,923,392 bytes)
- Disc: 103 files, 6,404,940,920 bytes; `default.xex` is 6,615,040 bytes

## Prior work: skate3recomp

[skate3recomp](https://github.com/mchughalex/skate3recomp) runs Skate 3 on its own
ReXGlue 0.8 fork. It recompiles the Title Update 3 executables, as this build now
does as well; the disc build's own config is kept in `config/`. Its findings,
checked against the disc build here:

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

### 2026-09-17: recompiled from Title Update 3
The build now targets Title Update 3 (`default.xexp` and `data/webkit/EAWebkit.xexp`
from `TU_12K2276_000000C000000.00000000000O3`), which the runtime installs and
applies: `XEX patch applied successfully: base version: 0.0.0.3, new version:
0.0.3.3`.

The first attempt reused this folder's disc-build seeds and generated 1,475
`REX_FATAL("Unresolved ...")` stubs, because the update moves code and those seeds
land inside its functions and split them. It died five seconds in on
`Unresolved branch from 0x82F72D20 to 0x82F72D0C`, where 0x82F72D20 is a
disc-build seed.

The update now has its own config (`config/tu3/`, and `config/tu3/eawebkit/` for
the patched DLL), discovered against the patched image: 592 seeds, 344 disabled,
one under-counted jump table, no stubs left. Its CRT setjmp/longjmp are at
0x82F6FAA0 / 0x82F4FEB0, not the disc build's 0x82F44E40 / 0x82F25260 - the
clearest illustration of why a version needs its own pass. EAWebkit never loads
in the tested offline menus, so the SDK now dumps its fully
loaded, patched image during codegen. Discovery against that image produced 119
active seeds and the module still builds cleanly. EA's official Skate 3 GPL release
identifies this module as EAWebKit 1.10.01 and provides its Xenon Visual Studio
2008 project and source: https://gpl.ea.com/skate3.html

Played for two minutes with a controller: skating, tricks scoring, pedestrians
and shadows, 56 fps average and 14 fps 1% low, no errors. The run ended when the
guide's Leave Game was chosen, not on a fault. The same `big:` and `dlcbig:`
locator lookups fail as on the disc build.

## Renderer

### 2026-09-18: what a city frame draws
Traced the attract demo's city, which needs no input: the EA logo and intro run
to about frame 1,200, and the city holds from there to the end of the run. 2,949
draws in the frame traced, 55.6 fps average over 100 s.

The frame is **tiled**. Every world draw is submitted twice, and the second half
is an exact replay of the first: same draw count per program, same index count
draw for draw, for both the colour and the depth programs. The Xenos splits the
1200-pitch 2x MSAA target across EDRAM tiles and the guest replays the command
list once per tile. A native renderer has to draw this scene once, or it renders
and pays for everything twice.

There is a **depth pre-pass**: `B4E21448711110DE:A4A965C189287B99`, 388 draws,
`edram_mode` 5 (`kDepthOnly`), no textures. The same geometry is drawn again for
colour.

The programs in a city frame, by draws:

| vertex:pixel | draws | indices/draw | textures | pass |
| --- | --- | --- | --- | --- |
| `F91B29D8FC044DBF:959B4F51AB838BB0` | 924 | 345 | 2 | colour, the world |
| `B4E21448711110DE:A4A965C189287B99` | 388 | 443 | 0 | depth pre-pass |
| `B56724DBA4CA4FE0:C24E796B8CA7A91A` | 228 | 238 | 1 | colour |
| `B6C9863F710683EC:A4A965C189287B99` | 216 | 1 | 0 | not geometry, see below |
| `B35C2C3AC53D594A:F8CA956A5B036EEE` | 204 | 413 | 10 | colour, layered material |

The hashes in skateRecomped#4 came from an earlier capture and no longer
describe this build: its two textured candidates draw 24 times and not at all.
Its one negative result holds and is stronger than recorded - `B6C9863F710683EC`
draws a *single index as a point list*, 216 times, spread over four surface
pitches, so it is a per-pass marker rather than overlay geometry.

### 2026-09-18: the world is already in world space
None of the world programs carries a per-object transform. For
`F91B29D8FC044DBF` nothing in c0-c15 changes per draw at all; `B35C2C3AC53D594A`
does have per-draw constants but they are scalars in `.x` (0.3, 8, 1), material
parameters rather than a matrix.

The transform chain, checked numerically rather than read off:

- **c0-c3 is the view-projection**, row-major, w row in c3.
- **c4 is the camera in world space.** `c3 . (c4,1)` is 0.00001 in every frame
  sampled, so the camera lies exactly on the w=0 plane, which only holds if c4
  is the eye and c3 is the w row.
- **The near plane is 0.1**: `c2 . (camera,1)` is -0.1000 every frame.
- c5-c7 never change. c8-c11 is a *fixed* frame - c11 is (22.03, 112.39,
  -459.79) while the camera moves, so it is a light or sector origin, not the
  camera as first assumed. c12-c15 changes per frame.

Vertex buffers back this up: one buffer per draw, and the addresses persist -
193 of 200 are the same ten frames apart, at a 20-byte stride (5 words) for
`F91B29D8FC044DBF` and 28 bytes for the other two. Resident, long-lived,
world-space geometry a native renderer can read directly and draw with its own
view-projection. That is the good case; no per-object matrices to reconstruct.
