# BloonsTD
* A remake of Ninja Kiwi's BTD5 + BTD6 games.
* Written in C++ with the [`SDL2`] library for graphic rendering and
  [`SDL2_mixer`] for music + sound effects.
## Preview
<img src="/assets/preview.gif?raw=true" width="600px">

## Project layout
```
BloonsTD-master/
├── assets/                  # game assets (shared)
├── src/                     # game sources (shared)
├── ios/                     # Info.plist for the iOS .app bundle
├── third_party/             # vendored SDL2 sources for the iOS build (not committed; cloned at build time)
├── CMakeLists.txt           # dispatcher — picks the right config below
├── build-windows.cmake      # Windows / MinGW toolchain & deps
├── build-macos.cmake        # macOS / Homebrew toolchain & deps
├── build-ios.cmake          # iOS — links vendored SDL2 statically into the .app
├── build-emscripten.cmake   # Web — uses emsdk's bundled SDL2 ports (no clones)
└── scripts/
```
All four targets compile from the same `src/`. The top-level `CMakeLists.txt`
detects the host platform / configured toolchain and `include()`s the
matching `build-*.cmake`.

## How to play (pre-built release)
* Go to the [`releases`] page and download the zip file for windows/mac.
* Exctract it and run the .bat file (windows) or .app file (mac).

## Continuous integration
A GitHub Actions workflow at `.github/workflows/build.yml` builds **all four**
targets on every push to `master` and on every PR:

| Job | Runner | Output artifact |
| --- | --- | --- |
| `macos`   | `macos-latest` (Apple Silicon) | `BloonsTD-macos-arm64.zip` — `BloonsTD.app` bundled via `dylibbundler`, ad-hoc signed, zipped with `ditto` to preserve permissions and signatures |
| `windows` | `windows-latest` + MSYS2/MinGW64 | `BloonsTD-windows-x64.zip` — exe + recursively-resolved MinGW DLLs + assets |
| `ios`     | `macos-latest` | `BloonsTD-ios-simulator.zip` — unsigned `.app` for iOS Simulator (arm64). The job clones SDL2/SDL2_image/SDL2_ttf/SDL2_mixer fresh into `third_party/` per run; nothing SDL-related is stored in the repo. |
| `web`     | `ubuntu-latest` + emsdk | `BloonsTD-web` artifact (HTML/JS/WASM/data). Uses Emscripten's built-in SDL2 ports — no clones. |
| `pages`   | `ubuntu-latest` | Deploys the `web` build to **GitHub Pages** so the game is playable in any modern browser, including iPad Safari, with no install / signing dance. Master pushes only. |

Artifacts are uploaded to the workflow run page; download from the Actions tab
on GitHub. The Pages URL is shown on the workflow run page after the `pages`
job finishes (typically `https://<owner>.github.io/<repo>/`).

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
* SDL2_ttf-2.0.15
* SDL2_mixer-2.x (MP3 support)

Steps (CLion + CMake):
1. Download the SDL2 development libraries and copy the `lib` and `include`
   directories into the project root.
2. Do the same for the rest of the `SDL2_x` libraries.
3. Open the project in CLion and build with the built-in CMake configuration.
   The dispatcher picks up `build-windows.cmake` automatically when `WIN32` is
   set.
4. Copy the `SDL2_x` DLLs next to `BloonsTD.exe` so it can run.

## How to compile on macOS
Install dependencies via Homebrew (Apple Silicon `/opt/homebrew` and Intel
`/usr/local` are both auto-detected):
```bash
brew install cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer
```
SDL2_gfx and Boost are no longer required — circle primitives are inlined in
`RenderSystem.cpp` and `std::filesystem` replaced `boost::filesystem`. Both
were dropped to keep iOS portable.

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

## How to build for iOS / iPad
The iOS build vendors SDL2 + extensions as **static libraries** linked into a
single `.app` bundle. App Store rules forbid loose dylibs the app didn't sign
itself, and CMake's iOS support cleanly drives an Xcode project with these
libraries and the game sources.

### One-time setup — clone the SDL2 sources
```bash
mkdir -p third_party && cd third_party
git clone --depth 1 --branch release-2.30.10 https://github.com/libsdl-org/SDL.git
git clone --depth 1 --branch release-2.8.4    https://github.com/libsdl-org/SDL_image.git
git clone --depth 1 --branch release-2.22.0   https://github.com/libsdl-org/SDL_ttf.git
git clone --depth 1 --branch release-2.8.0    https://github.com/libsdl-org/SDL_mixer.git
cd ..
```
The four trees are gitignored locally; CI doesn't need them.

### Configure & build for the iOS Simulator (Apple Silicon Mac → arm64-sim)
```bash
cmake -G Xcode -B build-ios \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_SYSROOT=iphonesimulator \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=13.0
cmake --build build-ios --config Debug -- -sdk iphonesimulator
```
This produces `build-ios/Debug-iphonesimulator/BloonsTD.app` containing the
binary, all static SDL2 libs linked in, and the entire `assets/` tree under
`Resources/assets/`.

### Run in the Simulator
```bash
# pick a destination — list with `xcrun simctl list devices available`
DEVICE="iPad Pro (12.9-inch) (6th generation)"
xcrun simctl boot "$DEVICE" 2>/dev/null
open -a Simulator
xcrun simctl install booted build-ios/Debug-iphonesimulator/BloonsTD.app
xcrun simctl launch --console booted com.idmozes.BloonsTD
```

## How to build for the Web (WebAssembly)
The Web target produces a single static site (HTML/JS/WASM/data) you can host
anywhere. CI publishes it to GitHub Pages automatically; locally:

```bash
brew install emscripten                   # one-time, ~1GB

emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web -j

# Serve over HTTP — opening the .html file directly via file:// fails because
# Emscripten fetches the .wasm/.data over XHR.
(cd build-web && python3 -m http.server 8000)
# then open http://localhost:8000/BloonsTD.html
```

No SDL2 clones needed — `emsdk` ships SDL2 + SDL2_image + SDL2_ttf +
SDL2_mixer as ports, enabled via `-sUSE_SDL=2 -sUSE_SDL_IMAGE=2 …` in
`build-emscripten.cmake`. The `assets/` tree is packed into the `.data` file
at virtual path `/assets/` via `--preload-file`; `Assets.h` reads from
there under `__EMSCRIPTEN__`.

The bundle is ~54 MB total (mostly PNG and MP3 assets), downloaded once and
cached by the browser.

### iOS-specific source notes
| File | Why |
| --- | --- |
| `src/Assets.h` | Thin helper. On iOS `assetPath()` prepends `SDL_GetBasePath()` (the bundle's `Resources/`); on desktop it prepends `../assets/` (relative to `build/`). Lets the same source compile against both layouts. |
| `src/systems/RenderSystem.cpp` | Range circles drawn via inlined midpoint/scanline routines instead of `SDL2_gfx` so the iOS build doesn't need an extra library. |
| `src/Game.cpp` | `boost::filesystem` → `std::filesystem` (C++17). iOS toolchain's libc++ ships `<filesystem>`; Boost would be a heavyweight extra dependency. |

[`this`]: <https://stackoverflow.com/questions/36519453/setup-boost-in-clion>
[`releases`]: <https://github.com/idomoz/BloonsTD/releases>
[`SDL2`]: <https://www.libsdl.org/>
[`SDL2_mixer`]: <https://github.com/libsdl-org/SDL_mixer>
