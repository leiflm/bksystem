# - Try to find ethumb
# Once done this will define
#  ETHUMB_FOUND - System has ethumb
#  ETHUMB_INCLUDE_DIRS - The ethumb include directories
#  ETHUMB_LIBRARIES - The libraries needed to use ethumb
#  ETHUMB_DEFINITIONS - Compiler switches required for using ethumb

find_package(PkgConfig)
if ("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER "2.8.1")
   # "QUIET" was introduced in 2.8.2
   set(_QUIET QUIET)
endif ()
pkg_check_modules(PC_LIBETHUMB ${_QUIET} ethumb)
set(ETHUMB_DEFINITIONS ${PC_LIBETHUMB_CFLAGS_OTHER})

find_path(ETHUMB_INCLUDE_DIR Ethumb.h
          HINTS ${PC_LIBETHUMB_INCLUDEDIR} ${PC_LIBETHUMB_INCLUDE_DIRS}
          PATH_SUFFIXES ethumb-1 )

find_library(ETHUMB_LIBRARY NAMES ethumb
             HINTS ${PC_LIBETHUMB_LIBDIR} ${PC_LIBETHUMB_LIBRARY_DIRS} )

set(ETHUMB_LIBRARIES ${ETHUMB_LIBRARY} )
set(ETHUMB_INCLUDE_DIRS ${ETHUMB_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ETHUMB_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(ethumb  DEFAULT_MSG
                                  ETHUMB_LIBRARY ETHUMB_INCLUDE_DIR)

mark_as_advanced(ETHUMB_INCLUDE_DIR ETHUMB_LIBRARY )
