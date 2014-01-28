dnl $Id$
dnl config.m4 for extension timer

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(timer, for timer support,
dnl Make sure that the comment is aligned:
dnl [  --with-timer             Include timer support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(timer, whether to enable timer support,
dnl Make sure that the comment is aligned:
[  --enable-timer           Enable timer support])

if test "$PHP_TIMER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-timer -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/timer.h"  # you most likely want to change this
  dnl if test -r $PHP_TIMER/$SEARCH_FOR; then # path given as parameter
  dnl   TIMER_DIR=$PHP_TIMER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for timer files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TIMER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TIMER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the timer distribution])
  dnl fi

  dnl # --with-timer -> add include path
  dnl PHP_ADD_INCLUDE($TIMER_DIR/include)

  dnl # --with-timer -> check for lib and symbol presence
  dnl LIBNAME=timer # you may want to change this
  dnl LIBSYMBOL=timer # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TIMER_DIR/lib, TIMER_SHARED_LIBADD)
    AC_DEFINE(HAVE_TIMERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong timer lib version or lib not found])
  dnl ],[
  dnl   -L$TIMER_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(TIMER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(timer, timer.c, $ext_shared)
fi
