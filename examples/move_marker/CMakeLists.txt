cmake_minimum_required(VERSION 3.16)

project(move_marker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Charts Widgets Svg SvgWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts Widgets Svg SvgWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.hpp
)

# Проверяем наличие QPLOTMARKER_DIR
if(NOT QPLOTMARKER_DIR)
    message(FATAL_ERROR "QPLOTMARKER_DIR not set! Specify the path to QPlotMarker.")
endif()

# Добавляем путь к заголовкам и библиотеке
set(QPLOTMARKER_INCLUDE_DIR "${QPLOTMARKER_DIR}/include/QPlotMarker")
set(QPLOTMARKER_LIB_DIR "${QPLOTMARKER_DIR}/lib")

# Проверяем, существует ли библиотека
find_library(QPLOTMARKER_LIB
    NAMES
      qplotmarker
      QPlotMarker
      libqplotmarker.a
    HINTS ${QPLOTMARKER_LIB_DIR}
    REQUIRED
)

message(STATUS "Found QPlotMarker library: ${QPLOTMARKER_LIB_DIR}")

include_directories(${QPLOTMARKER_INCLUDE_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(move_marker
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET move_marker APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(move_marker SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(move_marker
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(
  move_marker
  PRIVATE
  ${QPLOTMARKER_LIB}
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Charts
  Qt${QT_VERSION_MAJOR}::Svg
  Qt${QT_VERSION_MAJOR}::SvgWidgets
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.move_marker)
endif()
set_target_properties(move_marker PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS move_marker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(move_marker)
endif()
