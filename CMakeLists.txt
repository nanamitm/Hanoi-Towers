cmake_minimum_required(VERSION 3.16)

project(HanoiTowersSlideshow VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if(COMMAND qt_standard_project_setup)
    qt_standard_project_setup()
else()
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
endif()

set(APP_RESOURCE_FILES resources/resources.qrc)
if(WIN32)
    list(APPEND APP_RESOURCE_FILES resources/app.rc)
endif()

qt_add_executable(HanoiTowersSlideshow
    ${APP_RESOURCE_FILES}
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

install(TARGETS HanoiTowersSlideshow
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(UNIX AND NOT APPLE)
    install(FILES packaging/io.github.nanamitm.HanoiTowersSlideshow.desktop
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
    )
    install(FILES packaging/hanoi-towers.png
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps
    )
endif()
