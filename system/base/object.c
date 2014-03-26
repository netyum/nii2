#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */
#include "ext/standard/php_var.h"
#include "ext/spl/spl_iterators.h"
#include "zend_globals.h"
#include "zend_exceptions.h"
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
    public function __get($name)
    {
        $getter = 'get' . $name;
        if (method_exists($this, $getter)) {
            return $this->$getter();
        } elseif (method_exists($this, 'set' . $name)) {
            throw new InvalidCallException('Getting write-only property: ' . get_class($this) . '::' . $name);
        } else {
            throw new UnknownPropertyException('Getting unknown property: ' . get_class($this) . '::' . $name);
        }
    }
*/
PHP_METHOD(Object, __get){
	char *name, *getter;
	int name_len, getter_len;
	zval *method_exists_return_zv=NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

	/*$getter = 'get' . $name;*/
	getter_len = spprintf(&getter, 0, "get%s", name);
	zval *getter_zv;
	NII_NEW_STRING(getter_zv, getter);

	/* method_exists($this,$getter) */
	if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, getThis(), getter_zv) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			/* return $this->$getter();*/
			zval *getter_retval_zv;

			if (nii_call_class_method_0(getThis(), getter, &getter_retval_zv) == SUCCESS) {
				RETVAL_ZVAL(getter_retval_zv, 1, 0);
				
				NII_PTR_DTOR(method_exists_return_zv);
				NII_PTR_DTOR(getter_zv);
				NII_PTR_DTOR(getter_retval_zv);
				return;
			}
			
		}
	}

	NII_PTR_DTOR(method_exists_return_zv);
	//get class name
	int dup;
	const char *classname;
	zend_uint classname_len;
	dup = zend_get_object_classname(getThis(), &classname, &classname_len TSRMLS_CC);

	/*method_exists($this, 'set' . $name)*/
	char *setter;
	int setter_len;
	zval *setter_zv;

	setter_len = spprintf(&setter, 0, "set%s", name);
	NII_NEW_STRING(setter_zv, setter);

	if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, getThis(), setter_zv) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			/* throw new InvalidCallException('Getting write-only property: ' . get_class($this) . '::' . $name); */

			char *message;
			int message_len;
			zval *message_zv;

			message_len = spprintf(&message, 0, "Getting write-only property: %s::%s", classname, name);
			NII_NEW_STRING(message_zv, message);

			zval *invalidcallexception_zv;
			MAKE_STD_ZVAL(invalidcallexception_zv);

			if (nii_new_class_instance_1(&invalidcallexception_zv, "nii\\base\\InvalidCallException", message_zv TSRMLS_CC) == SUCCESS) {
				zend_throw_exception_object(invalidcallexception_zv TSRMLS_CC);
				return;
			}
		}
	}


	char *message;
	int message_len;
	zval *message_zv;

	message_len = spprintf(&message, 0, "Getting unknown property: %s::%s", classname, name);
	NII_NEW_STRING(message_zv, message);

	zval *unknownpropertyexception;
	MAKE_STD_ZVAL(unknownpropertyexception);

	if (nii_new_class_instance_1(&unknownpropertyexception, "nii\\base\\UnknownPropertyException", message_zv TSRMLS_CC) == SUCCESS) {
		zend_throw_exception_object(unknownpropertyexception TSRMLS_CC);
		return;
	}
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
	char *name;
	int name_len;

	zval *params_zv;
	zval *unknownmethodexception_zv;
	MAKE_STD_ZVAL(unknownmethodexception_zv);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &name, &name_len, &params_zv) == FAILURE) {
		return;
	}

	//get class name
	int dup;
	const char *classname;
	zend_uint classname_len;
	dup = zend_get_object_classname(getThis(), &classname, &classname_len TSRMLS_CC);

	char *message;
	int message_len;
	zval *message_zv;

	message_len = spprintf(&message, 0, "Unknown method: %s::%s()", classname, name);
	NII_NEW_STRING(message_zv, message);

	//UnknownMethodException('Unknown method: ' . get_class($this) . "::$name()");
	if (nii_new_class_instance_1(&unknownmethodexception_zv, "nii\\base\\UnknownMethodException", message_zv TSRMLS_CC) == SUCCESS) {
		zend_throw_exception_object(unknownmethodexception_zv TSRMLS_CC);
		return;
	}
	return;
	
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