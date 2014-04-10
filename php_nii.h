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

#ifndef PHP_NII_H
#define PHP_NII_H

extern zend_module_entry nii_module_entry;
#define phpext_nii_ptr &nii_module_entry

#ifdef PHP_WIN32
#	define PHP_NII_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_NII_API __attribute__ ((visibility("default")))
#else
#	define PHP_NII_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#if ZEND_DEBUG
/* debug */
	#define NII_DEBUG_PRINTF(arg) php_printf arg
#else
	#define NII_DEBUG_PRINTF(arg)
#endif

#define NII_VERSION 					"1.1.14-dev"

/* Declaration Class */
#define NII_CLASS_FUNCTION(name)   	ZEND_MINIT_FUNCTION(nii_ ##name)

/* Call Class */
#define NII_CLASS(name)	 			ZEND_MODULE_STARTUP_N(nii_ ##name)(INIT_FUNC_ARGS_PASSTHRU)

/* Declaration Class Method */
#define NII_CLASS_METHODS(name) const zend_function_entry nii_ ##name## _method_entry[] =

/* Declaration class Entry */
#define NII_CLASS_DECLARE_ENTRY(name) zend_class_entry *nii_ ##name## _ce;

/* Declaration class Entry Extern*/
#define NII_CLASS_DECLARE_ENTRY_EX(name) extern zend_class_entry *nii_ ##name## _ce;

/* Call class Entry Extern*/
#define NII_CLASS_ENTRY(name) nii_ ##name## _ce

