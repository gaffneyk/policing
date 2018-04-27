cmake_minimum_required(VERSION 3.9)
project(haplodiplo)

set(CMAKE_CXX_STANDARD 11)

add_executable(haplodiplo main.cpp Bot.cpp Bot.h Controller.cpp Controller.h Token.cpp Token.h)

set(enki_DIR /Users/kevingaffney/Development/enki)
set(Qt5Core_DIR /usr/local/Cellar/qt/5.10.0_1/lib/cmake/Qt5Core)
set(Qt5Gui_DIR /usr/local/Cellar/qt/5.10.0_1/lib/cmake/Qt5Gui)
set(Qt5Widgets_DIR /usr/local/Cellar/qt/5.10.0_1/lib/cmake/Qt5Widgets)
set(Qt5OpenGL_DIR /usr/local/Cellar/qt/5.10.0_1/lib/cmake/Qt5OpenGL)

find_package(enki REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${enki_INCLUDE_DIR})

target_link_libraries(haplodiplo ${enki_VIEWER_LIBRARIES} ${enki_LIBRARY} ${OPENGL_LIBRARIES})