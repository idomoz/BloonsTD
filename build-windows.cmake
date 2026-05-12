# Windows / MinGW configuration. Supports two layouts:
#   1) Project-local "lib/" and "include/" populated with the SDL2 dev libs
#      (the legacy CLion flow described in README.md).
#   2) System-installed packages on MSYS2 / MinGW64 (used by CI).

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Project-local layout, if present.
if(EXISTS "${CMAKE_SOURCE_DIR}/include")
    include_directories(${CMAKE_SOURCE_DIR}/include)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/lib")
    link_directories(${CMAKE_SOURCE_DIR}/lib)
endif()

# Pick up system-installed SDL2 headers (MSYS2 puts them in
# /mingw64/include/SDL2). Harmless when the project-local layout is in use.
find_path(SDL2_SYS_INCLUDE_DIR SDL.h PATH_SUFFIXES SDL2)
if(SDL2_SYS_INCLUDE_DIR)
    include_directories(${SDL2_SYS_INCLUDE_DIR})
endif()

find_package(Boost REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

link_libraries(mingw32 SDL2main SDL2 SDL2_image SDL2_gfx SDL2_ttf)

function(bloonstd_configure_target tgt)
    target_link_libraries(${tgt} ${Boost_LIBRARIES})
endfunction()
