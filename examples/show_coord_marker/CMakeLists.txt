cmake_minimum_required(VERSION 3.16)

project(show_coord_marker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Charts Widgets Svg SvgWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts Widgets Svg SvgWidgets)

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

set(PROJECT_SOURCES
        main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(show_coord_marker
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        chart_event_filter.hpp chart_event_filter.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET show_coord_marker APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(show_coord_marker SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(show_coord_marker
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(show_coord_marker
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
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.show_coord_marker)
endif()
set_target_properties(show_coord_marker PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS show_coord_marker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(show_coord_marker)
endif()
