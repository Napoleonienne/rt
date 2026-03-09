cmake_minimum_required(VERSION 3.28)

project(rt LANGUAGES CXX)

# ── Compiler requirements ────────────────────────────────────────────────────
# C++23 is required for import std; and std::views::cartesian_product.
# Supported toolchains:
#   • Clang 17+  with libc++  (pass -DCMAKE_CXX_FLAGS="-stdlib=libc++" if needed)
#   • GCC 15+    with libstdc++
set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# ── import std; support ──────────────────────────────────────────────────────
# CMake 3.30+ exposes this flag without the EXPERIMENTAL guard; keep the
# experimental UUID for 3.28/3.29 compatibility.
if(CMAKE_VERSION VERSION_LESS "3.30")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
        "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
endif()
set(CMAKE_CXX_MODULE_STD 1)

# ── Optional: prefer libc++ when using Clang ────────────────────────────────
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-stdlib=libc++)
    add_link_options(-stdlib=libc++)
endif()

# ── Module interface units (.cppm) ───────────────────────────────────────────
# Dependency order (no cycles):
#   vector  ← (no project deps)
#   color   ← vector
#   ray     ← vector
#   utility ← vector
#   perlinnoise ← vector
#   main    ← vector, color, ray, utility
add_library(rt_modules)
target_sources(rt_modules
    PUBLIC
        FILE_SET CXX_MODULES FILES
            modules/vector.cppm
            modules/color.cppm
            modules/ray.cppm
            modules/utility.cppm
            modules/perlinnoise.cppm
)

# ── Main executable ──────────────────────────────────────────────────────────
add_executable(rt src/main.cpp)
target_link_libraries(rt PRIVATE rt_modules)
