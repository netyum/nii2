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
#include "nii_func.h"
#include "system/base_nii.h"


NII_CLASS_DECLARE_ENTRY(base_nii);

/** {{{ ARG_INFO
*/
//getAlias
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_getAlias, 0, 0, 1)
	ZEND_ARG_INFO(0, alias)
	ZEND_ARG_INFO(0, throw_exception)
ZEND_END_ARG_INFO()

//getRootAlias
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_getRootAlias, 0, 0, 1)
    ZEND_ARG_INFO(0, alias)
ZEND_END_ARG_INFO()

//setAlias
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_setAlias, 0, 0, 2)
    ZEND_ARG_INFO(0, alias)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

//autoload
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_autoload, 0, 0, 1)
    ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

//createObject
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_createObject, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

//setLogger
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_setLogger, 0, 0, 1)
	ZEND_ARG_INFO(0, logger)
ZEND_END_ARG_INFO()

//trace
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_trace, 0, 0, 1)
	ZEND_ARG_INFO(0, message)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

//error
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_error, 0, 0, 1)
	ZEND_ARG_INFO(0, message)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

//warning
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_warning, 0, 0, 1)
	ZEND_ARG_INFO(0, message)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

//info
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_info, 0, 0, 1)
	ZEND_ARG_INFO(0, message)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

//beginProfile
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_beginProfile, 0, 0, 1)
	ZEND_ARG_INFO(0, token)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

//endProfile
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_endProfile, 0, 0, 1)
	ZEND_ARG_INFO(0, token)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

//t
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_t, 0, 0, 2)
	ZEND_ARG_INFO(0, category)
	ZEND_ARG_INFO(0, message)
    ZEND_ARG_ARRAY_INFO(0, params, 0)
	ZEND_ARG_INFO(0, language)
ZEND_END_ARG_INFO()

//configure
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_configure, 0, 0, 2)
	ZEND_ARG_INFO(0, object)
	ZEND_ARG_INFO(0, properties)
ZEND_END_ARG_INFO()

//getObjectVars
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_nii_getObjectVars, 0, 0, 1)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()
/* }}} */


/** {{{ public static BaseNii::getVersion()
    public static function getVersion()
    {
        return '2.0.0-dev';
    }
*/
PHP_METHOD(BaseNii, getVersion){

}
/* }}} */

/** {{{ public static BaseNii::getAlias()
    public static function getAlias($alias, $throwException = true)
    {
        if (strncmp($alias, '@', 1)) {
            // not an alias
            return $alias;
        }

        $pos = strpos($alias, '/');
        $root = $pos === false ? $alias : substr($alias, 0, $pos);

        if (isset(static::$aliases[$root])) {
            if (is_string(static::$aliases[$root])) {
                return $pos === false ? static::$aliases[$root] : static::$aliases[$root] . substr($alias, $pos);
            } else {
                foreach (static::$aliases[$root] as $name => $path) {
                    if (strpos($alias . '/', $name . '/') === 0) {
                        return $path . substr($alias, strlen($name));
                    }
                }
            }
        }

        if ($throwException) {
            throw new InvalidParamException("Invalid path alias: $alias");
        } else {
            return false;
        }
    }
*/
PHP_METHOD(BaseNii, getAlias){

}
/* }}} */

/** {{{ public static BaseNii::getRootAlias()
    public static function getRootAlias($alias)
    {
        $pos = strpos($alias, '/');
        $root = $pos === false ? $alias : substr($alias, 0, $pos);

        if (isset(static::$aliases[$root])) {
            if (is_string(static::$aliases[$root])) {
                return $root;
            } else {
                foreach (static::$aliases[$root] as $name => $path) {
                    if (strpos($alias . '/', $name . '/') === 0) {
                        return $name;
                    }
                }
            }
        }

        return false;
    }
*/
PHP_METHOD(BaseNii, getRootAlias){

}
/* }}} */

