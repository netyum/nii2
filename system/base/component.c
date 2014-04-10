#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "string.h"

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */
#include "ext/standard/php_string.h"
#include "ext/standard/php_var.h"
#include "ext/spl/spl_iterators.h"
#include "zend_globals.h"
#include "zend_exceptions.h"
#include "php_nii.h"
#include "func.h"
#include "nii_func.h"
#include "system/base_nii.h"
#include "system/base/object.h"
#include "system/base/behavior.h"
#include "system/base/component.h"

NII_CLASS_DECLARE_ENTRY(base_component);

/** {{{ ARG_INFO
*/
//__get
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component___get, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//__set
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component___set, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

//__isset
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component___isset, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//__unset
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component___unset, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//__call
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component___call, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_ARRAY_INFO(0, parameters, 0)
ZEND_END_ARG_INFO()

//hasProperty
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_hasProperty, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, check_vars)
	ZEND_ARG_INFO(0, check_behaviors)
ZEND_END_ARG_INFO()

//canGetProperty
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_canGetProperty, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, check_vars)
	ZEND_ARG_INFO(0, check_behaviors)
ZEND_END_ARG_INFO()

//canSetProperty
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_canSetProperty, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, check_vars)
	ZEND_ARG_INFO(0, check_behaviors)
ZEND_END_ARG_INFO()

//hasMethod
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_hasMethod, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, check_behaviors)
ZEND_END_ARG_INFO()

//hasEventHandlers
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_hasEventHandlers, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//on
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_on, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, handler)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

//off
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_off, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

//trigger
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_trigger, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, event)
ZEND_END_ARG_INFO()

//getBehavior
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_getBehavior, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//attachBehavior
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_attachBehavior, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, behavior)
ZEND_END_ARG_INFO()

//attachBehaviors
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_attachBehaviors, 0, 0, 1)
	ZEND_ARG_INFO(0, behavior)
ZEND_END_ARG_INFO()

//detachBehavior
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_detachBehavior, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//attachBehaviorInternal
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_component_attachBehaviorInternal, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, behavior)
ZEND_END_ARG_INFO()
/* }}} */


/** {{{ public Component::__get()
    public function __get($name)
    {
        $getter = 'get' . $name;
        if (method_exists($this, $getter)) {
            // read property, e.g. getName()
            return $this->$getter();
        } else {
            // behavior property
            $this->ensureBehaviors();
            foreach ($this->_behaviors as $behavior) {
                if ($behavior->canGetProperty($name)) {
                    return $behavior->$name;
                }
            }
        }
        if (method_exists($this, 'set' . $name)) {
            throw new InvalidCallException('Getting write-only property: ' . get_class($this) . '::' . $name);
        } else {
            throw new UnknownPropertyException('Getting unknown property: ' . get_class($this) . '::' . $name);
        }
    }
*/
PHP_METHOD(Component, __get){
	char *name;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

	zval *retval;

	if (nii_getter(getThis(), name, &retval TSRMLS_CC) == SUCCESS) {
		ZVAL_ZVAL(return_value, retval, 1, 0);
		NII_PTR_DTOR(retval);
		return;
	}

	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}

	zval *behaviors_zv;
	behaviors_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_behaviors"), 0 TSRMLS_CC);


	HashPosition    pos;
	zval **entry;
	char *string_key;
	uint string_key_len;
	ulong index;

	zval *name_zv;
	NII_NEW_STRING(name_zv, name);

    zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(behaviors_zv), &pos);
    while (zend_hash_get_current_data_ex(Z_ARRVAL_P(behaviors_zv), (void **)&entry, &pos) == SUCCESS) {
    	
    	zval *retval;
    	if (nii_call_class_method_1(*entry, "canGetProperty", &retval, name_zv TSRMLS_CC) == SUCCESS) {
    		if (Z_TYPE_P(retval) == IS_BOOL && Z_BVAL_P(retval) == 1) {
    			zval *rretval;
    			if(nii_call_class_method_1(*entry, "__get", &rretval, name_zv TSRMLS_CC) == SUCCESS) {
    				RETVAL_ZVAL(rretval, 1, 0);
    				NII_PTR_DTOR(rretval);
    				NII_PTR_DTOR(retval);
    				return;
    				break;
    			}
    			NII_PTR_DTOR(rretval);
    		}
    	}
    	NII_PTR_DTOR(retval);
        zend_hash_move_forward_ex(Z_ARRVAL_P(behaviors_zv), &pos);
    }

	//get class name
	const char *classname; 
	uint classname_len;
	classname_len = nii_get_class(getThis(), &classname TSRMLS_CC);

	if (nii_setter_exception(getThis(), name, classname TSRMLS_CC) == SUCCESS) return;

	nii_getter_unknow_exception(getThis(), name, classname TSRMLS_CC);
	return;
}
/* }}} */

