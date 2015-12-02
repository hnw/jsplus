dnl $Id$
dnl config.m4 for extension jsplus

PHP_ARG_ENABLE(jsplus, whether to enable jsplus support,
Make sure that the comment is aligned:
[  --enable-jsplus           Enable jsplus support])

if test "$PHP_JSPLUS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-jsplus -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/jsplus.h"  # you most likely want to change this
  dnl if test -r $PHP_JSPLUS/$SEARCH_FOR; then # path given as parameter
  dnl   JSPLUS_DIR=$PHP_JSPLUS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for jsplus files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       JSPLUS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$JSPLUS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the jsplus distribution])
  dnl fi

  dnl # --with-jsplus -> add include path
  dnl PHP_ADD_INCLUDE($JSPLUS_DIR/include)

  dnl # --with-jsplus -> check for lib and symbol presence
  dnl LIBNAME=jsplus # you may want to change this
  dnl LIBSYMBOL=jsplus # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $JSPLUS_DIR/$PHP_LIBDIR, JSPLUS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_JSPLUSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong jsplus lib version or lib not found])
  dnl ],[
  dnl   -L$JSPLUS_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(JSPLUS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(jsplus, jsplus.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