/** {{{ public static BaseNii::setAlias()
    public static function setAlias($alias, $path)
    {
        if (strncmp($alias, '@', 1)) {
            $alias = '@' . $alias;
        }
        $pos = strpos($alias, '/');
        $root = $pos === false ? $alias : substr($alias, 0, $pos);
        if ($path !== null) {
            $path = strncmp($path, '@', 1) ? rtrim($path, '\\/') : static::getAlias($path);
            if (!isset(static::$aliases[$root])) {
                if ($pos === false) {
                    static::$aliases[$root] = $path;
                } else {
                    static::$aliases[$root] = [$alias => $path];
                }
            } elseif (is_string(static::$aliases[$root])) {
                if ($pos === false) {
                    static::$aliases[$root] = $path;
                } else {
                    static::$aliases[$root] = [
                        $alias => $path,
                        $root => static::$aliases[$root],
                    ];
                }
            } else {
                static::$aliases[$root][$alias] = $path;
                krsort(static::$aliases[$root]);
            }
        } elseif (isset(static::$aliases[$root])) {
            if (is_array(static::$aliases[$root])) {
                unset(static::$aliases[$root][$alias]);
            } elseif ($pos === false) {
                unset(static::$aliases[$root]);
            }
        }
    }
*/
PHP_METHOD(BaseNii, setAlias){

}
/* }}} */

/** {{{ public static BaseNii::autoload()
    public static function autoload($className)
    {
        if (isset(static::$classMap[$className])) {
            $classFile = static::$classMap[$className];
            if ($classFile[0] === '@') {
                $classFile = static::getAlias($classFile);
            }
        } elseif (strpos($className, '\\') !== false) {
            $classFile = static::getAlias('@' . str_replace('\\', '/', $className) . '.php', false);
            if ($classFile === false || !is_file($classFile)) {
                return;
            }
        } else {
            return;
        }

        include($classFile);

        if (YII_DEBUG && !class_exists($className, false) && !interface_exists($className, false) && !trait_exists($className, false)) {
            throw new UnknownClassException("Unable to find '$className' in file: $classFile. Namespace missing?");
        }
    }
*/
PHP_METHOD(BaseNii, autoload){

}
/* }}} */

