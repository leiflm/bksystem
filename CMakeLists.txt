cmake_minimum_required(VERSION 2.6)
project(bksystem C)
set(VMAJ 0)
set(VMIN 1)
set(VMIC 3)

set(PACKAGE ${CMAKE_PROJECT_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Eina REQUIRED)
find_package(Ecore REQUIRED)
find_package(Ecore_File REQUIRED)
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

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An EFL based kiosk point of sale application")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_PACKAGE_VERSION ${VMAJ}.${VMIN}.${VMIC})
set(CPACK_RPM_PACKAGE_LICENSE BSD)
set(CPACK_RPM_PACKAGE_GROUP "Productivity/Office/Finance")
set(CPACK_RPM_PACKAGE_DESCRIPTION
	"BKSystem is a simple and lightweight kiosk-style point of sale application based on the Enlightenment Foundation Libraries (http://www.enlightenment.org)")
include(CPack)
