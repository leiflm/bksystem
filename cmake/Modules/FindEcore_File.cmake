   pkg_check_modules(PC_LIBECORE_X ${_QUIET} ecore-file)
   set(ECORE_FILE_DEFINITIONS ${PC_LIBECORE_FILE_CFLAGS_OTHER})

   find_path(ECORE_FILE_INCLUDE_DIR Ecore_X.h
             HINTS ${PC_LIBECORE_FILE_INCLUDEDIR} ${PC_LIBECORE_FILE_INCLUDE_DIRS}
             PATH_SUFFIXES ecore-1 )

   find_library(ECORE_FILE_LIBRARY NAMES ecore_file
                HINTS ${PC_LIBECORE_FILE_LIBDIR} ${PC_LIBECORE_FILE_LIBRARY_DIRS} )

   set(ECORE_FILE_LIBRARIES ${ECORE_FILE_LIBRARY} )
   set(ECORE_FILE_INCLUDE_DIRS ${ECORE_FILE_INCLUDE_DIR} )

   include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ECORE_FILE_FOUND to TRUE
# if all listed variables are TRUE
   find_package_handle_standard_args(ecore_file  DEFAULT_MSG
                                     ECORE_FILE_LIBRARY ECORE_FILE_INCLUDE_DIR)

   mark_as_advanced( ECORE_FILE_INCLUDE_DIR ECORE_FILE_LIBRARY )