/** {{{ public Component::__set()
    public function __set($name, $value)
    {
        $setter = 'set' . $name;
        if (method_exists($this, $setter)) {
            // set property
            $this->$setter($value);

            return;
        } elseif (strncmp($name, 'on ', 3) === 0) {
            // on event: attach event handler
            $this->on(trim(substr($name, 3)), $value);

            return;
        } elseif (strncmp($name, 'as ', 3) === 0) {
            // as behavior: attach behavior
            $name = trim(substr($name, 3));
            $this->attachBehavior($name, $value instanceof Behavior ? $value : Yii::createObject($value));

            return;
        } else {
            // behavior property
            $this->ensureBehaviors();
            foreach ($this->_behaviors as $behavior) {
                if ($behavior->canSetProperty($name)) {
                    $behavior->$name = $value;

                    return;
                }
            }
        }
        if (method_exists($this, 'get' . $name)) {
            throw new InvalidCallException('Setting read-only property: ' . get_class($this) . '::' . $name);
        } else {
            throw new UnknownPropertyException('Setting unknown property: ' . get_class($this) . '::' . $name);
        }
    }
*/
PHP_METHOD(Component, __set){
	
	char *name;
	int name_len;
	zval *method_exists_return_zv=NULL;
	zval *value;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &name, &name_len, &value) == FAILURE) {
		return;
	}

	if (nii_setter(getThis(), name, value TSRMLS_CC)  == SUCCESS) return;

	int a = strncmp(name, "on ", 3);
	if (a == 0) {
		char *nnew_str;
		char *new_str;

		new_str = estrndup(name+3, name_len-3);
		nnew_str = php_trim(new_str, name_len-3, NULL, 0, NULL, 3 TSRMLS_CC);
		efree(new_str);

		zval *param_zv;
		NII_NEW_STRING(param_zv, nnew_str);
		if (nii_call_class_method_2_no(getThis(), "on", param_zv, value TSRMLS_CC) == SUCCESS) {
			efree(nnew_str);
			NII_PTR_DTOR(param_zv);
			return;
		}
		efree(nnew_str);
		NII_PTR_DTOR(param_zv);
	}

	int b = strncmp(name, "as ", 3);
	if (b == 0) {
		char *nnew_str;
		char *new_str;
		new_str = estrndup(name+3, name_len-3);
		nnew_str = php_trim(new_str, name_len-3, NULL, 0, NULL, 3 TSRMLS_CC);
		efree(new_str);

		zval *param_zv;
		NII_NEW_STRING(param_zv, nnew_str);
		NII_DEBUG_PRINTF(("There is 1\n"));
		if (Z_TYPE_P(value) == IS_OBJECT) {
			if (instanceof_function(Z_OBJCE_P(value), NII_CLASS_ENTRY(base_behavior) TSRMLS_CC))
			{
				NII_DEBUG_PRINTF(("There is 3\n"));
				if (nii_call_class_method_2_no(getThis(), "attachBehavior", param_zv, value TSRMLS_CC) == SUCCESS) {
					efree(nnew_str);
					NII_PTR_DTOR(param_zv);
					return;
				}
			}
		}
		else if (Z_TYPE_P(value) == IS_ARRAY) {
			zval *retval;
			if (nii_call_class_static_method_1(NULL, "\\Nii", "createObject", &retval, value TSRMLS_CC) == SUCCESS) {
NII_DEBUG_PRINTF(("RUN THERE"));
return;
				if (nii_call_class_method_2_no(getThis(), "attachBehavior", param_zv, retval TSRMLS_CC) == SUCCESS) {
					efree(nnew_str);
					NII_PTR_DTOR(param_zv);
					NII_PTR_DTOR(retval);
					return;
				}
			}
		}
		efree(nnew_str);
		NII_PTR_DTOR(param_zv);
	}

	//get class name
	const char *classname; 
	uint classname_len;
	classname_len = nii_get_class(getThis(), &classname TSRMLS_CC);

	if (nii_getter_exception(getThis(), name, classname TSRMLS_CC) == SUCCESS) return;

	nii_setter_unknow_exception(getThis(), name, classname TSRMLS_CC);
	return;
	
}
/* }}} */

/** {{{ public Component::__isset()
    public function __isset($name)
    {
        $getter = 'get' . $name;
        if (method_exists($this, $getter)) {
            return $this->$getter() !== null;
        } else {
            // behavior property
            $this->ensureBehaviors();
            foreach ($this->_behaviors as $behavior) {
                if ($behavior->canGetProperty($name)) {
                    return $behavior->$name !== null;
                }
            }
        }

        return false;
    }
*/
PHP_METHOD(Component, __isset){
	/*
	char *name;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

	zval *getter_value;
	if (nii_getter(getThis(), name, &getter_value TSRMLS_CC) == SUCCESS) {
		if (Z_TYPE_P(getter_value) != IS_NULL) {
			NII_PTR_DTOR(getter_value);
			RETURN_TRUE;
		}
	}
    RETURN_FALSE;*/

}
/* }}} */

