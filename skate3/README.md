<p align="center">
  <img src="docs/icon.png" alt="Skate 3" width="320">
</p>

<h1 align="center">Skate 3</h1>

<p align="center">
  Native PC static recompilation of the Xbox 360 version, built on the
  <a href="../README.md">skateRecomped</a> framework and ReXGlue.
</p>

## Game

| | |
| --- | --- |
| Developer | EA Black Box |
| Publisher | Electronic Arts |
| Series | Skate |
| Platform recompiled | Xbox 360 |
| Released | May 2010 |
| Genre | Sports, skateboarding |
| Achievements | 47, 1000 Gamerscore |

## Regions

| Region | Serial | Status |
| --- | --- | --- |
| 🇺🇸🇪🇺 USA, Europe (English, French, German, Spanish, Italian, Dutch) | `TODO` | ✅ Tested (the disc below) |

Only the tested disc's executable has been recompiled, as patched by Title
Update 3. Other regional executables, and other updates, are likely to differ and
need their own codegen pass.
Region list from [Redump](http://redump.org/discs/system/xbox360/).

## Disc

| | |
| --- | --- |
| Region | 🇺🇸🇪🇺 USA, Europe |
| Title ID | `454108E6` |
| Media ID | `5C087C2C` |
| Executable version | 0.0.0.3 on the disc; this build is recompiled from Title Update 3 (0.0.3.3) |
| Languages | English, French, German, Spanish, Italian, Dutch |
| Contents | 103 files, 6,404,940,920 bytes |
| Executable | `default.xex`, 6,615,040 bytes |
| DLL modules | `data/webkit/EAWebkit.xex` (4,923,392 bytes) |

## Status

| Area | State |
| --- | --- |
| Boot, shader compilation | Working |
| Intro | EA logo and intro cinematic play, then a skater skates through the city with the HUD |
| Gameplay | Skating with the trick HUD, pedestrians and shadows, driven with a controller |
| Stability | Ran two minutes of gameplay and three minutes of the intro without errors |
| Performance | 56 fps average, 14 fps 1% low in gameplay (emulated GPU, RTX 5080 Laptop GPU) |
| Menus, career, free skate | Not yet tested beyond the above |
| EAWebkit module | Recompiled and discovered offline for the disc and TU3 builds; 119 function seeds each |
| Audio | Initializes; not yet checked by ear |
| Title update | Title Update 3 required: the recompiled code is the update's, so the update must be installed. The disc's own executable would need its own build (`config/`, kept for it) |
| DLC | Installer in place (see the [root README](../README.md#dlc)); no packages tested |
| Xbox PC app, UWP builds | Configured, not yet tested |
| Linux, macOS, Steam Deck | Builds expected, not play-tested |

## Play

1. Download `Skate3-v<version>-windows-x64.zip` from
   [Releases](https://github.com/furqanagwan/skateRecomped/releases?q=skate3)
   and extract it to a folder you can write to.
2. Run `Skate 3.exe` and choose your Xbox 360 ISO (see
   [Regions](#regions)); the files are copied once.
3. When asked, choose your own Title Update 3 package (the
   `TU_12K2276_000000C000000.00000000000O3` file an Xbox 360 downloads). It is
   checked against this build and installed once.
4. Open the system menu with **View + Menu** (or **Esc**) for Settings and Exit.

## System requirements

Keep this table in step with `release.json`.

| | Required |
| --- | --- |
| OS | Windows 10 version 2004 (build 19041) or Windows 11, 64-bit |
| Processor | 64-bit x86 CPU with SSE4.1 |
| Graphics | DirectX 12 GPU (feature level 11_0) |
| Memory | 8 GB RAM recommended |
| Storage | 6.5 GB, plus room for the ISO while it is copied |
| Software | [Microsoft Visual C++ Redistributable 2015-2022 (x64)](https://aka.ms/vs/17/release/vc_redist.x64.exe) |
| Game | Your own Skate 3 Xbox 360 disc image |

## Build from source

```
rexglue extract "<your disc>.iso" skate3\assets
.\framework\scripts\build.ps1 -Game skate3
```

Setup is described in [CONTRIBUTING.md](../CONTRIBUTING.md).

## Default settings

`settings/skate_3.toml` starts from the framework defaults (`rov` / `fsi`
render target paths, no background pipeline creation, 60 Hz vsync), as Skate 2
does. It also sets `map_low_null_offset_page = true`: the game reads through a
null pointer while it boots (see [docs/NOTES.md](docs/NOTES.md)).

## Recompilation notes

| | |
| --- | --- |
| Modules | `default.xex`, and `data/webkit/EAWebkit.xex` in `config/tu3/eawebkit/` |
| Generated sources | 213 files, about 221 MB (EAWebkit about 66 MB more) |
| Configs | `config/tu3/` for this Title Update 3 build; `config/` holds the disc build's, which its own seeds do not fit |
| Function seeds | 592 in `config/tu3/functions.toml` |
| Disabled seeds | 344 in `config/tu3/disabled_function_seeds.txt` (they split functions or loops) |
| Jump tables | One under-counted table in `config/tu3/switch_tables.toml` |
| Kernel stubs | None needed beyond the framework's |
| Known codegen warnings | 6 unhandled `vpkd3d128` float16_4 packs, one 1.09 MB function |

The full log of what was found and fixed is in [docs/NOTES.md](docs/NOTES.md).

## Xbox Developer Mode (UWP)

```powershell
.\framework\scripts\build.ps1 -Game skate3 -Preset win-amd64-uwp-release
.\framework\scripts\package_uwp.ps1 -Game skate3 -Register
.\framework\scripts\package_uwp.ps1 -Game skate3 -Pack
```

## Artwork

`docs/icon.png` is the title image from `default.xex`, upscaled to 1024x1024.
To regenerate the exe icon and Xbox app images locally:

1. `rexglue init --project-name skate_3 --xex-path assets\default.xex achievements assets\default.xex metadata`
2. Upscale `metadata/icons/title.png` 4x twice with Real-ESRGAN
   (`realesrgan-x4plus`) to `metadata/gdk_hd/title_1024.png`, or copy
   `docs/icon.png` there.
3. `.\framework\scripts\generate_artwork.ps1 -Game skate3 -ProjectName skate_3`

## Legal

Not affiliated with or endorsed by Electronic Arts or Microsoft. Skate and EA are
trademarks of Electronic Arts. You must own the game.
