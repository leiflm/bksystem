cmake_minimum_required(VERSION 2.6)
project(bksystem C)
set(VMAJ 0)
set(VMIN 1)
set(VMIC 0)
set(PACKAGE_VERSION ${VMAJ}.${VMIN}.${VMIC})

set(PACKAGE ${CMAKE_PROJECT_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Eina REQUIRED)
find_package(Ecore REQUIRED)
find_package(Efreet REQUIRED)
find_package(Evas REQUIRED)
find_package(Threads REQUIRED)
find_package(Elementary REQUIRED)
find_package(Sqlite3 REQUIRED)

ADD_DEFINITIONS(-DPACKAGE_BIN_DIR="${CMAKE_INSTALL_PREFIX}/bin"
    -DPACKAGE_DATA_DIR="${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROEJECT_NAME}")

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(doc)

set(CPACK_RPM_PACKAGE_SUMMARY "An EFL based kiosk point of sale application")
set(CPACK_RPM_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_RPM_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_RPM_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_LICENSE BSD)
set(CPACK_RPM_PACKAGE_GROUP "Productivity/Office/Finance")
set(CPACK_RPM_PACKAGE_DESCRIPTION
	"BKSystem is a simple and lightweight kiosk-style point of sale application based on the Enlightenment Foundation Libraries (http://www.enlightenment.org)")
set(CPACK_RPM_PACKAGE_REQUIRES libelementary >= 1.7)
