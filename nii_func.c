#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */
#include "ext/standard/php_var.h"
#include "zend_exceptions.h"
#include "php_nii.h"
#include "func.h"
#include "nii_func.h"

uint nii_get_class(zval *ce, const char **classname TSRMLS_DC)
{
	int dup;
	//const char *classname;
	zend_uint classname_len;
	dup = zend_get_object_classname(ce, classname, &classname_len TSRMLS_CC);
	return (uint) classname_len;
}

int nii_getter(zval *ce, char *name, zval **retval TSRMLS_DC)
{
	char *getter;
	int getter_len;
	zval *method_exists_return_zv=NULL;
	zval *getter_zv;

	getter_len = spprintf(&getter, 0, "get%s", name);
	NII_NEW_STRING(getter_zv, getter);
	
	/* method_exists($this,$getter) */
	if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, ce, getter_zv TSRMLS_CC) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			NII_PTR_DTOR(method_exists_return_zv);
			NII_PTR_DTOR(getter_zv);

			if (nii_call_class_method_0(ce, getter, retval TSRMLS_CC) == SUCCESS) {
				efree(getter);
				return SUCCESS;
			}
			
		}
	}

	NII_PTR_DTOR(method_exists_return_zv);
	NII_PTR_DTOR(getter_zv);
	efree(getter);
	return FAILURE;
}

int nii_getter_exception(zval *ce, char *name, const char *classname TSRMLS_DC)
{
	char *getter;
	int getter_len;
	zval *method_exists_return_zv=NULL;
	zval *getter_zv;

	getter_len = spprintf(&getter, 0, "get%s", name);
	NII_NEW_STRING(getter_zv, getter);

	if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, ce, getter_zv TSRMLS_CC) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			char *message;
			int message_len;
			zval *message_zv;

			message_len = spprintf(&message, 0, "Setting read-only property: %s::%s", classname, name);
			NII_NEW_STRING(message_zv, message);

			zval *invalidcallexception_zv;
			MAKE_STD_ZVAL(invalidcallexception_zv);

			if (nii_new_class_instance_1(&invalidcallexception_zv, "nii\\base\\InvalidCallException", message_zv TSRMLS_CC) == SUCCESS) {
				zend_throw_exception_object(invalidcallexception_zv TSRMLS_CC);
				return SUCCESS;
			}
		}
	}
	return FAILURE;
}

int nii_setter(zval *ce, char *name, zval *value TSRMLS_DC)
{
	char *setter;
	int setter_len;
	zval *method_exists_return_zv=NULL;
	zval *setter_zv;

	setter_len = spprintf(&setter, 0, "set%s", name);
	NII_NEW_STRING(setter_zv, setter);

	/* method_exists($this,$setter) */
	if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, ce, setter_zv TSRMLS_CC) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			/*return $this->$setter($value);*/
			if (nii_call_class_method_1_no(ce, setter, value TSRMLS_CC) == SUCCESS) {
				NII_PTR_DTOR(method_exists_return_zv);
				NII_PTR_DTOR(setter_zv);
				efree(setter);
				return SUCCESS;
			}
			
		}
	}

	NII_PTR_DTOR(method_exists_return_zv);
	NII_PTR_DTOR(setter_zv);
	efree(setter);
	return FAILURE;
}

int nii_setter_exception(zval *ce, char *name, const char *classname TSRMLS_DC)
{
	zval *method_exists_return_zv=NULL;

	char *setter;
	int setter_len;
	zval *setter_zv;

	setter_len = spprintf(&setter, 0, "set%s", name);
	NII_NEW_STRING(setter_zv, setter);

	if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, ce, setter_zv TSRMLS_CC) == SUCCESS) {
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
				return SUCCESS;
			}
		}
	}
	return FAILURE;
}

void nii_getter_unknow_exception(zval *ce, char *name, const char *classname TSRMLS_DC)
{
	char *message;
	int message_len;
	zval *message_zv;

	message_len = spprintf(&message, 0, "Getting unknown property: %s::%s", classname, name);
	NII_NEW_STRING(message_zv, message);

	zval *unknownpropertyexception;
	MAKE_STD_ZVAL(unknownpropertyexception);

	if (nii_new_class_instance_1(&unknownpropertyexception, "nii\\base\\UnknownPropertyException", message_zv TSRMLS_CC) == SUCCESS) {
		zend_throw_exception_object(unknownpropertyexception TSRMLS_CC);
	}
}

void nii_setter_unknow_exception(zval *ce, char *name, const char *classname TSRMLS_DC)
{
	char *message;
	int message_len;
	zval *message_zv;

	message_len = spprintf(&message, 0, "Setting unknown property: %s::%s", classname, name);
	NII_NEW_STRING(message_zv, message);

	zval *unknownpropertyexception;
	MAKE_STD_ZVAL(unknownpropertyexception);

	if (nii_new_class_instance_1(&unknownpropertyexception, "nii\\base\\UnknownPropertyException", message_zv TSRMLS_CC) == SUCCESS) {
		zend_throw_exception_object(unknownpropertyexception TSRMLS_CC);
	}
}