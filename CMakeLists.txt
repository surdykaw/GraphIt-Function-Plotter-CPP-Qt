cmake_minimum_required(VERSION 3.16)
project(GraphIt VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

include(GNUInstallDirs)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_add_executable(GraphIt)

target_sources(GraphIt PRIVATE
    app.cpp
    src/buttons.cpp
    src/function.cpp
    src/graphlayer.cpp
    src/mainwin.cpp
)

target_sources(GraphIt PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
        include/buttons.h
        include/function.h
        include/graphlayer.h
        include/mainwin.h
)

target_include_directories(GraphIt PRIVATE
    external
    include
)

target_link_libraries(GraphIt PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# MSVC fix for large template objects (exprtk)
if (MSVC)
    target_compile_options(GraphIt PRIVATE /bigobj)
endif()

qt_add_resources(GraphIt resources
    PREFIX "/"
    FILES
        resources/arrowDown.png
        resources/arrowUp.png
        resources/mainAxisArrRight.png
        resources/mainAxisArrUp.png
        resources/minusRounded.png
        resources/minusRounded_40.png
        resources/plusCircle.png
        resources/plusCircle_40.png
)

install(TARGETS GraphIt
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
