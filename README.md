# QPlotMarker

<p align="center">
  <img src="screenshots/logo.png" alt="Баннер проекта"/>
</p>

**QPlotMarker** – an interactive marker for **QChart** (Qt). Supports color and
orientation (vertical/horizontal) changes, snaps to data points, and displays
intersection coordinates.

## 🛠 Technology used:

[![C++](https://img.shields.io/badge/C++-00599C?logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![Qt](https://img.shields.io/badge/Qt-41CD52?logo=qt&logoColor=white)](https://qt.io)
[![QtSvg](https://img.shields.io/badge/QtSvg-2A9FD8?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsPSJ3aGl0ZSIgZD0iTTUgM2gxNHYxOEg1em0xMiAxNlY3SDd2MTJ6TTkgOWg2djZIOXoiLz48L3N2Zz4=)](https://doc.qt.io/qt-6/qtsvg-index.html)
[![QtCharts](https://img.shields.io/badge/QtCharts-8A2BE2?logo=qt&logoColor=white)](https://doc.qt.io/qt-6/qtcharts-index.html)
[![QDoc](https://img.shields.io/badge/Qt_Doc-41CD52?logo=qt&logoColor=white&labelColor=lightgray)](https://doc.qt.io/qtdoc/)
[![Qt Help](https://img.shields.io/badge/Qt_Help-3498DB?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI4Ij48cGF0aCBmaWxsPSJ3aGl0ZSIgZD0iTTMgMjFWM3MwLTEuMS45LTJoMTZjMS4xIDAgMiAuOSAyIDJ2MThjMCAxLjEtLjkgMi0yIDJINGMtMS4xIDAtMi0uOS0yLTJ6bTUuNS0xMGgzdjNoLTN2LTN6bTAgNC41aDN2M2gtM3YtM3ptNC41LTQuNWgzdjNoLTN2LTN6bTAgNC41aDN2M2gtM3YtM3pNNiAxNmgxMnYySDZ2LTJ6Ii8+PC9zdmc+)](https://doc.qt.io/qt-6/qthelp-framework.html)

[![CLion](https://img.shields.io/badge/CLion-F5842C?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIxMDUiIGhlaWdodD0iMTA1IiB2aWV3Qm94PSIwIDAgMjQgMjQiPjxwYXRoIGZpbGw9IiMwMDAiIGQ9Ik0xMiAyQzYuNSAyIDIgNi41IDIgMTJzNC41IDEwIDEwIDEwIDEwLTQuNSAxMC0xMFMxNy41IDIgMTIgMnptMy4zIDE0LjljLS4yLjEtLjUuMi0xIC4yLTEuMiAwLTIuMi0uOS0yLjItMi44di0yLjFjMC0xLjkgMS0yLjggMi4yLTIuOC41IDAgLjguMSAxIC4ydi0uN2MwLS40LS4zLS43LS43LS43aC0xYy0uNCAwLS43LjMtLjcuN3Y3LjRjMCAuNC4zLjcuNy43aDFjLjQgMCAuNy0uMy43LS43di0uN3oiLz48cGF0aCBmaWxsPSIjZjU4NDJjIiBkPSJNMTIuOSA4LjJjLTEuMiAwLTIuMi45LTIuMiAyLjh2Mi4xYzAgMS45IDEgMi44IDIuMiAyLjguNSAwIC44LS4xIDEtLjJ2LjZjMCAuNC4zLjcuNy43aDFjLjQgMCAuNy0uMy43LS43VjguOWMwLS40LS4zLS43LS43LS43aC0xYy0uNCAwLS43LjMtLjcuN3YuN3oiLz48L3N2Zz4=&logoColor=000000&labelColor=FFFFFF)](https://jetbrains.com/clion)
[![CMD](https://img.shields.io/badge/CMD-4D4D4D?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsPSJ3aGl0ZSIgZD0iTTIgNGEyIDIgMCAwIDEtMi0ydjE0YTIgMiAwIDAgMSAyIDJoMjBhMiAyIDAgMCAxIDIgMnYtMTRhMiAyIDAgMCAxLTItMnptMiAyaDJ2Mkg0em0wIDRoMnYySDR6bTAgNGgydjJINHptNCAwaDJ2Mkg4em00IDBoMnYyaC0yem0wLTRoMnYyaC0yem00IDBoMnYyaC0yem0wLTRoMnYyaC0yem00IDBoMnYyaC0yem0wLTRoMnYyaC0yem0tOC00aDJ2Mkg4em00IDBoMnYyaC0yem00IDBoMnYyaC0yem0tMTIgNGgydjJINHptNCAwaDJ2Mkg4eiIvPjwvc3ZnPg==)](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/windows-commands)
[![Git](https://img.shields.io/badge/Git-F05032?logo=git&logoColor=white)](https://git-scm.com/)
[![GitHub](https://img.shields.io/badge/GitHub-181717?logo=github&logoColor=white)](https://github.com/)

# ⬇️ Install

You must have
installed [project dependencies](https://github.com/Mark194/qplotmarker#dependencies)

```bash
# Clone repository

git clone https://github.com/Mark194/qplotmarker

# Go to directory

cd qplotmarker

# Create and go to the build directory

mkdir build

cd build

# Project configuration via CMake

cmake ..

# If you need to specify a specific generator, use

cmake -G "Visual Studio 17 2022" ..

# If you want to configure the library as static, add the option

-DBUILD_SHARED_LIBS=OFF

# Build project

cmake --build .

# Installing the library into the system

cmake --install .

# By default, installation goes to /usr/local/ on Linux/macOS or C:\ProgramFiles (x86)\ on Windows. To change the path, specify

-DCMAKE_INSTALL_PREFIX=/your/path 
```

[Library releases]: https://github.com/Mark194/qplotmarker/releases

<!--Integration-->

# 🧩 Integration

## 🔧 QMake Integration

```makefile

# Add include path

INCLUDEPATH += $$PWD/thirdparty/qplotmarker/include

# Link static library

LIBS += -L$$PWD/thirdparty/qplotmarker/lib -lqplotmarker

# OR compile sources directly (if needed)

SOURCES += $$PWD/thirdparty/qplotmarker/src/*.cpp
HEADERS += $$PWD/thirdparty/qplotmarker/include/*.h

```

## 🧱 CMake Integration

### Method 1: Using find_package (Recommended if installed system-wide)

```cmake

find_package(qplotmarker REQUIRED)

target_link_libraries(your_target PRIVATE qplotmarker::qplotmarker)
```

### Method 2: Manual configuration (For custom locations)

```cmake
# For header files

target_include_directories(your_target PRIVATE
        path/to/qplotmarker/include
)

# For static library
target_link_libraries(your_target PRIVATE
        path/to/qplotmarker/lib/libqplotmarker.a
)

# Or for shared library
target_link_libraries(your_target PRIVATE
        path/to/qplotmarker/lib/libqplotmarker.so)
```

### Method 3: Using FetchContent (For direct GitHub integration)

```cmake
include(FetchContent)
FetchContent_Declare(
        qplotmarker
        GIT_REPOSITORY https://github.com/Mark194/qplotmarker.git
        GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(qplotmarker)
target_link_libraries(your_target PRIVATE qplotmarker)
```

<!--Support-->

# 🆘 Support

If you have any difficulties or questions about using the package, create a
[discussion](https://github.com/Mark194/qplotmarker/issues/new/choose) in this
repository or write to email


<!--dependencies-->

# 📦 Dependencies

- **Qt Core**: Version **5.15.2+** or **6.4.2+**
- **Required Modules**:
    - `QtCharts` (for plotting functionality)
    - `QtSvg` (for SVG support)
- **Compiler**:
    - Qt5: GCC 7+, Clang 6+, MSVC 2017 (v19.16+)
    - Qt6: GCC 9+, Clang 10+, MSVC 2019 (v19.28+)
- **Additional Notes**:
    - On Windows: MSVC is recommended (MinGW may have issues with QtCharts)
    - On macOS: Xcode 12+ required for Qt6
    - On Linux: Install `libgl1-mesa-dev` and other development tools

Found compatibility issues? Please report
in [Issues](https://github.com/Mark194/qplotmarker/issues).


<!--examples-->

# 💻 Examples

Ready-to-use code examples are available in
the [examples](https://github.com/Mark194/qplotmarker/tree/main/examples)
folder. These demonstrate:

- Basic usage of plotting functionality
- Advanced marker customization
- Integration with Qt widgets

## 👁️ Visual Examples

| Miniature                                                                                                                                            | Description                                                                  |
|------------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------|
| <details><summary><img src="screenshots/base_example.png" width="300"></summary>![Full size](screenshots/base_example.png)</details>                 | **Base Example**<br>                                                         |
| <details><summary><img src="screenshots/inverted_markers.png" width="300"></summary>![Full size](screenshots/inverted_markers.png)</details>         | **Inverted Markers**<br>Invert horizontal and vertical marker                |
| <details><summary><img src="screenshots/qplotmarker_distance.png" width="300"></summary>![Full size](screenshots/qplotmarker_distance.png)</details> | **PlotMarker Distance**<br>Coord difference between markers                  |
| <details><summary><img src="screenshots/show_coord_marker.png" width="300"></summary>![Full size](screenshots/show_coord_marker.png)</details>       | **Show Coord Marker**<br>Displays the coordinates of the intersection points |
| <details><summary><img src="screenshots/grouping_markers.png" width="300"></summary>![Full size](screenshots/grouping_markers.png)</details>         | **Grouping Markers**<br>Grouping markers for synchronous movement            |

<!--special thanks-->

# 🙏 Special Thanks

We would like to express our special gratitude to *
*[ShonZelno](https://github.com/ShonZelno)** for their valuable contributions to
this project, including:

- Active participation in development and testing
- Suggesting innovative ideas and improvements
- Helping to identify and fix critical issues

Your involvement made this library significantly better! 
