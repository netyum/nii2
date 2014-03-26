/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
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

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_var.h"
#include "ext/spl/spl_iterators.h"
#include "php_nii.h"
#include "func.h"

/* base class */
#include "system/base/object.h"
#include "system/base/exception.h"
#include "system/base/unknownmethodexception.h"
#include "system/base/unknownclassexception.h"
#include "system/base/unknownpropertyexception.h"
#include "system/base/userexception.h"
#include "system/base/invalidcallexception.h"
#include "system/base/invalidconfigexception.h"
#include "system/base/notsupportedexception.h"
#include "system/base/invalidparamexception.h"
#include "system/base/invalidrouteexception.h"

  



/* If you declare any globals in php_nii.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(nii)
*/

/* True global resources - no need for thread safety here */
static int le_nii;

zend_class_entry * ze_router_ce;

/* {{{ nii_functions[]
 *
 * Every user visible function must have an entry in nii_functions[].
 */
const zend_function_entry nii_functions[] = {
	PHP_FE_END	/* Must be the last line in nii_functions[] */
};
/* }}} */


#ifdef COMPILE_DL_NII
ZEND_GET_MODULE(nii)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("nii.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_nii_globals, nii_globals)
    STD_PHP_INI_ENTRY("nii.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_nii_globals, nii_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_nii_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_nii_init_globals(zend_nii_globals *nii_globals)
{
	nii_globals->global_value = 0;
	nii_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(nii)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/


	/* base class */
	NII_CLASS(base_object);


	/* exception */
	NII_CLASS(base_exception);
	NII_CLASS(base_unknownmethodexception);
	NII_CLASS(base_unknownclassexception);
	NII_CLASS(base_unknownpropertyexception);
	NII_CLASS(base_userexception);
	NII_CLASS(base_invalidcallexception);
	NII_CLASS(base_invalidconfigexception);
	NII_CLASS(base_notsupportedexception);
	NII_CLASS(base_invalidparamexception);
	NII_CLASS(base_invalidrouteexception);


	return SUCCESS;
}

/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(nii)
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
PHP_RINIT_FUNCTION(nii)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(nii)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(nii)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "nii support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ nii_module_entry
 */
zend_module_entry nii_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"nii",
	nii_functions,
	PHP_MINIT(nii),
	PHP_MSHUTDOWN(nii),
	PHP_RINIT(nii),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(nii),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(nii),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
