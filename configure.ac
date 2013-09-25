dnl Process this file with autoconf to produce a configure script
AC_INIT([esidl], [0.4.0])
AC_CONFIG_AUX_DIR(.)
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([foreign no-define])
AC_PROG_YACC
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AS
AC_PROG_RANLIB
AC_PROG_LEX
AC_CONFIG_FILES([
  esidl.spec
  debian/changelog
  Makefile
  testsuite/Makefile
  testsuite/runtime/Makefile
  java/Makefile
  cplusplus/Makefile 
  jsapi/Makefile 
  v8api/Makefile
])

AC_PATH_PROG([HELP2MAN], [help2man])
if test x"$HELP2MAN" = x; then
    AC_MSG_ERROR([Cannot find help2man.])
fi

AC_MSG_CHECKING([whether to generate Java interfaces])
AC_ARG_ENABLE(java, AC_HELP_STRING([--enable-java],
                                   [enable Java interface generation [[default=no]]]), 
              [], [enable_java="no"])
AC_MSG_RESULT([$enable_java])
AM_CONDITIONAL(ENABLE_JAVA, test "$enable_java" = "yes")

AC_MSG_CHECKING([whether to generate C++ interfaces])
AC_ARG_ENABLE(cplusplus, AC_HELP_STRING([--enable-cplusplus],
                                        [enable C++ interface generation [[default=no]]]), 
              [], [enable_cplusplus="no"])
AC_MSG_RESULT([$enable_cplusplus])
AM_CONDITIONAL(ENABLE_CPLUSPLUS, test "$enable_cplusplus" = "yes")

AC_MSG_CHECKING([whether to generate JSAPI interfaces])
AC_ARG_ENABLE(jsapi, AC_HELP_STRING([--enable-jsapi],
                                    [enable JSAPI bridge [[default=no]]]), 
              [], [enable_jsapi="no"])
AC_MSG_RESULT([$enable_jsapi])
AM_CONDITIONAL(ENABLE_JSAPI, test "$enable_jsapi" = "yes")

# check for SpiderMonkey 1.8.5
AC_SEARCH_LIBS(JS_NewCompartmentAndGlobalObject, js mozjs185)

AC_MSG_CHECKING([whether to generate V8 API interfaces])
AC_ARG_ENABLE(v8api, AC_HELP_STRING([--enable-v8api],
                                    [enable V8 API bridge [[default=no]]]), 
              [], [enable_v8api="no"])
AC_MSG_RESULT([$enable_v8api])
AM_CONDITIONAL(ENABLE_V8API, test "$enable_v8api" = "yes")

AC_OUTPUT
