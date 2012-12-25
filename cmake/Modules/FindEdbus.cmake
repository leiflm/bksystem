# - Try to find edbus
# Once done this will define
#  EDBUS_FOUND - System has edbus
#  EDBUS_INCLUDE_DIRS - The edbus include directories
#  EDBUS_LIBRARIES - The libraries needed to use edbus
#  EDBUS_DEFINITIONS - Compiler switches required for using edbus

find_package(PkgConfig)
if ("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER "2.8.1")
   # "QUIET" was introduced in 2.8.2
   set(_QUIET QUIET)
endif ()
pkg_check_modules(PC_LIBEDBUS ${_QUIET} edbus)
set(EDBUS_DEFINITIONS ${PC_LIBEDBUS_CFLAGS_OTHER})

find_path(EDBUS_INCLUDE_DIR E_DBus.h
          HINTS ${PC_LIBEDBUS_INCLUDEDIR} ${PC_LIBEDBUS_INCLUDE_DIRS}
          PATH_SUFFIXES e_dbus-1 )

find_library(EDBUS_LIBRARY NAMES edbus
             HINTS ${PC_LIBEDBUS_LIBDIR} ${PC_LIBEDBUS_LIBRARY_DIRS} )

set(EDBUS_LIBRARIES ${EDBUS_LIBRARY} )
set(EDBUS_INCLUDE_DIRS ${EDBUS_INCLUDE_DIR} "/usr/include/dbus-1.0" "/usr/lib/x86_64-linux-gnu/dbus-1.0/include" )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set EDBUS_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(edbus  DEFAULT_MSG
                                  EDBUS_LIBRARY EDBUS_INCLUDE_DIR)

mark_as_advanced(EDBUS_INCLUDE_DIR EDBUS_LIBRARY )