/** {{{ public Component::__unset()
    public function __unset($name)
    {
        $setter = 'set' . $name;
        if (method_exists($this, $setter)) {
            $this->$setter(null);

            return;
        } else {
            // behavior property
            $this->ensureBehaviors();
            foreach ($this->_behaviors as $behavior) {
                if ($behavior->canSetProperty($name)) {
                    $behavior->$name = null;

                    return;
                }
            }
        }
        throw new InvalidCallException('Unsetting an unknown or read-only property: ' . get_class($this) . '::' . $name);
    }
*/
PHP_METHOD(Component, __unset){
	/*char *name;
    int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
	  return;
	}

	char *setter;
	int setter_len;
	zval *method_exists_return_zv=NULL;
	zval *setter_zv;

	NII_NEW_STRING(setter_zv, setter);
	setter_len = spprintf(&setter, 0, "set%s", name);

    if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, getThis(), setter_zv TSRMLS_CC) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			zval *value;
			NII_NEW_NULL(value);
			if (nii_setter(getThis(), name, value TSRMLS_CC) == SUCCESS) {
			    NII_PTR_DTOR(method_exists_return_zv);
			    NII_PTR_DTOR(setter_zv);
			    NII_PTR_DTOR(value);
			    efree(setter);
				return;
			}
		}
    }

    NII_PTR_DTOR(setter_zv);
    NII_PTR_DTOR(method_exists_return_zv);
    efree(setter);

	//get class name
	const char *classname;
	uint classname_len;
	classname_len = nii_get_class(getThis(), &classname TSRMLS_CC);

    char *getter;
    int getter_len;
    zval *getter_zv;
    getter_len = spprintf(&getter, 0, "get%s", name);
    NII_NEW_STRING(getter_zv, getter);
    if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, getThis(), getter_zv TSRMLS_CC) == SUCCESS) {
          if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
               char *message;
               int message_len;
               zval *message_zv;

               message_len = spprintf(&message, 0, "Unsetting read-only property: %s::%s", classname, name);
               NII_NEW_STRING(message_zv, message);

               zval *invalidcallexception;
               MAKE_STD_ZVAL(invalidcallexception);

               if (nii_new_class_instance_1(&invalidcallexception, "nii\\base\\InvalidCallException", message_zv TSRMLS_CC) == SUCCESS) {
                    zend_throw_exception_component(invalidcallexception TSRMLS_CC);
               }
          }
    }

    NII_PTR_DTOR(method_exists_return_zv);
    NII_PTR_DTOR(getter_zv);
    efree(getter);
    return;
    */
}
/* }}} */

/** {{{ public Component::__call()
    public function __call($name, $params)
    {
        $this->ensureBehaviors();
        foreach ($this->_behaviors as $object) {
            if ($object->hasMethod($name)) {
                return call_user_func_array([$object, $name], $params);
            }
        }

        throw new UnknownMethodException('Calling unknown method: ' . get_class($this) . "::$name()");
    }
*/
PHP_METHOD(Component, __call){
	/*char *name;
	int name_len;

	zval *params_zv;
	zval *unknownmethodexception_zv;
	MAKE_STD_ZVAL(unknownmethodexception_zv);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &name, &name_len, &params_zv) == FAILURE) {
		return;
	}

	//get class name
	const char *classname; 
	uint classname_len;
	classname_len = nii_get_class(getThis(), &classname TSRMLS_CC);

	char *message;
	int message_len;
	zval *message_zv;

	message_len = spprintf(&message, 0, "Unknown method: %s::%s()", classname, name);
	NII_NEW_STRING(message_zv, message);

	//UnknownMethodException('Unknown method: ' . get_class($this) . "::$name()");
	if (nii_new_class_instance_1(&unknownmethodexception_zv, "nii\\base\\UnknownMethodException", message_zv TSRMLS_CC) == SUCCESS) {
		zend_throw_exception_component(unknownmethodexception_zv TSRMLS_CC);
		return;
	}
	return;*/
	
}
/* }}} */

/** {{{ public Component::__clone()
    public function __clone()
    {
        $this->_events = [];
        $this->_behaviors = null;
    }
*/
PHP_METHOD(Component, __clone){

}
/* }}} */

/** {{{ public Component::hasProperty()
    public function hasProperty($name, $checkVars = true, $checkBehaviors = true)
    {
        return $this->canGetProperty($name, $checkVars, $checkBehaviors) || $this->canSetProperty($name, false, $checkBehaviors);
    }
*/
PHP_METHOD(Component, hasProperty){
	/*char *name;
	int name_len;
	zval *name_zv, *method_exists_return_zv=NULL;

	zend_bool check_vars = 1;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &name, &name_len, &check_vars) == FAILURE) {
		return;
	}

	zval *check_vars_zv;
	NII_NEW_BOOL(check_vars_zv, check_vars);
	NII_NEW_STRING(name_zv, name);

	if (nii_call_class_method_2(getThis(), "canGetProperty", &method_exists_return_zv, name_zv, check_vars_zv TSRMLS_CC) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			NII_PTR_DTOR(method_exists_return_zv);
			NII_PTR_DTOR(name_zv);
			NII_PTR_DTOR(check_vars_zv);
			RETURN_TRUE;
		}
	}

	NII_PTR_DTOR(method_exists_return_zv);
	ZVAL_BOOL(check_vars_zv, 0);
	
	if (nii_call_class_method_2(getThis(), "canSetProperty", &method_exists_return_zv, name_zv, check_vars_zv TSRMLS_CC) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			NII_PTR_DTOR(method_exists_return_zv);
			NII_PTR_DTOR(name_zv);
			NII_PTR_DTOR(check_vars_zv);
			RETURN_TRUE;
		}
	}
	NII_PTR_DTOR(method_exists_return_zv);
	NII_PTR_DTOR(name_zv);
	NII_PTR_DTOR(check_vars_zv);
	RETURN_FALSE;*/
}
/* }}} */