/** class/interface registering */
#define NII_CLASS_DECLARE(class_name, name, flags) \
	{ \
		zend_class_entry ce; \
		INIT_CLASS_ENTRY(ce, #class_name, nii_ ##name## _method_entry); \
		nii_ ##name## _ce = zend_register_internal_class(&ce TSRMLS_CC); \
		nii_ ##name## _ce->ce_flags |= flags;  \
	}

#define NII_CLASS_DECLARE_EX(class_name, name, parent_name, parent, flags) \
	{ \
		zend_class_entry ce; \
		INIT_CLASS_ENTRY(ce, #class_name, nii_ ##name## _method_entry); \
		nii_ ##name## _ce = zend_register_internal_class_ex(&ce, parent_name, parent TSRMLS_CC); \
		if (!nii_ ##name## _ce) { \
			nii_inherit_not_found(parent, #class_name); \
			return FAILURE;	\
		}  \
		nii_ ##name## _ce->ce_flags |= flags;  \
	}
	
#define NII_NS_CLASS_DECLARE(ns, class_name, name, flags) \
	{ \
		zend_class_entry ce; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, nii_ ##name## _method_entry); \
		nii_ ##name## _ce = zend_register_internal_class(&ce TSRMLS_CC); \
		nii_ ##name## _ce->ce_flags |= flags;  \
	}

#define NII_NS_CLASS_DECLARE_EX(ns, class_name, name, parent_name, parent, flags) \
	{ \
		zend_class_entry ce; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, nii_ ##name## _method_entry); \
		nii_ ##name## _ce = zend_register_internal_class_ex(&ce, parent_name, NULL TSRMLS_CC); \
		if (!nii_ ##name## _ce) { \
			nii_inherit_not_found(parent, ZEND_NS_NAME(#ns, #class_name)); \
			return FAILURE;	\
		}  \
		nii_ ##name## _ce->ce_flags |= flags;  \
	}

#define NII_CLASS_INTERFACE(class_name, name) \
	{ \
		zend_class_entry ce; \
		INIT_CLASS_ENTRY(ce, #class_name, nii_ ##name## _method_entry); \
		nii_ ##name## _ce = zend_register_internal_interface(&ce TSRMLS_CC); \
	}

#define NII_CLASS_INTERFACE_EX(class_name, name, parent) \
	{ \
		zend_class_entry ce; \
		INIT_CLASS_ENTRY(ce, #class_name, nii_ ##name## _method_entry); \
		nii_ ##name## _ce = nii_register_internal_interface_ex(&ce, parent TSRMLS_CC); \
		if (!nii_ ##name## _ce) { \
			fprintf(stderr, "Can't register interface with parent: %s", parent); \
			return FAILURE;	\
		}  \
	}
	
#define NII_NS_CLASS_INTERFACE(ns, class_name, name) \
	{ \
		zend_class_entry ce; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, nii_ ##name## _method_entry); \
		nii_ ##name## _ce = zend_register_internal_interface(&ce TSRMLS_CC); \
	}

#define NII_NS_CLASS_INTERFACE_EX(ns, class_name, name, parent) \
	{ \
		zend_class_entry ce; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, nii_ ##name## _method_entry); \
		nii_ ##name## _ce = nii_register_internal_interface_ex(&ce, parent TSRMLS_CC); \
		if (!nii_ ##name## _ce) { \
			fprintf(stderr, "Can't register interface with parent: %s", parent); \
			return FAILURE;	\
		}  \
	}


#define NII_SL(str) ZEND_STRL(str)
#define NII_SS(str) ZEND_STRS(str)

#define NII_NEW_STRING(zv, str) \
	MAKE_STD_ZVAL(zv); \
	ZVAL_STRING(zv, str, 1);
	
#define NII_NEW_LONG(zv, l) \
	MAKE_STD_ZVAL(zv); \
	ZVAL_LONG(zv, l);
	
#define NII_NEW_BOOL(zv, l) \
	MAKE_STD_ZVAL(zv); \
	ZVAL_BOOL(zv, l);
	
#define NII_NEW_NULL(zv) \
	MAKE_STD_ZVAL(zv); \
	ZVAL_NULL(zv);

#define NII_NEW_ARRAY(arr_name) \
	ALLOC_INIT_ZVAL(arr_name); \
	array_init(arr_name);
	
#define NII_ADD_ARRAY(arr_name, index, item) \
	switch(Z_TYPE_P(item)) { \
		case IS_ARRAY: case IS_OBJECT: \
			add_index_zval(arr_name, index, item); \
			break; \
		case IS_BOOL: \
			add_index_bool(arr_name, index, Z_BVAL_P(item)); \
			break; \
		case IS_LONG: \
			add_index_long(arr_name, index, Z_LVAL_P(item)); \
			break; \
		case IS_DOUBLE: \
			add_index_double(arr_name, index, Z_DVAL_P(item)); \
			break; \
		case IS_RESOURCE: \
			add_index_resource(arr_name, index, Z_RESVAL_P(item)); \
			break; \
		case IS_STRING: \
			add_index_stringl(arr_name, index, Z_STRVAL_P(item), Z_STRLEN_P(item), 1); \
			break; \
		default: \
			add_index_null(arr_name, index); \
	}
	
#define NII_ADD_NEXT_ARRAY(arr_name, item) \
	switch(Z_TYPE_P(item)) { \
		case IS_ARRAY: case IS_OBJECT: \
			add_next_index_zval(arr_name, item); \
			break; \
		case IS_BOOL: \
			add_next_index_bool(arr_name, Z_BVAL_P(item)); \
			break; \
		case IS_LONG: \
			add_next_index_long(arr_name, Z_LVAL_P(item)); \
			break; \
		case IS_DOUBLE: \
			add_next_index_double(arr_name, Z_DVAL_P(item)); \
			break; \
		case IS_RESOURCE: \
			add_next_index_resource(arr_name, Z_RESVAL_P(item)); \
			break; \
		case IS_STRING: \
			add_next_index_stringl(arr_name, Z_STRVAL_P(item), Z_STRLEN_P(item), 1); \
			break; \
		default: \
			add_next_index_null(arr_name); \
	}
	
#define NII_ADD_ARRAY_EX(arr_name, key, item) \
	switch(Z_TYPE_P(item)) { \
		case IS_ARRAY: case IS_OBJECT: \
			add_assoc_zval(arr_name, key, item); \
			break; \
		case IS_BOOL: \
			add_assoc_bool(arr_name, key, Z_BVAL_P(item)); \
			break; \
		case IS_LONG: \
			add_assoc_long(arr_name, key, Z_LVAL_P(item)); \
			break; \
		case IS_DOUBLE: \
			add_assoc_double(arr_name, key, Z_DVAL_P(item)); \
			break; \
		case IS_RESOURCE: \
			add_assoc_resource(arr_name, key, Z_RESVAL_P(item)); \
			break; \
		case IS_STRING: \
			add_assoc_stringl(arr_name, key, Z_STRVAL_P(item), Z_STRLEN_P(item), 1); \
			break; \
		default: \
			add_assoc_null(arr_name, key); \
	}

//
#define NII_DTOR(z) \
	if (&z) {	\
		zval_dtor(&z); \
	}
	
#define NII_PTR_DTOR(zp) \
	if (zp) {	\
		zval_ptr_dtor(&zp); \
	}

#define NII_FREE_ZVAL(z) \
    if (z) { \
        zval_dtor(z); \
        FREE_ZVAL(z); \
    }

PHP_MINIT_FUNCTION(nii);
PHP_MSHUTDOWN_FUNCTION(nii);
PHP_RINIT_FUNCTION(nii);
PHP_RSHUTDOWN_FUNCTION(nii);
PHP_MINFO_FUNCTION(nii);


/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(nii)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(nii)
*/

/* In every utility function you add that needs to use variables 
   in php_nii_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as NII_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define NII_G(v) TSRMG(nii_globals_id, zend_nii_globals *, v)
#else
#define NII_G(v) (nii_globals.v)
#endif

#endif	/* PHP_NII_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
