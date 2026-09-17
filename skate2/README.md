<p align="center">
  <img src="docs/icon.png" alt="Skate 2" width="320">
</p>

<h1 align="center">Skate 2</h1>

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
| Released | January 2009 |
| Genre | Sports, skateboarding |
| Achievements | 46, 1000 Gamerscore |

## Regions

| Region | Serial | Status |
| --- | --- | --- |
| 🇪🇺 Europe (English, Spanish, Italian, Dutch) | `EA-2175` | ✅ Tested (the disc below) |
| 🇪🇺 Europe (French, German) | `EA-2175` | ⬜ Not tested |
| 🇩🇪 Germany | `EA-2223` | ⬜ Not tested |
| 🇺🇸 USA | `EA-2175` | ⬜ Not tested |
| 🇯🇵 Japan | `EA-2175` | ⬜ Not tested |

This build targets the tested disc with Title Update 2. Other regional
executables and title updates may need their own codegen pass.
Region list from [Redump](http://redump.org/discs/system/xbox360/).

## Disc

| | |
| --- | --- |
| Region | 🇪🇺 Europe |
| Title ID | `4541087F` |
| Media ID | `6ADB5821` |
| Executable version | 0.0.0.2 (built 2008-12-10) |
| Required update | Title Update 2 (`00000002`) |
| Languages | English, Spanish, Italian, Dutch |
| Contents | 142 files, 6,754,906,074 bytes |
| Executable | `default.xex`, 5,713,920 bytes |
| DLL modules | None |
| Video format | EA VP6 (`.vp6`) |

## Status

| Area | State |
| --- | --- |
| Boot, shader compilation | Working |
| Main menu | Reached, with the 3D city backdrop |
| Stability | Ran two minutes at the main menu without errors |
| Controller input, career, free skate | Not yet tested |
| Audio | Initializes; not yet checked by ear |
| DLC | Installer in place (see the [root README](../README.md#dlc)); no packages tested |
| Xbox PC app, UWP builds | Configured, not yet tested |
| Linux, macOS, Steam Deck | Builds expected, not play-tested |

## Play

1. Download `Skate2-v<version>-windows-x64.zip` from
   [Releases](https://github.com/furqanagwan/skateRecomped/releases?q=skate2)
   and extract it to a folder you can write to.
2. Run `Skate 2.exe` and choose your Xbox 360 ISO (European English disc, see
   [Regions](#regions)); the files are copied once.
3. When prompted, choose your own Title Update 2 package. The matching package
   has title ID `4541087F`, media ID `6ADB5821`, and version `00000002`.
4. Open the system menu with **View + Menu** (or **Esc**) for Settings and Exit.

## System requirements

| | Required |
| --- | --- |
| OS | Windows 10 version 2004 (build 19041) or Windows 11, 64-bit |
| Processor | 64-bit x86 CPU with SSE4.1 |
| Graphics | DirectX 12 GPU (feature level 11_0) |
| Memory | 8 GB RAM recommended |
| Storage | 7 GB, plus room for the ISO while it is copied |
| Software | [Microsoft Visual C++ Redistributable 2015-2022 (x64)](https://aka.ms/vs/17/release/vc_redist.x64.exe) |
| Game | Your own Skate 2 (Europe, English/Spanish/Italian/Dutch) Xbox 360 disc image |

Tested on an Intel Core Ultra 9 275HX, GeForce RTX 5080 Laptop GPU and 32 GB RAM
(Windows 11).

## Build from source

```
rexglue extract "<your disc>.iso" skate2\assets
rexglue package "<your Title Update 2 package>" skate2\title_updates\staging
.\framework\scripts\build.ps1 -Game skate2
```

Setup is described in [CONTRIBUTING.md](../CONTRIBUTING.md).

## Default settings

`settings/skate_2.toml` starts from the NBA LIVE settings (`rov` / `fsi` render
target paths, no background pipeline creation, 60 Hz vsync). Whether Skate 2
needs each of them has not been tested separately.

## Recompilation notes

| | |
| --- | --- |
| Generated sources | 177 files, about 186 MB |
| Function seeds | 802 in `config/functions.toml` |
| Disabled seeds | 416 in `config/disabled_function_seeds.txt` (they split functions or loops) |
| Jump tables | 3 under-counted tables fixed in `config/switch_tables.toml` |
| Kernel stubs | None needed beyond the framework's |
| Known codegen warnings | 6 unhandled `vpkd3d128` float16_4 packs, one 1.35 MB function |

The full log of what was found and fixed is in [docs/NOTES.md](docs/NOTES.md).

## Xbox Developer Mode (UWP)

```powershell
.\framework\scripts\build.ps1 -Game skate2 -Preset win-amd64-uwp-release
.\framework\scripts\package_uwp.ps1 -Game skate2 -Register
.\framework\scripts\package_uwp.ps1 -Game skate2 -Pack
```

## Artwork

`docs/icon.png` is the title image from `default.xex`, upscaled to 1024x1024.
To regenerate the exe icon and Xbox app images locally:

1. `rexglue init --project-name skate_2 --xex-path assets\default.xex achievements assets\default.xex metadata`
2. Upscale `metadata/icons/title.png` 4x twice with Real-ESRGAN
   (`realesrgan-x4plus`) to `metadata/gdk_hd/title_1024.png`, or copy
   `docs/icon.png` there.
3. `.\framework\scripts\generate_artwork.ps1 -Game skate2 -ProjectName skate_2`

## Legal

Not affiliated with or endorsed by Electronic Arts or Microsoft. Skate and EA are
trademarks of Electronic Arts. You must own the game.