/** {{{ public Component::canGetProperty()
	public function canGetProperty($name, $checkVars = true, $checkBehaviors = true)
    {
        if (method_exists($this, 'get' . $name) || $checkVars && property_exists($this, $name)) {
            return true;
        } elseif ($checkBehaviors) {
            $this->ensureBehaviors();
            foreach ($this->_behaviors as $behavior) {
                if ($behavior->canGetProperty($name, $checkVars)) {
                    return true;
                }
            }
        }

        return false;
    }
*/
PHP_METHOD(Component, canGetProperty){
	/*char *name;
	int name_len;
	zval *name_zv, *method_exists_return_zv=NULL;

	zend_bool check_vars = 1;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &name, &name_len, &check_vars) == FAILURE) {
		return;
	}


	char *getter;
	int getter_len;
	zval *getter_zv;

	getter_len = spprintf(&getter, 0, "get%s", name);
	NII_NEW_STRING(getter_zv, getter);

    if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, getThis(), getter_zv TSRMLS_CC) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			NII_PTR_DTOR(method_exists_return_zv);
			NII_PTR_DTOR(getter_zv);
			efree(getter);
			RETURN_TRUE;
		}
    }

	NII_PTR_DTOR(method_exists_return_zv);
	NII_PTR_DTOR(getter_zv);
	efree(getter);

    if (check_vars) {
    	NII_NEW_STRING(name_zv, name);
		if (nii_call_user_fun_2("property_exists", &method_exists_return_zv, getThis(), name_zv TSRMLS_CC) == SUCCESS) {
			if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
				NII_PTR_DTOR(method_exists_return_zv);
				NII_PTR_DTOR(name_zv);
				RETURN_TRUE;
			}
		}
    }

	NII_PTR_DTOR(method_exists_return_zv);
	NII_PTR_DTOR(name_zv);
	RETURN_FALSE;*/
}
/* }}} */

/** {{{ public Component::canSetProperty()
    public function canSetProperty($name, $checkVars = true, $checkBehaviors = true)
    {
        if (method_exists($this, 'set' . $name) || $checkVars && property_exists($this, $name)) {
            return true;
        } elseif ($checkBehaviors) {
            $this->ensureBehaviors();
            foreach ($this->_behaviors as $behavior) {
                if ($behavior->canSetProperty($name, $checkVars)) {
                    return true;
                }
            }
        }

        return false;
    }
*/
PHP_METHOD(Component, canSetProperty){
	/*char *name;
	int name_len;
	zval *name_zv, *method_exists_return_zv=NULL;

	zend_bool check_vars = 1;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &name, &name_len, &check_vars) == FAILURE) {
		return;
	}


	char *setter;
	int setter_len;
	zval *setter_zv;

	setter_len = spprintf(&setter, 0, "set%s", name);
	NII_NEW_STRING(setter_zv, setter);

    if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, getThis(), setter_zv TSRMLS_CC) == SUCCESS) {
		if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
			NII_PTR_DTOR(method_exists_return_zv);
			NII_PTR_DTOR(setter_zv);
			efree(setter);
			RETURN_TRUE;
		}
    }

	NII_PTR_DTOR(method_exists_return_zv);
	NII_PTR_DTOR(setter_zv);
	efree(setter);

    if (check_vars) {
    	NII_NEW_STRING(name_zv, name);
		if (nii_call_user_fun_2("property_exists", &method_exists_return_zv, getThis(), name_zv TSRMLS_CC) == SUCCESS) {
			if (Z_TYPE_P(method_exists_return_zv) == IS_BOOL && Z_BVAL_P(method_exists_return_zv) == 1) {
				NII_PTR_DTOR(method_exists_return_zv);
				NII_PTR_DTOR(name_zv);
				RETURN_TRUE;
			}
		}
    }

	NII_PTR_DTOR(method_exists_return_zv);
	NII_PTR_DTOR(name_zv);
	RETURN_FALSE;
	*/
}
/* }}} */

/** {{{ public Component::hasMethod()
    public function hasMethod($name, $checkBehaviors = true)
    {
        if (method_exists($this, $name)) {
            return true;
        } elseif ($checkBehaviors) {
            $this->ensureBehaviors();
            foreach ($this->_behaviors as $behavior) {
                if ($behavior->hasMethod($name)) {
                    return true;
                }
            }
        }

        return false;
    }
*/
PHP_METHOD(Component, hasMethod){
	/*char *name;
	int name_len;
	zval *name_zv, *method_exists_return_zv=NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

    NII_NEW_STRING(name_zv, name);
    if (nii_call_user_fun_2("method_exists", &method_exists_return_zv, getThis(), name_zv TSRMLS_CC) == SUCCESS) {
		NII_PTR_DTOR(method_exists_return_zv);
		NII_PTR_DTOR(name_zv);
		RETURN_TRUE;
    }

	NII_PTR_DTOR(method_exists_return_zv);
	NII_PTR_DTOR(name_zv);
    RETURN_FALSE;*/
}
/* }}} */

