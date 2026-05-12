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
| `macos`   | `macos-latest` (Apple Silicon) | `BloonsTD-macos-arm64` — fully bundled via `dylibbundler`, runs on any Mac |
| `windows` | `windows-latest` + MSYS2/MinGW64 | `BloonsTD-windows-x64` — exe + recursively-resolved MinGW DLLs |

Artifacts are uploaded to the workflow run page; download from the Actions tab
on GitHub.

## How to compile with MinGW for Windows
This project depends on:
* SDL2-2.0.9
* SDL2_image-2.0.5
* SDL2_gfx-1.0.4
* SDL2_ttf-2.0.15
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
brew install cmake sdl2 sdl2_image sdl2_ttf sdl2_gfx boost
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

### Standalone (redistributable) bundle for macOS
Produces a self-contained directory that runs on any Apple Silicon Mac without
Homebrew installed:
```bash
brew install dylibbundler          # one-time
cmake --build build -j8
mkdir -p dist/BloonsTD/{bin,lib}
cp build/BloonsTD dist/BloonsTD/bin/
cp -R assets      dist/BloonsTD/assets
cd dist/BloonsTD
dylibbundler -od -b -x bin/BloonsTD -d lib -p '@executable_path/../lib/'
```
Ship `dist/BloonsTD/`. To launch: `cd dist/BloonsTD/bin && ./BloonsTD`.
First run on another Mac may trigger Gatekeeper (the binary is ad-hoc signed,
not notarized); clear quarantine with
`xattr -dr com.apple.quarantine BloonsTD/`.

[`this`]: <https://stackoverflow.com/questions/36519453/setup-boost-in-clion>
[`releases`]: <https://github.com/idomoz/BloonsTD/releases>
[`SDL2`]: <https://www.libsdl.org/>
