cmake_minimum_required(VERSION 3.12.1)
project(gebaar)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_executable(gebaard
        src/main.cpp
        src/io/input.cpp
        src/io/input.h
        src/config/config.cpp
        src/config/config.h
        src/daemonizer.cpp
        src/daemonizer.h
        src/util.cpp
        src/util.h)

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_search_module(LIBINPUT REQUIRED libinput)
endif ()

find_package(udev)

target_link_libraries(gebaard ${LIBINPUT_LIBRARIES} ${UDEV_LIBRARIES} stdc++fs)
target_include_directories(gebaard PUBLIC ${LIBINPUT_INCLUDE_DIRS} ${UDEV_INCLUDE_DIRS} libs/cxxopts/include libs/cpptoml/include)
target_compile_options(gebaard PUBLIC ${LIBINPUT_CFLAGS_OTHER} ${UDEV_CFLAGS_OTHER})

install(TARGETS gebaard DESTINATION bin)