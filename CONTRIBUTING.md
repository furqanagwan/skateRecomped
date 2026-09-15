# Contributing

Thanks for helping. Setup, building, adding a game, the codegen workflow and
code style are shared by every repository that uses the framework, so they live
in [recomp-framework's CONTRIBUTING.md](https://github.com/furqanagwan/recomp-framework/blob/main/CONTRIBUTING.md).
Read that first; this file only covers what is specific to this repository.

## Where changes go

| Change | Repository |
| --- | --- |
| A Skate game: codegen config, seeds, game-only kernel stubs, settings, README, research notes | This repository, in the game's folder |
| Installer, menus, input, settings UI, packaging or analysis scripts | [recomp-framework](https://github.com/furqanagwan/recomp-framework), then update the `framework` submodule here |
| Recompiler, kernel, graphics, audio, file system | [furqanagwan/rexglue-sdk](https://github.com/furqanagwan/rexglue-sdk) |

## Testing a change

Say in your pull request which games and disc regions you tested, and for how
long (boot to menus, free skate, career mode...). Record anything new you
learn about a game in its `docs/NOTES.md`, and keep the status tables in the
game's README honest.

Updating the framework submodule:

```
git -C framework pull origin main
git add framework
```

## Making a release

Releases can only be built by someone who owns the game, because codegen needs
`default.xex`. For each game:

1. Update `framework` to the commit you want and make sure the working tree is
   clean.
2. Install the SDK from `framework/thirdparty/rexglue-sdk` (see the framework
   guide) so the build uses the pinned SDK.
3. Build a release: `.\framework\scripts\build.ps1 -Game <GAME>`.
4. Play-test it from a fresh folder: extract the zip produced in the next step
   somewhere new, run it, install from the ISO and play.
5. Package and publish:
   ```
   .\framework\scripts\package_release.ps1 -Game <GAME> -Version <x.y.z>
   .\framework\scripts\package_release.ps1 -Game <GAME> -Version <x.y.z> -Publish
   ```
   This creates the tag `<GAME>-v<x.y.z>` (for example `skate-v0.1.0`) and
   a GitHub release with the zip, system requirements from `<GAME>/release.json`
   and the SHA-256. Add `-Draft` to review it before it goes public; a version
   with a suffix such as `0.1.0-beta.1` is marked as a pre-release.
6. Update the game README's status table if anything changed.

Each game is released separately, so a Skate fix doesn't need a new
Skate 2 release.
