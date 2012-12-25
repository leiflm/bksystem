   pkg_check_modules(PC_LIBECORE_X ${_QUIET} ecore-x)
   set(ECORE_X_DEFINITIONS ${PC_LIBECORE_X_CFLAGS_OTHER})

   find_path(ECORE_X_INCLUDE_DIR Ecore_X.h
             HINTS ${PC_LIBECORE_X_INCLUDEDIR} ${PC_LIBECORE_X_INCLUDE_DIRS}
             PATH_SUFFIXES ecore-1 )

   find_library(ECORE_X_LIBRARY NAMES ecore_x
                HINTS ${PC_LIBECORE_X_LIBDIR} ${PC_LIBECORE_X_LIBRARY_DIRS} )

   set(ECORE_X_LIBRARIES ${ECORE_X_LIBRARY} )
   set(ECORE_X_INCLUDE_DIRS ${ECORE_X_INCLUDE_DIR} )

   include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ECORE_X_FOUND to TRUE
# if all listed variables are TRUE
   find_package_handle_standard_args(ecore_x  DEFAULT_MSG
                                     ECORE_X_LIBRARY ECORE_X_INCLUDE_DIR)

   mark_as_advanced( ECORE_X_INCLUDE_DIR ECORE_X_LIBRARY )

