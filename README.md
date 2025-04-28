
# qplotmarker

<p align="center">
  <img src="https://sun9-24.userapi.com/impg/tjFWW8NfkvxUbL1uxSfCZKagMPkjK-wBvQx5Iw/P81u_iViUNE.jpg?size=1244x512&quality=95&sign=3ccf47b46789cc9a17c4c45edf76b440&type=album)" alt="Баннер проекта" width="729" height="300"/>
</p>

**QPlotMarker** – an interactive marker for **QChart** (Qt). Supports color and orientation (vertical/horizontal) changes, snaps to data points, and displays intersection coordinates.

<!--Установка-->
## Install
You must have installed [project dependencies](https://github.com/Mark194/qplotmarker#dependencies)

1. Clone repository 

```git clone https://github.com/Mark194/qplotmarker```

2. Go to directory

```cd qplotmarker```

3. Create and go to the build directory

```mkdir build```

```cd build```


4. Project configuration via CMake

```cmake ..```


If you need to specify a specific generator, use

```cmake -G "Visual Studio 17 2022" ..```

If you want to configure the library as static, add the option

```-DBUILD_SHARED_LIBS=OFF```

5. Build project

```cmake --build .```

6. Installing the library into the system

```cmake --install .```

By default, installation goes to /usr/local/ on Linux/macOS or C:\Program Files (x86)\ on Windows. To change the path, specify 

```-DCMAKE_INSTALL_PREFIX=/your/path ```


[Library releases]: https://github.com/Mark194/qplotmarker/releases


<!--Support-->
## Support
If you have any difficulties or questions about using the package, create a
[discussion](https://github.com/Mark194/qplotmarker/issues/new/choose) in this repository or write to email


<!--dependencies-->
## Dependencies

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

Found compatibility issues? Please report in [Issues](https://github.com/Mark194/qplotmarker/issues).


<!--examples-->
## Examples

Ready-to-use code examples are available in the [examples](https://github.com/Mark194/qplotmarker/tree/main/examples) folder. These demonstrate:
- Basic usage of plotting functionality
- Advanced marker customization
- Integration with Qt widgets

<!--special thanks-->
## Special Thanks

We would like to express our special gratitude to **[ShonZelno](https://github.com/ShonZelno)** for their valuable contributions to this project, including:
- Active participation in development and testing
- Suggesting innovative ideas and improvements
- Helping to identify and fix critical issues

Your involvement made this library significantly better! 
