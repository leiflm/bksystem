cmake_minimum_required(VERSION 2.6)
project(bksystem)
set(VMAJ 0)
set(VMIN 1)
set(VMIC 0)
set(PACKAGE_VERSION ${VMAJ}.${VMIN}.${VMIC})

set(PACKAGE ${CMAKE_PROEJECT_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Eina REQUIRED)
find_package(Ecore REQUIRED)
find_package(Evas REQUIRED)
find_package(Threads REQUIRED)
find_package(Elementary REQUIRED)
find_package(Sqlite3 REQUIRED)

ADD_DEFINITIONS(-DPACKAGE_BIN_DIR="${CMAKE_INSTALL_PREFIX}/bin"
    -DPACKAGE_DATA_DIR="${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROEJECT_NAME}")

add_subdirectory(src)
