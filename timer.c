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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
#include "php_timer.h"
#include <time.h>

#ifdef __APPLE__ // OSX-only
#include "osxGetTime.h"
#endif

/* If you declare any globals in php_timer.h uncomment this:*/
ZEND_DECLARE_MODULE_GLOBALS(timer)
/**/

/* True global resources - no need for thread safety here */
static int le_timer;
static double *times = NULL;
static int num_elements = 0;
static int num_allocated = 0;

/* {{{ timer_functions[]
 *
 * Every user visible function must have an entry in timer_functions[].
 */
/*const zend_function_entry timer_functions[] = {
	PHP_FE(timer,	NULL)
	PHP_FE_END
};*/
/* }}} */

static function_entry timer_functions[] = {
	PHP_FE(timerGet, NULL)
	PHP_FE(timerStart, NULL)
	PHP_FE(timerCheckpoint, NULL)
	PHP_FE(timerStop, NULL)
	PHP_FE(timerDiff, NULL)
	{NULL, NULL, NULL}
};

/* {{{ timer_module_entry
 */
zend_module_entry timer_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_TIMER_EXTNAME,
	timer_functions,
	PHP_MINIT(timer),
	PHP_MSHUTDOWN(timer),
	PHP_RINIT(timer),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(timer),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(timer),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_TIMER_VERSION, /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TIMER
ZEND_GET_MODULE(timer)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("timer.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_timer_globals, timer_globals)
    STD_PHP_INI_ENTRY("timer.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_timer_globals, timer_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_timer_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_timer_init_globals(zend_timer_globals *timer_globals)
{
	timer_globals->global_value = 0;
	timer_globals->global_string = NULL;
}
*/
/* }}} */

struct timespec gettime(void) {
	struct timespec temp;
#ifdef CLOCK_MONOTONIC_RAW
	clock_gettime(CLOCK_MONOTONIC_RAW, &temp);
#elif CLOCK_MONOTONIC
	clock_gettime(CLOCK_MONOTONIC, &temp);
#elif CLOCK_REALTIME
	clock_gettime(CLOCK_REALTIME, &temp);
#endif
	return temp;
}

int addToArray(double item) {
	if (num_elements >= num_allocated) {
		if (num_allocated == 0) {
			num_allocated = 20;
		} else {
			num_allocated = num_allocated + 20;
		}
		void *_tmp = realloc(times, (num_allocated * sizeof(double)));
		if (!_tmp) {
			fprintf(stderr, "ERROR: TimerHP: Couldn't realloc memory!\n");
			return(-1);
		}
		times = (double*)_tmp;
	}
	times[num_elements] = (double)item;
	num_elements++;
	return num_elements - 1;
}

double convertTsToDouble(struct timespec thetime) {
	return ((double)thetime.tv_nsec / (double)1000000000) + (double)thetime.tv_sec;
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(timer)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	addToArray(convertTsToDouble(gettime()));
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(timer)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(timer)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(timer)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(timer)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "timer support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */

double diff(double start, double end) {
	double tmp = end - start;
	if (tmp < 0) {
		tmp = tmp * -1;
	}
	return (double)tmp;
}

/* {{{ proto string confirm_timer_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(timerStart) {
	RETURN_LONG(addToArray(convertTsToDouble(gettime())));
}
/* }}} */

/* {{{ proto string confirm_timer_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(timerCheckpoint) {
	RETURN_LONG(addToArray(convertTsToDouble(gettime())));
}
/* }}} */

/* {{{ proto string confirm_timer_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(timerStop) {
	RETURN_LONG(addToArray(convertTsToDouble(gettime())));
}
/* }}} */

/* {{{ proto string confirm_timer_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(timerDiff) {
	long time1;
	long time2;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &time1, &time2) == FAILURE) {
		RETURN_NULL();
	}
	if (time1 >= num_elements || time2 >= num_elements || time1 < 0 || time2 < 0) {
		zend_throw_exception(zend_get_error_exception(), "Invalid timer ID", -1 TSRMLS_CC);
		RETURN_NULL();
	}
	RETURN_DOUBLE(diff(times[time1], times[time2]));
}
/* }}} */

/* {{{ proto string confirm_timer_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(timerGet) {
	RETURN_DOUBLE(diff(times[0], convertTsToDouble(gettime())));
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