/** {{{ public static BaseNii::createObject()
    public static function createObject($type, array $params = [])
    {
        if (is_string($type)) {
            return static::$container->get($type, $params);
        } elseif (is_array($type) && isset($type['class'])) {
            $class = $type['class'];
            unset($type['class']);
            return static::$container->get($class, $params, $type);
        } elseif (is_callable($type, true)) {
            return call_user_func($type, $params);
        } elseif (is_array($type)) {
            throw new InvalidConfigException('Object configuration must be an array containing a "class" element.');
        } else {
            throw new InvalidConfigException("Unsupported configuration type: " . gettype($type));
        }
    }
*/
PHP_METHOD(BaseNii, createObject){
	zval *type_zv, *params_zv=NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|a", &type_zv, &params_zv) == FAILURE) {
	  return;
	}

    NII_DEBUG_PRINTF(("There is Nii::createObject\n"));
    if (!params_zv) {
        NII_NEW_ARRAY(params_zv);
    }
	if (Z_TYPE_P(params_zv) != IS_ARRAY) {
        NII_PTR_DTOR(params_zv);
		NII_NEW_ARRAY(params_zv);
	}

	if (Z_TYPE_P(type_zv) == IS_STRING) {


	}
	else if (Z_TYPE_P(type_zv) == IS_ARRAY) {
	        zval **class;
                zval *class_zv;
                if (zend_hash_find(Z_ARRVAL_P(type_zv), NII_SS("class"), (void **)&class) == SUCCESS) {
		    //char *class_name;
		    //class_name = estrndup(Z_STRVAL_P(*class), Z_STRLEN_P(*class));
		    MAKE_STD_ZVAL(class_zv);
		    ZVAL_ZVAL(class_zv, *class, 1, 0);
                    if ( zend_hash_del(Z_ARRVAL_P(type_zv), "class", sizeof("class")) == SUCCESS) {
			//NII_NEW_STRING(class_zv, class_name);
			//RETVAL_STRING(class_zv, 1);
			NII_PTR_DTOR(class_zv);
	                NII_PTR_DTOR(params_zv);
			//efree(class_name);
			return;
                    }
                    else {
                        NII_DEBUG_PRINTF(("NO\n"));
                    }
	            NII_PTR_DTOR(params_zv);
                    return;
                }
		return;
	//RETURN_TRUE;
        //return;
		/*
            zval *class_zv;
            
            NII_DEBUG_PRINTF(("There is find class ok\n"));
            //add_assoc_unset(type_zv, "class");
            if ( zend_hash_del(Z_ARRVAL_P(type_zv), "class", 6) == SUCCESS) {
                //php_var_dump(&type_zv, 1 TSRMLS_CC);

                //php_var_dump(*class, 1 TSRMLS_CC);
                return;
            }
            //Z_SET_REFCOUNT_P(*class, 0);
            //Z_UNSET_ISREF_P(*class);

            //php_var_dump(class, 1 TSRMLS_CC);
            //NII_PTR_DTOR(*class);

            //NII_DEBUG_PRINTF(("%d\n", Z_TYPE_P(*class)));
            return;
		}
		else {
			zval *message_zv;
			NII_NEW_STRING(message_zv, "Object configuration must be an array containing a \"class\" element.");
			zval *invalidconfigexception_zv;
			MAKE_STD_ZVAL(invalidconfigexception_zv);
			if (nii_new_class_instance_1(&invalidconfigexception_zv, "nii\\base\\InvalidConfigException", message_zv TSRMLS_CC) == SUCCESS) {
				zend_throw_exception_object(invalidconfigexception_zv TSRMLS_CC);
				return;
			}
			NII_PTR_DTOR(invalidconfigexception_zv);
			NII_PTR_DTOR(message_zv);
		}*/
	}
	else if (Z_TYPE_P(type_zv) == IS_CALLABLE) {
		zval *retval;
		if (nii_call_user_fun_2("call_user_func", &retval, type_zv, params_zv TSRMLS_CC) != SUCCESS) {
			NII_PTR_DTOR(params_zv);
		}
		NII_PTR_DTOR(params_zv);
		RETVAL_ZVAL(retval, 1, 0);
		NII_PTR_DTOR(retval);
		return;
	}
	else {
		zval *type_name_zv;
		if (nii_call_user_fun_1("gettype", &type_name_zv, type_zv TSRMLS_CC) != SUCCESS) {
			return;
		}
		zval *message_zv;
		NII_NEW_STRING(message_zv, "Unsupported configuration type: ");

		nii_concat(&message_zv, type_name_zv TSRMLS_CC);

		zval *invalidconfigexception_zv;
		MAKE_STD_ZVAL(invalidconfigexception_zv);
		if (nii_new_class_instance_1(&invalidconfigexception_zv, "nii\\base\\InvalidConfigException", message_zv TSRMLS_CC) == SUCCESS) {
			zend_throw_exception_object(invalidconfigexception_zv TSRMLS_CC);
			return;
		}

		NII_PTR_DTOR(invalidconfigexception_zv);
		NII_PTR_DTOR(message_zv);
	}
}
/* }}} */

/** {{{ public static BaseNii::getLogger()
    public static function getLogger()
    {
        if (self::$_logger !== null) {
            return self::$_logger;
        } else {
            return self::$_logger = static::createObject('yii\log\Logger');
        }
    }
*/
PHP_METHOD(BaseNii, getLogger){

}
/* }}} */

/** {{{ public static BaseNii::setLogger()
    public static function setLogger($logger)
    {
        self::$_logger = $logger;
    }
*/
PHP_METHOD(BaseNii, setLogger){

	
}
/* }}} */

/** {{{ public static BaseNii::trace()
    public static function trace($message, $category = 'application')
    {
        if (YII_DEBUG) {
            static::getLogger()->log($message, Logger::LEVEL_TRACE, $category);
        }
    }
*/
PHP_METHOD(BaseNii, trace){

}
/* }}} */