/** {{{ public Component::behaviors()
    public function behaviors()
    {
        return [];
    }
*/
PHP_METHOD(Component, behaviors){
	zval *retval;
	NII_NEW_ARRAY(retval);
	RETVAL_ZVAL(retval, 1, 0);
	NII_PTR_DTOR(retval);
	return;
}
/* }}} */

/** {{{ public Component::hasEventHandlers()
    public function hasEventHandlers($name)
    {
        $this->ensureBehaviors();

        return !empty($this->_events[$name]) || Event::hasHandlers($this, $name);
    }
*/
PHP_METHOD(Component, hasEventHandlers){
	char *name;
	int *name_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}

	zval *events_zv;
	events_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_events"), 0 TSRMLS_CC);
	if (Z_TYPE_P(events_zv) != IS_ARRAY) {
		NII_NEW_ARRAY(events_zv);
	}

	zval *class;
	if (zend_hash_find(Z_ARRVAL_P(events_zv), name, (uint)name_len, (void **)&class) == SUCCESS) {

		if (Z_TYPE_P(class)!=IS_NULL) {
			NII_PTR_DTOR(events_zv);
			RETURN_TRUE;
		}

	}

	NII_PTR_DTOR(events_zv);
	RETURN_FALSE;

}
/* }}} */

/** {{{ public Component::on()
    public function on($name, $handler, $data = null)
    {
        $this->ensureBehaviors();
        $this->_events[$name][] = [$handler, $data];
    }
*/
PHP_METHOD(Component, on){
	char *name;
	int *name_len;
	zval *handler_zv, *data_zv=NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz|z", &name, &name_len, &handler_zv, &data_zv) == FAILURE) {
		return;
	}

	if (!data_zv) {
		NII_NEW_NULL(data_zv);
	}
	NII_DEBUG_PRINTF(("in Component::on enter here 1\n"));
	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}
	NII_DEBUG_PRINTF(("in Component::on enter here 2\n"));

	zval *events_zv;
	events_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_events"), 0 TSRMLS_CC);
	NII_DEBUG_PRINTF(("in Component::on enter here 3\n"));
	if (Z_TYPE_P(events_zv) != IS_ARRAY) {
		NII_DEBUG_PRINTF(("in Component::on event is not array\n"));
		NII_NEW_ARRAY(events_zv);
	}

	zval *class;
	if (zend_hash_find(Z_ARRVAL_P(events_zv), name, (uint)name_len, (void **)&class) == SUCCESS) {
		NII_DEBUG_PRINTF(("in Component::on enter here 4\n"));
		zval *tmp_array_zv;
		NII_NEW_ARRAY(tmp_array_zv);
		add_next_index_zval(tmp_array_zv, handler_zv);
		add_next_index_zval(tmp_array_zv, data_zv);

		add_next_index_zval(events_zv, tmp_array_zv);

		zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_events"), events_zv TSRMLS_CC);

		NII_PTR_DTOR(tmp_array_zv);
	}

	NII_PTR_DTOR(data_zv);
	NII_PTR_DTOR(events_zv);
	return;
}
/* }}} */

/** {{{ public Component::off()
    public function off($name, $handler = null)
    {
        $this->ensureBehaviors();
        if (empty($this->_events[$name])) {
            return false;
        }
        if ($handler === null) {
            unset($this->_events[$name]);

            return true;
        } else {
            $removed = false;
            foreach ($this->_events[$name] as $i => $event) {
                if ($event[0] === $handler) {
                    unset($this->_events[$name][$i]);
                    $removed = true;
                }
            }
            if ($removed) {
                $this->_events[$name] = array_values($this->_events[$name]);
            }

            return $removed;
        }
    }
*/
PHP_METHOD(Component, off){
	char *name;
	int *name_len;
	zval *handler_zv = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &name, &name_len, &handler_zv) == FAILURE) {
		return;
	}

	if (!handler_zv) {
		NII_NEW_NULL(handler_zv);
	}

	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}

	zval *events_zv;
	events_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_events"), 0 TSRMLS_CC);
	
	zval *class;
	if (zend_hash_find(Z_ARRVAL_P(events_zv), name, (uint)name_len, (void **)&class) != SUCCESS) {
		NII_PTR_DTOR(handler_zv);
		RETURN_FALSE;
	}

	if (Z_TYPE_P(handler_zv) == IS_NULL) {
		add_assoc_unset(events_zv, name);
		zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_events"), events_zv TSRMLS_CC);
		RETURN_TRUE;
	}

	int removed = 0;
	if (Z_TYPE_P(class) == IS_ARRAY) {
		HashPosition    pos;
		zval **entry;
		char *string_key;
		uint string_key_len;
		ulong index;

	    zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(class), &pos);
	    while (zend_hash_get_current_data_ex(Z_ARRVAL_P(class), (void **)&entry, &pos) == SUCCESS) {
	    	
	    	zval *event_0_zv;
	    	if (zend_hash_index_find(Z_ARRVAL_P(*entry), 0, (void **)&event_0_zv) != SUCCESS) {
	    		return;
	    	}
			zval compare_result;
			//$event[0] === $handler
			is_identical_function(&compare_result, event_0_zv, handler_zv TSRMLS_CC);
            if (Z_LVAL(compare_result)) {
            	if (zend_hash_get_current_key_ex(Z_ARRVAL_P(class), &string_key, &string_key_len, &index, 0, &pos) == HASH_KEY_IS_STRING) {
            		add_assoc_unset(events_zv, string_key);
            	}
            	else {
            		add_index_unset(events_zv, index);
            	}
	        	removed = 1;
            }
	        zend_hash_move_forward_ex(Z_ARRVAL_P(class), &pos);
	    }

	    if (removed == 1) {
	    	//$this->_events[$name] = array_values($this->_events[$name]);
	    	zval *retval;
	    	if (nii_call_user_fun_1("array_values", &retval, class TSRMLS_CC) == SUCCESS) {
	    		add_assoc_zval(events_zv, name, retval);
	    		zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_events"), events_zv TSRMLS_CC);
	    	}
	    }

		zval *retbool;
		NII_NEW_BOOL(retbool, (long) removed);
		RETVAL_BOOL(retbool);
		NII_PTR_DTOR(retbool);
		return;
	}
}
/* }}} */

