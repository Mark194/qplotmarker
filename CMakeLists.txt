cmake_minimum_required(VERSION 3.16...3.27)
project(qplotmarker VERSION 1.0.0 LANGUAGES CXX)

# Настройки в стиле Qt
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматизация Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поиск Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Charts)


if(QT_VERSION_MAJOR EQUAL 5)
    find_package(Qt5 REQUIRED COMPONENTS Svg)
elseif(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS SvgWidgets)
endif()


# Опции проекта
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

# Подпроекты
add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Установка
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
    DIRECTORY include/QPlotMarker
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/QPlotMarkerConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/QPlotMarkerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QPlotMarker
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/QPlotMarkerConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/QPlotMarkerConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/QPlotMarkerConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QPlotMarker
)

export(EXPORT QPlotMarkerTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/QPlotMarkerTargets.cmake
    NAMESPACE QPlotMarker::
)
