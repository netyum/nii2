#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */
#include "ext/standard/php_var.h"
#include "ext/spl/spl_iterators.h"
#include "zend_globals.h"
#include "php_nii.h"
#include "func.h"
#include "system/base/object.h"


NII_CLASS_DECLARE_ENTRY(base_object);

/** {{{ ARG_INFO
*/
//__construct
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

//__get
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object___get, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//__set
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object___set, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

//__isset
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object___isset, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//__unset
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object___unset, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//__call
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object___call, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_ARRAY_INFO(0, parameters, 0)
ZEND_END_ARG_INFO()

//hasProperty
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object_hasProperty, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, check_vars)
ZEND_END_ARG_INFO()

//canGetProperty
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object_canGetProperty, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, check_vars)
ZEND_END_ARG_INFO()

//canSetProperty
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object_canSetProperty, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, check_vars)
ZEND_END_ARG_INFO()

//hasMethod
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_object_hasMethod, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()
/* }}} */


/** {{{ string static public Object::className()
*/
PHP_METHOD(Object, className){
    if (zend_parse_parameters_none() == FAILURE) {
            return;
    }

    if (EG(called_scope)) {
            RETURN_STRINGL(EG(called_scope)->name, EG(called_scope)->name_length, 1);
    } else if (!EG(scope))  {
            zend_error(E_WARNING, "get_called_class() called from outside a class");
    }
    RETURN_FALSE;

    //return;
	//int len;
	//char *strg;
	//len = spprintf(&strg, 0, YII_VERSION);
	//RETURN_STRINGL(strg, len, 0);
}
/* }}} */

/** {{{ public Object::__construct()
*/
PHP_METHOD(Object, __construct){
    if (nii_call_class_method_0_no(getThis(), "init") != SUCCESS) {
        return;
    }
}
/* }}} */

/** {{{ public Object::init()
*/
PHP_METHOD(Object, init){

}
/* }}} */

/** {{{ public Object::__get()
*/
PHP_METHOD(Object, __get){

}
/* }}} */

/** {{{ public Object::__set()
*/
PHP_METHOD(Object, __set){

}
/* }}} */

/** {{{ public Object::__isset()
*/
PHP_METHOD(Object, __isset){

}
/* }}} */

/** {{{ public Object::__unset()
*/
PHP_METHOD(Object, __unset){

}
/* }}} */

/** {{{ public Object::__call()
*/
PHP_METHOD(Object, __call){

}
/* }}} */

/** {{{ public Object::hasProperty()
*/
PHP_METHOD(Object, hasProperty){

}
/* }}} */

/** {{{ public Object::canGetProperty()
*/
PHP_METHOD(Object, canGetProperty){

}
/* }}} */

/** {{{ public Object::canSetProperty()
*/
PHP_METHOD(Object, canSetProperty){

}
/* }}} */

/** {{{ public Object::hasMethod()
*/
PHP_METHOD(Object, hasMethod){
	char *name;
	int name_len;
	zval *name_zv, *method_exists_return_zv=NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

    NII_NEW_STRING(name_zv, name);
    /* method_exists($this, name) */
    if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, getThis(), name_zv) == SUCCESS) {
        RETVAL_TRUE;

        NII_FREE_ZVAL(name_zv);
        NII_FREE_ZVAL(method_exists_return_zv);

        return;
    }

    NII_FREE_ZVAL(name_zv);
    NII_FREE_ZVAL(method_exists_return_zv);
    RETURN_FALSE;
}
/* }}} */

/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(base_object){
	PHP_ME(Object, className, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC) //静态方法
	PHP_ME(Object, __construct, arginfo_base_object___construct, ZEND_ACC_PUBLIC) //构造方法
	PHP_ME(Object, init, NULL, ZEND_ACC_PUBLIC) //init方法
	PHP_ME(Object, __set, arginfo_base_object___set, ZEND_ACC_PUBLIC)
	PHP_ME(Object, __get, arginfo_base_object___get, ZEND_ACC_PUBLIC)
	PHP_ME(Object, __isset, arginfo_base_object___isset, ZEND_ACC_PUBLIC)
	PHP_ME(Object, __unset, arginfo_base_object___unset, ZEND_ACC_PUBLIC)
	PHP_ME(Object, __call, arginfo_base_object___call, ZEND_ACC_PUBLIC)
	PHP_ME(Object, hasProperty, arginfo_base_object_hasProperty, ZEND_ACC_PUBLIC)
	PHP_ME(Object, canGetProperty, arginfo_base_object_canGetProperty, ZEND_ACC_PUBLIC)
	PHP_ME(Object, canSetProperty, arginfo_base_object_canSetProperty, ZEND_ACC_PUBLIC)
	PHP_ME(Object, hasMethod, arginfo_base_object_hasMethod, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\\base\\Object
*/
NII_CLASS_FUNCTION(base_object) {


	//AUTO CALL CLASS METHOD
	NII_NS_CLASS_DECLARE(nii\\base, Object, base_object, 0);
	return SUCCESS;
}
/* }}} */