/** {{{ public Component::trigger()
    public function trigger($name, Event $event = null)
    {
        $this->ensureBehaviors();
        if (!empty($this->_events[$name])) {
            if ($event === null) {
                $event = new Event;
            }
            if ($event->sender === null) {
                $event->sender = $this;
            }
            $event->handled = false;
            $event->name = $name;
            foreach ($this->_events[$name] as $handler) {
                $event->data = $handler[1];
                call_user_func($handler[0], $event);
                // stop further handling if the event is handled
                if ($event->handled) {
                    return;
                }
            }
        }
        // invoke class-level attached handlers
        Event::trigger($this, $name, $event);
    }
*/
PHP_METHOD(Component, trigger){

}
/* }}} */

/** {{{ public Component::getBehavior()
    public function getBehavior($name)
    {
        $this->ensureBehaviors();

        return isset($this->_behaviors[$name]) ? $this->_behaviors[$name] : null;
    }
*/
PHP_METHOD(Component, getBehavior){
	char *name;
	int *name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}

	zval *behaviors_zv;
	behaviors_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_behaviors"), 0 TSRMLS_CC);
	
	zval *class;
	if (zend_hash_find(Z_ARRVAL_P(behaviors_zv), name, (uint)name_len, (void **)&class) == SUCCESS) {
		RETVAL_ZVAL(class, 1, 0);
	}
	else {
		RETVAL_NULL();
	}
	NII_PTR_DTOR(behaviors_zv);
	NII_PTR_DTOR(class);
	return;
}
/* }}} */

/** {{{ public Component::getBehaviors()
    public function getBehaviors()
    {
        $this->ensureBehaviors();

        return $this->_behaviors;
    }
*/
PHP_METHOD(Component, getBehaviors){
	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}
	zval *behaviors_zv;
	behaviors_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_behaviors"), 0 TSRMLS_CC);
	RETVAL_ZVAL(behaviors_zv, 1, 0);
	NII_PTR_DTOR(behaviors_zv);
	return;
}
/* }}} */

/** {{{ public Component::attachBehavior()
    public function attachBehavior($name, $behavior)
    {
        $this->ensureBehaviors();

        return $this->attachBehaviorInternal($name, $behavior);
    }
*/
PHP_METHOD(Component, attachBehavior){
	zval *behavior_zv;
	char *name;
	int *name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &name, &name_len, &behavior_zv) == FAILURE) {
		return;
	}

	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}

	zval *retval;
	zval *name_zv;
	NII_NEW_STRING(name_zv, name);

	if (nii_call_class_method_2(getThis(), "attachBehaviorInternal", &retval, name_zv, behavior_zv TSRMLS_CC) != SUCCESS) {
		NII_PTR_DTOR(name_zv);
		return;
	}

	
	RETVAL_ZVAL(retval, 1, 0);
	NII_PTR_DTOR(name_zv);
	NII_PTR_DTOR(retval);
	return;
}
/* }}} */

/** {{{ public Component::attachBehaviors()
    public function attachBehaviors($behaviors)
    {
        $this->ensureBehaviors();
        foreach ($behaviors as $name => $behavior) {
            $this->attachBehaviorInternal($name, $behavior);
        }
    }
*/
PHP_METHOD(Component, attachBehaviors){
	zval *behaviors_zv;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &behaviors_zv) == FAILURE) {
		return;
	}

	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}

	HashPosition    pos;
	zval **entry;
	char *string_key;
	uint string_key_len;
	ulong index;

    zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(behaviors_zv), &pos);
    while (zend_hash_get_current_data_ex(Z_ARRVAL_P(behaviors_zv), (void **)&entry, &pos) == SUCCESS) {
        if (zend_hash_get_current_key_ex(Z_ARRVAL_P(behaviors_zv), &string_key, &string_key_len, &index, 0, &pos) == HASH_KEY_IS_STRING) {
        	zval *name_zv;
        	NII_NEW_STRING(name_zv, string_key);
        	if (nii_call_class_method_2_no(getThis(), "attachBehaviorInternal", name_zv, *entry TSRMLS_CC) != SUCCESS) {
        		NII_PTR_DTOR(name_zv);
        		return;
        	}
        	NII_PTR_DTOR(name_zv);
        }
        zend_hash_move_forward_ex(Z_ARRVAL_P(behaviors_zv), &pos);
    }
}
/* }}} */

