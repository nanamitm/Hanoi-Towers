cmake_minimum_required(VERSION 3.16)

project(HanoiTowersSlideshow VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(HanoiTowersSlideshow
    resources/app.rc
    resources/resources.qrc
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/hanoimodel.cpp
    src/hanoimodel.h
    src/hanoisolver.cpp
    src/hanoisolver.h
    src/hanoiview.cpp
    src/hanoiview.h
)

target_link_libraries(HanoiTowersSlideshow PRIVATE Qt6::Widgets)

set_target_properties(HanoiTowersSlideshow PROPERTIES
    WIN32_EXECUTABLE TRUE
)
