AC_PREREQ([2.69])
AC_INIT([libsqrl],[0.3.11],
  [https://github.com/charwliu/libsqrl/issues],
  [libsqrl],
  [https://github.com/charwliu/libsqrl])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([config.h.in])
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([1.11 dist-bzip2 tar-ustar foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_MAINTAINER_MODE
AM_DEP_TRACK
AC_CONFIG_HEADERS([config.h])

SQRL_LIBRARY_VERSION_MAJOR=0
SQRL_LIBRARY_VERSION_MINOR=3
DLL_VERSION=1
SQRL_LIBRARY_VERSION=1:1:1

AC_SUBST(SQRL_LIBRARY_VERSION_MAJOR)
AC_SUBST(SQRL_LIBRARY_VERSION_MINOR)
AC_SUBST(SQRL_LIBRARY_VERSION)
AC_SUBST(DLL_VERSION)



# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11(noext)

LT_INIT
AC_SUBST(LIBTOOL_DEPS)


dnl Libtool.

LT_INIT([dlopen])
AC_LIBTOOL_WIN32_DLL
gl_LD_OUTPUT_DEF



# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h mach/mach.h stdint.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T



# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([clock_gettime gethrtime gettimeofday memset strchr strstr strtol])

AC_CONFIG_FILES([Makefile
                 libsqrl.pc
                 libsqrl-uninstalled.pc
                 src/Makefile
                 include/Makefile
                 include/sqrl/version.h
                 ])
AC_CONFIG_SUBDIRS([libsodium])
AC_OUTPUT
