cmake_minimum_required(VERSION 3.16)

project(Siili_Task VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)

set(SOURCES
  main.cpp
  gaugecontrol.cpp
)

set(HEADERS
  gaugecontrol.h
)
qt_add_executable(appSiili_Task
  ${SOURCES}
  ${HEADERS}
)

qt_add_qml_module(appSiili_Task
    URI Siili_Task
    VERSION 1.0
    QML_FILES
      main.qml
      SpeedGauge.qml
      Histogram.qml
    RESOURCES
      assets/svgBackgroundCircle.png
      assets/Speed\ texture.png
      assets/Needle.png
      assets/TopCircle.png
)

set_target_properties(appSiili_Task PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appSiili_Task
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appSiili_Task
    PRIVATE Qt6::Quick)