/** {{{ public static BaseNii::error()
    public static function error($message, $category = 'application')
    {
        static::getLogger()->log($message, Logger::LEVEL_ERROR, $category);
    }
*/
PHP_METHOD(BaseNii, error){

}
/* }}} */

/** {{{ public static BaseNii::warning()
    public static function warning($message, $category = 'application')
    {
        static::getLogger()->log($message, Logger::LEVEL_WARNING, $category);
    }
*/
PHP_METHOD(BaseNii, warning){

}
/* }}} */

/** {{{ public static BaseNii::info()
    public static function info($message, $category = 'application')
    {
        static::getLogger()->log($message, Logger::LEVEL_INFO, $category);
    }
*/
PHP_METHOD(BaseNii, info){

}
/* }}} */

/** {{{ public static BaseNii::beginProfile()
    public static function beginProfile($token, $category = 'application')
    {
        static::getLogger()->log($token, Logger::LEVEL_PROFILE_BEGIN, $category);
    }
*/
PHP_METHOD(BaseNii, beginProfile){

}
/* }}} */

/** {{{ public static BaseNii::endProfile()
    public static function endProfile($token, $category = 'application')
    {
        static::getLogger()->log($token, Logger::LEVEL_PROFILE_END, $category);
    }
*/
PHP_METHOD(BaseNii, endProfile){

}
/* }}} */

/** {{{ public static BaseNii::powered()
    public static function powered()
    {
        return 'Powered by <a href="http://www.yiiframework.com/" rel="external">Yii Framework</a>';
    }
*/
PHP_METHOD(BaseNii, powered){

}
/* }}} */

/** {{{ public static BaseNii::t()
    public static function t($category, $message, $params = [], $language = null)
    {
        if (static::$app !== null) {
            return static::$app->getI18n()->translate($category, $message, $params, $language ?: static::$app->language);
        } else {
            $p = [];
            foreach ((array) $params as $name => $value) {
                $p['{' . $name . '}'] = $value;
            }

            return ($p === []) ? $message : strtr($message, $p);
        }
    }
*/
PHP_METHOD(BaseNii, t){

}
/* }}} */

/** {{{ public static BaseNii::configure()
    public static function configure($object, $properties)
    {
        foreach ($properties as $name => $value) {
            $object->$name = $value;
        }

        return $object;
    }
*/
PHP_METHOD(BaseNii, configure){

}
/* }}} */

/** {{{ public static BaseNii::getObjectVars()
    public static function getObjectVars($object)
    {
        return get_object_vars($object);
    }
*/
PHP_METHOD(BaseNii, getObjectVars){

}
/* }}} */

/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(base_nii){
	PHP_ME(BaseNii, getVersion, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC) //静态方法
	PHP_ME(BaseNii, getAlias, arginfo_base_nii_getAlias, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, getRootAlias, arginfo_base_nii_getRootAlias, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, setAlias, arginfo_base_nii_setAlias, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, autoload, arginfo_base_nii_autoload, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, createObject, arginfo_base_nii_createObject, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, getLogger, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, setLogger, arginfo_base_nii_setLogger, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, trace, arginfo_base_nii_trace, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, error, arginfo_base_nii_error, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, warning, arginfo_base_nii_warning, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, info, arginfo_base_nii_info, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, beginProfile, arginfo_base_nii_beginProfile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, endProfile, arginfo_base_nii_endProfile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, powered, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, t, arginfo_base_nii_t, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, configure, arginfo_base_nii_configure, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(BaseNii, getObjectVars, arginfo_base_nii_getObjectVars, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
/* }}} */

/** {{{ Class BaseNii
*/
NII_CLASS_FUNCTION(base_nii) {


	//AUTO CALL CLASS METHOD
	NII_CLASS_DECLARE(BaseNii, base_nii, 0);

	zend_declare_property_null(NII_CLASS_ENTRY(base_nii), NII_SL("classMap"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(NII_CLASS_ENTRY(base_nii), NII_SL("app"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(NII_CLASS_ENTRY(base_nii), NII_SL("aliases"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(NII_CLASS_ENTRY(base_nii), NII_SL("container"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
	return SUCCESS;
}
/* }}} */
