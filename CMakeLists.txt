cmake_minimum_required(VERSION 3.14)
project(BloonsTD CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources are shared across platforms; toolchain/dependency wiring is not.
# Pick the right config based on the host platform / target SDK.
# CMAKE_SYSTEM_NAME is set to "iOS" by the iOS toolchain or
# `-DCMAKE_SYSTEM_NAME=iOS` on the cmake command line.
# EMSCRIPTEN is set when configuring via `emcmake cmake`.
if(EMSCRIPTEN)
    include(${CMAKE_CURRENT_LIST_DIR}/build-emscripten.cmake)
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    include(${CMAKE_CURRENT_LIST_DIR}/build-ios.cmake)
elseif(APPLE)
    include(${CMAKE_CURRENT_LIST_DIR}/build-macos.cmake)
elseif(WIN32)
    include(${CMAKE_CURRENT_LIST_DIR}/build-windows.cmake)
else()
    message(FATAL_ERROR
            "BloonsTD has been ported to Windows (MinGW), macOS, iOS, and "
            "WebAssembly (Emscripten). See README.md.")
endif()

# Shared source list.
set(BLOONSTD_SOURCES
        src/SDL_FontCache.c src/SDL_FontCache.h
        src/main.cpp
        src/Game.cpp src/Game.h
        src/Entity.cpp src/Entity.h
        src/Component.h src/System.cpp src/System.h
        src/GameData.cpp src/GameData.h
        src/Audio.cpp src/Audio.h
        src/Physics.cpp src/Physics.h
        src/Settings.h
        src/components/Visibility.cpp src/components/Visibility.h
        src/components/Velocity.cpp src/components/Velocity.h
        src/components/Sequence.cpp src/components/Sequence.h
        src/components/AttackSpeed.cpp src/components/AttackSpeed.h
        src/components/Position.h src/components/PathIndex.h
        src/components/Action.h src/components/Draggable.h
        src/components/MoveEntityEvent.h src/components/RangeShadow.h
        src/components/IntegerComponents.h src/components/FloatComponents.h
        src/components/PoppedBloons.h src/components/FlagComponents.h
        src/components/DamageEvent.h src/components/Regrow.h
        src/components/Goo.h src/components/Upgrades.h
        src/systems/RenderSystem.cpp src/systems/RenderSystem.h
        src/systems/MovementSystem.cpp src/systems/MovementSystem.h
        src/systems/BloonsSpawnSystem.cpp src/systems/BloonsSpawnSystem.h
        src/systems/EventSystem.cpp src/systems/EventSystem.h
        src/systems/DraggingSystem.cpp src/systems/DraggingSystem.h
        src/systems/RemoveEntitiesSystem.cpp src/systems/RemoveEntitiesSystem.h
        src/systems/CollisionSystem.cpp src/systems/CollisionSystem.h
        src/systems/ShotsSpawnSystem.cpp src/systems/ShotsSpawnSystem.h
        src/systems/DamageSystem.cpp src/systems/DamageSystem.h
        src/systems/MenuSystem.cpp src/systems/MenuSystem.h
        src/systems/LoadLevelSystem.cpp src/systems/LoadLevelSystem.h
        src/systems/PopEffectSystem.cpp src/systems/PopEffectSystem.h)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # MACOSX_BUNDLE turns the executable into a .app — required on iOS.
    add_executable(BloonsTD MACOSX_BUNDLE ${BLOONSTD_SOURCES})
else()
    add_executable(BloonsTD ${BLOONSTD_SOURCES})
endif()
bloonstd_configure_target(BloonsTD)
