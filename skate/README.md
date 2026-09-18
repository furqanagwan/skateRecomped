<p align="center">
  <img src="docs/icon.png" alt="Skate" width="320">
</p>

<h1 align="center">Skate</h1>

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
| Released | September 2007 |
| Genre | Sports, skateboarding |
| Achievements | 44, 1000 Gamerscore |

## Regions

| Region | Serial | Status |
| --- | --- | --- |
| 🇪🇺 Europe (English, Spanish, Italian) | `EA-2067` | ✅ Tested (the disc below) |
| 🇪🇺 Europe (French, German) | `EA-2067` | ⬜ Not tested |
| 🇺🇸 USA | `EA-2067` | ⬜ Not tested |
| 🇯🇵 Japan | `EA-2067` | ⬜ Not tested |
| 🌏 Asia | `EA-2067` | ⬜ Not tested |

This build targets the tested disc as patched by Title Update 2. Other regional
executables and other updates are likely to differ and need their own codegen
pass.
Region list from [Redump](http://redump.org/discs/system/xbox360/).

## Disc

| | |
| --- | --- |
| Region | 🇪🇺 Europe |
| Title ID | `45410813` |
| Media ID | `1F6E4912` |
| Executable version | 0.0.0.4 on the disc; this build is recompiled from Title Update 2 (0.0.2.4) |
| Required update | Title Update 2 (media ID `1F6E4912`, version `00000004`) |
| Languages | English, Spanish, Italian |
| Contents | 148 files, 5,344,017,481 bytes |
| Executable | `default.xex`, 6,291,456 bytes |
| DLL modules | None |
| Video format | EA VP6 (`.vp6`) |

## Status

| Area | State |
| --- | --- |
| Boot, shader compilation | Working |
| Intro cinematic | Plays with correct colour |
| Title screen, main menu | Reached, driven with a controller |
| Gameplay | Skating around Downtown with the HUD, minimap, pedestrians, traffic and line scoring |
| Stability | Ran three minutes with a controller without errors |
| Performance | 59.9 fps average, 43.5 fps 1% low (emulated GPU, RTX 5080 Laptop GPU) |
| Career | Not yet tested |
| Audio | Initializes; not yet checked by ear |
| Title update | Title Update 2 required: the recompiled code is the update's, so the update must be installed. The disc's own executable would need its own build (`config/`, kept for it) |
| DLC | Installer in place (see the [root README](../README.md#dlc)); no packages tested |
| Xbox PC app, UWP builds | Configured, not yet tested |
| Linux, macOS, Steam Deck | Builds expected, not play-tested |

## Play

1. Download `Skate-v<version>-windows-x64.zip` from
   [Releases](https://github.com/furqanagwan/skateRecomped/releases?q=skate)
   and extract it to a folder you can write to.
2. Run `Skate.exe` and choose your Xbox 360 ISO (European English disc, see
   [Regions](#regions)); the files are copied once.
3. When prompted, choose your own Title Update 2 package, or let the game
   fetch it. The matching package has title ID `45410813`, media ID
   `1F6E4912`, and version `00000004`; it is checked against this build and
   installed once. Skate's other Title Update 2 package (media ID
   `21D6D331`) patches a different release of the disc and is rejected.
4. Open the system menu with **View + Menu** (or **Esc**) for Settings and Exit.

## System requirements

| | Required |
| --- | --- |
| OS | Windows 10 version 2004 (build 19041) or Windows 11, 64-bit |
| Processor | 64-bit x86 CPU with SSE4.1 |
| Graphics | DirectX 12 GPU (feature level 11_0) |
| Memory | 8 GB RAM recommended |
| Storage | 5.5 GB, plus room for the ISO while it is copied |
| Software | [Microsoft Visual C++ Redistributable 2015-2022 (x64)](https://aka.ms/vs/17/release/vc_redist.x64.exe) |
| Game | Your own Skate (Europe, English/Spanish/Italian) Xbox 360 disc image and your own Title Update 2 package |

Tested on an Intel Core Ultra 9 275HX, GeForce RTX 5080 Laptop GPU and 32 GB RAM
(Windows 11).

## Build from source

```
rexglue extract "<your disc>.iso" skate\assets
rexglue package "<your Title Update 2 package>" skate\title_updates\staging\tu1
copy skate\assets\default.xex skate\title_updates\staging\tu1
.\framework\scripts\build.ps1 -Game skate
```

Codegen for Skate takes 5 to 7 minutes per pass. Setup is described in
[CONTRIBUTING.md](../CONTRIBUTING.md).

## Default settings

`settings/skate.toml` starts from the NBA LIVE settings (`rov` / `fsi` render
target paths, no background pipeline creation, 60 Hz vsync). Whether Skate
needs each of them has not been tested separately.

## Recompilation notes

| | |
| --- | --- |
| Generated sources | 248 files, about 269 MB |
| Configs | `config/tu2/` for this Title Update 2 build; `config/` holds the disc build's, which its own seeds do not fit |
| Function seeds | 1,820 in `config/tu2/functions.toml`, including explicit bounds for 10 functions codegen split wrongly |
| Disabled seeds | 419 in `config/tu2/disabled_function_seeds.txt` (they split functions or loops) |
| Jump tables | No under-counted tables |
| Kernel stubs | None needed beyond the framework's |
| Known codegen warnings | Unhandled `vpkd3d128` float16_4 packs |

The full log of what was found and fixed is in [docs/NOTES.md](docs/NOTES.md).

## Xbox Developer Mode (UWP)

```powershell
.\framework\scripts\build.ps1 -Game skate -Preset win-amd64-uwp-release
.\framework\scripts\package_uwp.ps1 -Game skate -Register
.\framework\scripts\package_uwp.ps1 -Game skate -Pack
```

## Artwork

`docs/icon.png` is the title image from `default.xex`, upscaled to 1024x1024.
To regenerate the exe icon and Xbox app images locally:

1. `rexglue init --project-name skate --xex-path assets\default.xex achievements assets\default.xex metadata`
2. Upscale `metadata/icons/title.png` 4x twice with Real-ESRGAN
   (`realesrgan-x4plus`) to `metadata/gdk_hd/title_1024.png`, or copy
   `docs/icon.png` there.
3. `.\framework\scripts\generate_artwork.ps1 -Game skate -ProjectName skate`

## Legal

Not affiliated with or endorsed by Electronic Arts or Microsoft. Skate and EA are
trademarks of Electronic Arts. You must own the game.
