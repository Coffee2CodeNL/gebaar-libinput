cmake_minimum_required(VERSION 3.13)
project(sxgd)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_executable(sxgd main.cpp)

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_search_module(LIBINPUT REQUIRED libinput)
endif ()

find_package(udev)
find_package(spdlog)

target_link_libraries(sxgd ${LIBINPUT_LIBRARIES} ${UDEV_LIBRARIES})
target_include_directories(sxgd PUBLIC ${LIBINPUT_INCLUDE_DIRS} ${UDEV_INCLUDE_DIRS})
target_compile_options(sxgd PUBLIC ${LIBINPUT_CFLAGS_OTHER} ${UDEV_CFLAGS_OTHER})