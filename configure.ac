dnl Process this file with autoconf to produce a configure script
AC_INIT([esidl], [0.1.5])
AC_CONFIG_AUX_DIR(.)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign no-define])
AC_PROG_YACC
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AS
AC_PROG_LEX
AM_PROG_LIBTOOL
AC_CONFIG_FILES([
  esidl.spec
  Makefile
  testsuite/Makefile
  testsuite/runtime/Makefile
  java/Makefile
  cplusplus/Makefile 
  npapi/Makefile
  npapi/sample/Makefile
  jsapi/Makefile 
  v8api/Makefile
])

case ${target} in
  i[[34567]]86-*-*)
	cpu_type=i386
	;;
  x86_64-*-*)
	cpu_type=x86_64
	;;
  nacl-*-*)
	nacl=yes
	EXEEXT=.nexe
	;;
  *)
	echo 'unknown cpu type';
	exit 1
	;;
esac

AM_CONDITIONAL(I386, test x$cpu_type = xi386)
AM_CONDITIONAL(X86_64, test x$cpu_type = xx86_64)
AM_CONDITIONAL(NACL, test x$nacl = xyes)

AC_MSG_CHECKING([whether to generate Java interfaces])
AC_ARG_ENABLE(java, AC_HELP_STRING([--enable-java],
                                   [enable Java interface generation [[default=yes]]]), 
              [], [enable_java="yes"])
AC_MSG_RESULT([$enable_java])
AM_CONDITIONAL(ENABLE_JAVA, test "$enable_java" = "yes")

AC_MSG_CHECKING([whether to generate NPAPI interfaces])
AC_ARG_ENABLE(npapi, AC_HELP_STRING([--enable-npapi],
                                    [enable NPAPI demo [[default=yes]]]), 
              [], [enable_npapi="yes"])
AC_MSG_RESULT([$enable_npapi])
AM_CONDITIONAL(ENABLE_NPAPI, test "$enable_npapi" = "yes")

AC_MSG_CHECKING([whether to generate C++ interfaces])
AC_ARG_ENABLE(cplusplus, AC_HELP_STRING([--enable-cplusplus],
                                        [enable C++ interface generation [[default=yes]]]), 
              [], [enable_cplusplus="yes"])
AC_MSG_RESULT([$enable_cplusplus])
AM_CONDITIONAL(ENABLE_CPLUSPLUS, test "$enable_cplusplus" = "yes")

AC_MSG_CHECKING([whether to generate JSAPI interfaces])
AC_ARG_ENABLE(jsapi, AC_HELP_STRING([--enable-jsapi],
                                    [enable JSAPI bridge [[default=yes]]]), 
              [], [enable_jsapi="yes"])
AC_MSG_RESULT([$enable_jsapi])
AM_CONDITIONAL(ENABLE_JSAPI, test "$enable_jsapi" = "yes")

# check for SpiderMonkey 1.8.5
AC_SEARCH_LIBS(JS_NewCompartmentAndGlobalObject, js mozjs185)

AC_MSG_CHECKING([whether to generate V8 API interfaces])
AC_ARG_ENABLE(v8api, AC_HELP_STRING([--enable-v8api],
                                    [enable V8 API bridge [[default=yes]]]), 
              [], [enable_v8api="yes"])
AC_MSG_RESULT([$enable_v8api])
AM_CONDITIONAL(ENABLE_V8API, test "$enable_v8api" = "yes")

AC_OUTPUT
