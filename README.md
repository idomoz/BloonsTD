# BloonsTD
* A remake of Ninja Kiwi's BTD5 + BTD6 games.
* Written in C++ with the [`SDL2`] library for graphic rendering.
## Preview
<img src="/assets/preview.gif?raw=true" width="600px">

## Project layout
```
BloonsTD-master/
├── assets/              # game assets (shared)
├── src/                 # game sources (shared)
├── CMakeLists.txt       # dispatcher — picks the right config below
├── build-windows.cmake  # Windows / MinGW toolchain & deps
├── build-macos.cmake    # macOS / Homebrew toolchain & deps
└── scripts/
```
Both platforms compile from the same `src/`. The top-level `CMakeLists.txt`
detects the host platform and `include()`s the matching `build-*.cmake`.

A few minor source tweaks were needed for portability; all are cross-platform
safe and compile cleanly with both Apple Clang and MinGW:

| File | Change | Reason |
| --- | --- | --- |
| `src/Game.h` | Removed `#include <comdef.h>` | Windows-only COM header — was never used; broke Apple Clang. |
| `src/Entity.h` | `pow(2, bit)` → `(uint64_t)1 << bit` | Apple Clang's `libc++` enforces strict `constexpr`; `std::pow` is not `constexpr`. Bit-shift is equivalent. |
| `src/Entity.h` | Added `#include <cstdint>` | Required by newer MinGW/libstdc++ to pull in `uint64_t`; Apple Clang's libc++ exposed it transitively. |

## How to play (pre-built Windows release)
* Go to the [`releases`] page and download `BloonsTD.7z`
* Extract `BloonsTD.7z` and run `BloonsTD/bin/BloonsTD.exe`

## Continuous integration
A GitHub Actions workflow at `.github/workflows/build.yml` builds **both**
platforms on every push to `master` and on every PR:

| Job | Runner | Output artifact |
| --- | --- | --- |
| `macos`   | `macos-latest` (Apple Silicon) | `BloonsTD-macos-arm64.zip` — `BloonsTD.app` bundled via `dylibbundler`, ad-hoc signed, zipped with `ditto` to preserve permissions and signatures |
| `windows` | `windows-latest` + MSYS2/MinGW64 | `BloonsTD-windows-x64.zip` — exe + recursively-resolved MinGW DLLs + assets |

Artifacts are uploaded to the workflow run page; download from the Actions tab
on GitHub.

### Automatic releases
On every push to `master` (not on PRs or forks), a third `release` job runs
after both build jobs succeed. It downloads the two zips and publishes them
as a GitHub Release tagged `build-<run_number>` (marked `prerelease: true`,
`make_latest: true`). The release body includes:

* the source commit SHA,
* macOS launch instructions (double-click `BloonsTD.app`; first-run may need
  `xattr -dr com.apple.quarantine BloonsTD.app` since the bundle is ad-hoc
  signed, not notarized),
* Windows launch instructions (double-click `BloonsTD.bat` at the top of
  the unzipped tree, or run `bin\BloonsTD.exe` directly).

The workflow declares `permissions: contents: write` so the release job can
publish via `softprops/action-gh-release@v2`. Releases are visible on the
repo's [Releases page][`releases`].

## How to compile with MinGW for Windows
This project depends on:
* SDL2-2.0.9
* SDL2_image-2.0.5
* SDL2_gfx-1.0.4
* SDL2_ttf-2.0.15
* SDL2_mixer-2.x (MP3 support)
* Boost-1.17.0

Steps (CLion + CMake):
1. Download the SDL2 development libraries and copy the `lib` and `include`
   directories into the project root.
2. Do the same for the rest of the `SDL2_x` libraries.
3. Compile `SDL2_gfx` yourself — it doesn't ship development libraries.
4. For Boost, follow [`this`] guide.
5. Open the project in CLion and build with the built-in CMake configuration.
   The dispatcher picks up `build-windows.cmake` automatically when `WIN32` is
   set.
6. Copy the `SDL2_x` DLLs next to `BloonsTD.exe` so it can run.

## How to compile on macOS
Install dependencies via Homebrew (Apple Silicon `/opt/homebrew` and Intel
`/usr/local` are both auto-detected):
```bash
brew install cmake sdl2 sdl2_image sdl2_ttf sdl2_gfx sdl2_mixer boost
```

Build & run:
```bash
cmake -S . -B build
cmake --build build -j8
cd build && ./BloonsTD
```
The game uses `../assets/...` paths relative to the working directory at
runtime, so launching from inside `build/` resolves correctly to the
project-root `assets/` folder — no symlinks or copies needed.

### Standalone `.app` bundle for macOS
The CI workflow (`.github/workflows/build.yml`) packages the macOS build as a
proper `BloonsTD.app` so it can be double-clicked from Finder or dragged to
Applications. To produce one locally:
```bash
brew install dylibbundler          # one-time
cmake --build build -j8

APP=dist/BloonsTD.app
mkdir -p "$APP/Contents/MacOS" "$APP/Contents/lib" "$APP/Contents/assets"
cp build/BloonsTD "$APP/Contents/MacOS/BloonsTD-bin"
cp -R assets/.    "$APP/Contents/assets/"
cat > "$APP/Contents/MacOS/BloonsTD" <<'EOF'
#!/bin/bash
cd "$(dirname "$0")"
exec ./BloonsTD-bin "$@"
EOF
chmod +x "$APP/Contents/MacOS/BloonsTD"
dylibbundler -od -b -x "$APP/Contents/MacOS/BloonsTD-bin" \
             -d "$APP/Contents/lib" -p '@executable_path/../lib/'
# (also write Contents/Info.plist — see workflow for the exact contents)
codesign --force --deep --sign - "$APP"
```

#### macOS Gatekeeper notes
The bundle is **ad-hoc signed** (no Apple Developer ID, no notarization). On a
freshly-downloaded copy macOS will refuse to load the bundled dylibs with
errors like:

```
dyld[…]: Library not loaded: @executable_path/../lib/libSDL2-2.0.0.dylib
  Reason: code signature in 'libSDL2-2.0.0.dylib' not valid for use in process:
          library load disallowed by system policy
```

Two ways to allow it:
1. **Right-click → Open** the `.app` in Finder. Approve the "unidentified
   developer" prompt once; the whole bundle is then trusted.
2. **Strip the quarantine xattr** from the command line (works on either the
   `.app` or the older directory layout):
   ```bash
   xattr -dr com.apple.quarantine BloonsTD.app
   # or, for the directory layout:
   xattr -dr com.apple.quarantine BloonsTD/
   ```

[`this`]: <https://stackoverflow.com/questions/36519453/setup-boost-in-clion>
[`releases`]: <https://github.com/idomoz/BloonsTD/releases>
[`SDL2`]: <https://www.libsdl.org/>

## Theme music
Music from #Uppbeat (free for Creators!):
https://uppbeat.io/t/matt-stewart-evans/mischief-makers
License code: LSFEPVCL2JAJTS2D