/** {{{ public Component::detachBehavior()
    public function detachBehavior($name)
    {
        $this->ensureBehaviors();
        if (isset($this->_behaviors[$name])) {
            $behavior = $this->_behaviors[$name];
            unset($this->_behaviors[$name]);
            $behavior->detach();

            return $behavior;
        } else {
            return null;
        }
    }
*/
PHP_METHOD(Component, detachBehavior){
	zval *behavior_zv;
	char *name;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &name, &name_len, &behavior_zv) == FAILURE) {
		return;
	}

	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}

	zval *behaviors_zv;
	behaviors_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_behaviors"), 0 TSRMLS_CC);
	if ( Z_TYPE_P(behaviors_zv) != IS_ARRAY) {
		NII_NEW_ARRAY(behaviors_zv);
	}

	zval *class;
	if (zend_hash_find(Z_ARRVAL_P(behaviors_zv), name, name_len, (void **)&class) == SUCCESS) {
		add_assoc_unset(behaviors_zv, name);

		if (nii_call_class_method_0_no(class, "detach" TSRMLS_CC) != SUCCESS ) {
			RETVAL_ZVAL(class, 1, 0);
			return;
		}
	}
	else {
		RETURN_NULL();
	}


}
/* }}} */

/** {{{ public Component::detachBehaviors()
    public function detachBehaviors()
    {
        $this->ensureBehaviors();
        foreach ($this->_behaviors as $name => $behavior) {
            $this->detachBehavior($name);
        }
    }
*/
PHP_METHOD(Component, detachBehaviors){
	if (nii_call_class_method_0_no(getThis(), "ensureBehaviors" TSRMLS_CC) != SUCCESS) {
		return;
	}
	zval *behaviors_zv;
	behaviors_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_behaviors"), 0 TSRMLS_CC);

	if (Z_TYPE_P(behaviors_zv) ==  IS_ARRAY) {
		HashPosition    pos;
		zval **entry;
		char *string_key;
		uint string_key_len;
		ulong index;

	    zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(behaviors_zv), &pos);
	    while (zend_hash_get_current_data_ex(Z_ARRVAL_P(behaviors_zv), (void **)&entry, &pos) == SUCCESS) {
	        if (zend_hash_get_current_key_ex(Z_ARRVAL_P(behaviors_zv), &string_key, &string_key_len, &index, 0, &pos) == HASH_KEY_IS_STRING) {
	            zval *name_zv;
	            NII_NEW_STRING(name_zv, string_key);

				if (nii_call_class_method_1_no(getThis(), "detachBehavior", name_zv TSRMLS_CC) != SUCCESS) {
					NII_PTR_DTOR(name_zv);
					NII_PTR_DTOR(behaviors_zv);
					return;
				}
				NII_PTR_DTOR(name_zv);
	        }
	        zend_hash_move_forward_ex(Z_ARRVAL_P(behaviors_zv), &pos);
	    }

	    NII_PTR_DTOR(behaviors_zv);
	}
}
/* }}} */

/** {{{ public Component::ensureBehaviors()
    public function ensureBehaviors()
    {
        if ($this->_behaviors === null) {
            $this->_behaviors = [];
            foreach ($this->behaviors() as $name => $behavior) {
                $this->attachBehaviorInternal($name, $behavior);
            }
        }
    }
*/
PHP_METHOD(Component, ensureBehaviors){
	zval *behaviors_zv;
	behaviors_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_behaviors"), 0 TSRMLS_CC);
	if ( Z_TYPE_P(behaviors_zv) != IS_ARRAY) {
		NII_NEW_ARRAY(behaviors_zv);
		zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_behaviors"), behaviors_zv TSRMLS_CC);
		NII_PTR_DTOR(behaviors_zv);
	}

	zval *behaviors_retval;
	if (nii_call_class_method_0(getThis(), "behaviors", &behaviors_retval TSRMLS_CC) != SUCCESS) {
		NII_PTR_DTOR(behaviors_retval);
		return;
	}

	HashPosition pos;
	zval **entry;
	char *string_key;
	uint string_key_len;
	ulong index;

    zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(behaviors_retval), &pos);
    while (zend_hash_get_current_data_ex(Z_ARRVAL_P(behaviors_retval), (void **)&entry, &pos) == SUCCESS) {
        if (zend_hash_get_current_key_ex(Z_ARRVAL_P(behaviors_retval), &string_key, &string_key_len, &index, 0, &pos) == HASH_KEY_IS_STRING) {
            zval *name_zv;
            NII_NEW_STRING(name_zv, string_key);

			if (nii_call_class_method_2_no(getThis(), "attachBehaviorInternal", name_zv, *entry TSRMLS_CC) != SUCCESS) {
				NII_PTR_DTOR(name_zv);
				NII_PTR_DTOR(behaviors_retval);
				return;
			}
			NII_PTR_DTOR(name_zv);
        }
        zend_hash_move_forward_ex(Z_ARRVAL_P(behaviors_retval), &pos);
    }
    NII_PTR_DTOR(behaviors_retval);

}
/* }}} */

