dnl Process this file with autoconf to produce a configure script
AC_INIT([esidl], [0.1.5])
AC_CONFIG_AUX_DIR(.)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AC_PROG_YACC
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AS
AC_PROG_LEX
AM_PROG_LIBTOOL
AC_CONFIG_FILES(Makefile testsuite/Makefile testsuite/runtime/Makefile npapi/Makefile npapi/include/Makefile npapi/include/w3c/Makefile npapi/sample/Makefile java/Makefile)

case ${target} in
  i[[34567]]86-*-*)
	cpu_type=i386
	;;
  x86_64-*-*)
	cpu_type=x86_64
	;;
  *)
	echo 'unknown cpu type';
	exit 1
	;;
esac

AM_CONDITIONAL(I386, test x$cpu_type = xi386)
AM_CONDITIONAL(X86_64, test x$cpu_type = xx86_64)

AC_OUTPUT

