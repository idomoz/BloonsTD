# BloonsTD
* A remake of Ninja Kiwi's BTD5 + BTD6 games.
* Written in C++ with the [`SDL2`] library for graphic rendering.
## Preview
<img src="/assets/preview.gif?raw=true" width="600px">

## How to play:
* Go to the [`releases`] page and download `BloonsTD.7z`
* Extract `BloonsTD.7z` and run `BloonsTD/bin/BloonsTD.exe`

## How to compile with MinGW for windows:
This project depends on:
* SDL2-2.0.9
* SDL2_image-2.0.5
* SDL2_gfx-1.0.4
* SDL2_ttf-2.0.15
* Boost-1.17.0

In order to compile with CLion (and CMake):
1. Download the SDL2 development libraries and copy the `lib` and `include` directories to the main project folder.
2. Do the same for the rest of SDL2_x libraries.
3. You will need to compile SDL2_gfx yourself as it doesn't have development libraries.
4. For Boost, follow [`this`] guide on how to compile and add it to CLion.
5. Open the project with CLion and compile using the built-in CMake configuration.
6. Copy SDL2_x's dlls to the folder containing the compiled exe in order to run the game.

[`this`]: <https://stackoverflow.com/questions/36519453/setup-boost-in-clion>
[`releases`]: <https://github.com/idomoz/BloonsTD/releases>
[`SDL2`]: <https://www.libsdl.org/>
