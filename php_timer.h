/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 310447 2011-04-23 21:14:10Z bjori $ */

#ifndef PHP_TIMER_H
#define PHP_TIMER_H

#define PHP_TIMER_VERSION "1.0.6"
#define PHP_TIMER_EXTNAME "timer"

extern zend_module_entry timer_module_entry;
#define phpext_timer_ptr &timer_module_entry

#ifdef PHP_WIN32
#	define PHP_TIMER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_TIMER_API __attribute__ ((visibility("default")))
#else
#	define PHP_TIMER_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef __APPLE__ // OSX-only
#include "osxGetTime.h"
#endif
#include <time.h>

PHP_MINIT_FUNCTION(timer);
PHP_MSHUTDOWN_FUNCTION(timer);
PHP_RINIT_FUNCTION(timer);
PHP_RSHUTDOWN_FUNCTION(timer);
PHP_MINFO_FUNCTION(timer);

PHP_FUNCTION(timerStart);
PHP_FUNCTION(timerCheckpoint);
PHP_FUNCTION(timerStop);
PHP_FUNCTION(timerDiff);
PHP_FUNCTION(timerGet);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
*/
ZEND_BEGIN_MODULE_GLOBALS(timer)
ZEND_END_MODULE_GLOBALS(timer)
/**/

/* In every utility function you add that needs to use variables 
   in php_timer_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as TIMER_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define TIMER_G(v) TSRMG(timer_globals_id, zend_timer_globals *, v)
#else
#define TIMER_G(v) (timer_globals.v)
#endif

#endif	/* PHP_TIMER_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
