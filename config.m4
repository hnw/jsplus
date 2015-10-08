dnl $Id$
dnl config.m4 for extension strplus

PHP_ARG_ENABLE(strplus, whether to enable strplus support,
Make sure that the comment is aligned:
[  --enable-strplus           Enable strplus support])

if test "$PHP_STRPLUS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-strplus -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/strplus.h"  # you most likely want to change this
  dnl if test -r $PHP_STRPLUS/$SEARCH_FOR; then # path given as parameter
  dnl   STRPLUS_DIR=$PHP_STRPLUS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for strplus files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       STRPLUS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$STRPLUS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the strplus distribution])
  dnl fi

  dnl # --with-strplus -> add include path
  dnl PHP_ADD_INCLUDE($STRPLUS_DIR/include)

  dnl # --with-strplus -> check for lib and symbol presence
  dnl LIBNAME=strplus # you may want to change this
  dnl LIBSYMBOL=strplus # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $STRPLUS_DIR/$PHP_LIBDIR, STRPLUS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_STRPLUSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong strplus lib version or lib not found])
  dnl ],[
  dnl   -L$STRPLUS_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(STRPLUS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(strplus, strplus.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