/** {{{ public Component::attachBehaviorInternal()
    private function attachBehaviorInternal($name, $behavior)
    {
        if (!($behavior instanceof Behavior)) {
            $behavior = Yii::createObject($behavior);
        }
        if (isset($this->_behaviors[$name])) {
            $this->_behaviors[$name]->detach();
        }
        $behavior->attach($this);

        return $this->_behaviors[$name] = $behavior;
    }
*/
PHP_METHOD(Component, attachBehaviorInternal){
	
	zval *behavior_zv;
	char *name;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &name, &name_len, &behavior_zv) == FAILURE) {
		return;
	}

	if (!instanceof_function(Z_OBJCE_P(behavior_zv), NII_CLASS_ENTRY(base_behavior) TSRMLS_CC))
	{
		php_printf("Yii::createObject($behavior);");
		return;
	}

	zval *behaviors_zv;
	behaviors_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_behaviors"), 0 TSRMLS_CC);
	if ( Z_TYPE_P(behaviors_zv) != IS_ARRAY) {
		NII_NEW_ARRAY(behaviors_zv);
	}

	zval *class;
	if (zend_hash_find(Z_ARRVAL_P(behaviors_zv), name, name_len, (void **)&class) == SUCCESS) {
		if (nii_call_class_method_0_no(class, "detach" TSRMLS_CC) != SUCCESS ) {
			return;
		}
	}

	if (nii_call_class_method_1_no(behavior_zv, "attach", getThis() TSRMLS_CC) != SUCCESS ) {
		return;
	}

	add_assoc_zval(behaviors_zv, name, behavior_zv);

	zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_behaviors"), behaviors_zv TSRMLS_CC);

	RETVAL_ZVAL(behaviors_zv, 1, 0);
	return;
}
/* }}} */


/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(base_component){
	PHP_ME(Component, __set, arginfo_base_component___set, ZEND_ACC_PUBLIC)
	PHP_ME(Component, __get, arginfo_base_component___get, ZEND_ACC_PUBLIC)
	PHP_ME(Component, __isset, arginfo_base_component___isset, ZEND_ACC_PUBLIC)
	PHP_ME(Component, __unset, arginfo_base_component___unset, ZEND_ACC_PUBLIC)
	PHP_ME(Component, __call, arginfo_base_component___call, ZEND_ACC_PUBLIC)
	PHP_ME(Component, hasProperty, arginfo_base_component_hasProperty, ZEND_ACC_PUBLIC)
	PHP_ME(Component, canGetProperty, arginfo_base_component_canGetProperty, ZEND_ACC_PUBLIC)
	PHP_ME(Component, canSetProperty, arginfo_base_component_canSetProperty, ZEND_ACC_PUBLIC)
	PHP_ME(Component, hasMethod, arginfo_base_component_hasMethod, ZEND_ACC_PUBLIC)
	PHP_ME(Component, behaviors, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Component, hasEventHandlers, arginfo_base_component_hasEventHandlers, ZEND_ACC_PUBLIC)
	PHP_ME(Component, on, arginfo_base_component_on, ZEND_ACC_PUBLIC)
	PHP_ME(Component, off, arginfo_base_component_off, ZEND_ACC_PUBLIC)
	PHP_ME(Component, trigger, arginfo_base_component_trigger, ZEND_ACC_PUBLIC)
	PHP_ME(Component, getBehavior, arginfo_base_component_getBehavior, ZEND_ACC_PUBLIC)
	PHP_ME(Component, getBehaviors, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Component, attachBehavior, arginfo_base_component_attachBehavior, ZEND_ACC_PUBLIC)
	PHP_ME(Component, attachBehaviors, arginfo_base_component_attachBehaviors, ZEND_ACC_PUBLIC)
	PHP_ME(Component, detachBehavior, arginfo_base_component_detachBehavior, ZEND_ACC_PUBLIC)
	PHP_ME(Component, detachBehaviors, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Component, ensureBehaviors, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Component, attachBehaviorInternal, arginfo_base_component_attachBehaviorInternal, ZEND_ACC_PUBLIC)


	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\\base\\Component
*/
NII_CLASS_FUNCTION(base_component) {

	//AUTO CALL CLASS METHOD
	NII_NS_CLASS_DECLARE_EX(nii\\base, Component, base_component, NII_CLASS_ENTRY(base_object), "nii\\base\\Object", 0);
	zend_declare_property_null(NII_CLASS_ENTRY(base_component), NII_SL("_events"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(NII_CLASS_ENTRY(base_component), NII_SL("_behaviors"), ZEND_ACC_PUBLIC TSRMLS_CC);
	return SUCCESS;
}
/* }}} */
