

# Подключаем необходимые модули Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Charts Svg)

if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Svg SvgWidgets)
endif()


set(QPLOTMARKER_SOURCES
    qplotmarker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/QPlotMarker/qplotmarker.hpp
    entity/movable_button.hpp entity/movable_button.cpp
    entity/graphics_coord_item.hpp entity/graphics_coord_item.cpp
    effects/fast_colorize_effect.hpp effects/fast_colorize_effect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/QPlotMarker/qplotmarker_group.hpp
    qplotmarker_group.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/QPlotMarker/qplotmarker_distance.hpp
    qplot_marker_p.hpp qplot_marker_p.cpp
    utility/plot_geometry_utils.hpp utility/plot_geometry_utils.cpp
    ../resources/marker_icons.qrc
    qplotmarker_distance.cpp
    qplotmarker_distance_p.hpp qplotmarker_distance_p.cpp
    effects/glow_effect.cpp effects/glow_effect.hpp
)


if(BUILD_SHARED_LIBS)
   add_library(qplotmarker SHARED ${QPLOTMARKER_SOURCES})
else()
   add_library(qplotmarker STATIC ${QPLOTMARKER_SOURCES})
endif()



target_compile_definitions(qplotmarker
    PRIVATE
    MY_LIBRARY_EXPORT
    QT_FORCE_ASSERTS
)

# Остальные настройки остаются без изменений
target_include_directories(qplotmarker
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Указываем публичные заголовочные файлы
set_target_properties(qplotmarker PROPERTIES
    PUBLIC_HEADER
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/QPlotMarker/qplotmarker.hpp;${CMAKE_CURRENT_SOURCE_DIR}/../include/QPlotMarker/qplotmarker_group.hpp;${CMAKE_CURRENT_SOURCE_DIR}/../include/QPlotMarker/qplotmarker_global.hpp;${CMAKE_CURRENT_SOURCE_DIR}/../include/QPlotMarker/QPlotMarker;${CMAKE_CURRENT_SOURCE_DIR}/../include/QPlotMarker/qplotmarker_distance.hpp;${CMAKE_CURRENT_SOURCE_DIR}/../include/QPlotMarker/QPlotMarkerDistance;"
)


if(QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(qplotmarker
        PUBLIC
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Charts
            Qt${QT_VERSION_MAJOR}::Svg
    )
elseif(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(qplotmarker
        PUBLIC
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Charts
            Qt${QT_VERSION_MAJOR}::Svg
            Qt${QT_VERSION_MAJOR}::SvgWidgets
    )
endif()

set_target_properties(qplotmarker PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    DEFINE_SYMBOL QPLOTMARKER_LIBRARY
)

install(TARGETS qplotmarker
    EXPORT QPlotMarkerTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QPlotMarker
)
