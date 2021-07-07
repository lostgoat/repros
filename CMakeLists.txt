cmake_minimum_required(VERSION 3.20)

project(
  repros
  VERSION 1.0
  LANGUAGES CXX)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  set(CMAKE_CXX_EXTENSIONS OFF)
  set (CMAKE_CXX_STANDARD 20)

endif()

# System libs
find_package(PkgConfig REQUIRED) 
pkg_check_modules(glib-2.0 REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(gio-2.0 REQUIRED IMPORTED_TARGET gio-2.0)

# Output
add_executable(repros main.cpp)

# Link
target_link_libraries(repros PRIVATE PkgConfig::glib-2.0 PkgConfig::gio-2.0)
