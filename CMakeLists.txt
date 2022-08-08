cmake_minimum_required(VERSION 3.14)

project(GMUM LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core OpenGLWidgets SerialPort)

add_executable(GMUM
  resources.qrc
  main.cpp
  mainwindow.h mainwindow.cpp
  globject.h globject.cpp
  glwidget.h glwidget.cpp
  reflectosphere.h reflectosphere.cpp
  lightbeam.h lightbeam.cpp
)
target_link_libraries(GMUM Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::OpenGLWidgets Qt${QT_VERSION_MAJOR}::SerialPort)
