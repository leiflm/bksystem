# - Try to find ecore
# Once done this will define
#  ECORE_FOUND - System has ecore
#  ECORE_INCLUDE_DIRS - The ecore include directories
#  ECORE_LIBRARIES - The libraries needed to use ecore
#  ECORE_DEFINITIONS - Compiler switches required for using ecore

# Use FIND_PACKAGE( Ecore COMPONENTS ... ) to enable modules
IF( Ecore_FIND_COMPONENTS )
  FOREACH( component ${Ecore_FIND_COMPONENTS} )
    STRING( TOUPPER ${component} _COMPONENT )
    SET( ECORE_USE_${_COMPONENT} 1 )
  ENDFOREACH( component )
ENDIF( Ecore_FIND_COMPONENTS )

find_package(PkgConfig)
if ("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER "2.8.1")
   # "QUIET" was introduced in 2.8.2
   set(_QUIET QUIET)
endif ()
pkg_check_modules(PC_LIBECORE ${_QUIET} ecore)
set(ECORE_DEFINITIONS ${PC_LIBECORE_CFLAGS_OTHER})

find_path(ECORE_INCLUDE_DIR Ecore.h
          HINTS ${PC_LIBECORE_INCLUDEDIR} ${PC_LIBECORE_INCLUDE_DIRS}
          PATH_SUFFIXES ecore-1 )

find_library(ECORE_LIBRARY NAMES ecore
             HINTS ${PC_LIBECORE_LIBDIR} ${PC_LIBECORE_LIBRARY_DIRS} )

set(ECORE_LIBRARIES ${ECORE_LIBRARY} )
set(ECORE_INCLUDE_DIRS ${ECORE_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ECORE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(ecore  DEFAULT_MSG
                                  ECORE_LIBRARY ECORE_INCLUDE_DIR)

mark_as_advanced( ECORE_INCLUDE_DIR ECORE_LIBRARY )
