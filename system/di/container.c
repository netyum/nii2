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
#include "Zend/zend_exceptions.h"
#include "php_nii.h"
#include "func.h"
#include "nii_func.h"
#include "system/base_nii.h"
#include "system/base/component.h"
#include "system/di/container.h"

NII_CLASS_DECLARE_ENTRY(di_container);

/** {{{ ARG_INFO
*/
//get
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_get, 0, 0, 1)
    ZEND_ARG_INFO(0, class)
    ZEND_ARG_ARRAY_INFO(0, params, 0)
    ZEND_ARG_ARRAY_INFO(0, config, 0)
ZEND_END_ARG_INFO()

//set
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_set, 0, 0, 1)
    ZEND_ARG_INFO(0, class)
	ZEND_ARG_INFO(0, definition)
	ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

//setSingleton
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_setSingleton, 0, 0, 1)
    ZEND_ARG_INFO(0, class)
	ZEND_ARG_ARRAY_INFO(0, definition, 0)
	ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

//has
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_has, 0, 0, 1)
    ZEND_ARG_INFO(0, class)
ZEND_END_ARG_INFO()

//hasSingleton
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_hasSingleton, 0, 0, 1)
    ZEND_ARG_INFO(0, class)
    ZEND_ARG_INFO(0, checkInstance)
ZEND_END_ARG_INFO()

//clear
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_clear, 0, 0, 1)
	ZEND_ARG_INFO(0, class)
ZEND_END_ARG_INFO()

//normalizeDefinition
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_normalizeDefinition, 0, 0, 2)
	ZEND_ARG_INFO(0, class)
	ZEND_ARG_INFO(0, definition)
ZEND_END_ARG_INFO()

//build
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_build, 0, 0, 3)
	ZEND_ARG_INFO(0, class)
	ZEND_ARG_INFO(0, params)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

//mergeParams
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_mergeParams, 0, 0, 2)
	ZEND_ARG_INFO(0, class)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

//getDependencies
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_getDependencies, 0, 0, 1)
	ZEND_ARG_INFO(0, class)
ZEND_END_ARG_INFO()

//resolveDependencies
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_container_resolveDependencies, 0, 0, 1)
	ZEND_ARG_INFO(0, dependencies)
	ZEND_ARG_INFO(0, reflection)
ZEND_END_ARG_INFO()
/* }}} */


/** {{{ public Container::get()
	public function get($class, $params = [], $config = [])
    {
        if (isset($this->_singletons[$class])) {
            // singleton
            return $this->_singletons[$class];
        } elseif (!isset($this->_definitions[$class])) {
            return $this->build($class, $params, $config);
        }

        $definition = $this->_definitions[$class];

        if (is_callable($definition, true)) {
            $params = $this->resolveDependencies($this->mergeParams($class, $params));
            $object = call_user_func($definition, $this, $params, $config);
        } elseif (is_array($definition)) {
            $concrete = $definition['class'];
            unset($definition['class']);

            $config = array_merge($definition, $config);
            $params = $this->mergeParams($class, $params);

            if ($concrete === $class) {
                $object = $this->build($class, $params, $config);
            } else {
                $object = $this->get($concrete, $params, $config);
            }
        } elseif (is_object($definition)) {
            return $this->_singletons[$class] = $definition;
        } else {
            throw new InvalidConfigException("Unexpected object definition type: " . gettype($definition));
        }

        if (array_key_exists($class, $this->_singletons)) {
            // singleton
            $this->_singletons[$class] = $object;
        }

        return $object;
    }
*/
PHP_METHOD(Container, get){
    zval *params = NULL, *config = NULL;
    char *class;
    int class_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|aa", &class, &class_len, &params, &config) == FAILURE) {
        return;
    }

    zval *singletons_zv;
    singletons_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_singletons"), 0 TSRMLS_CC);

    if (Z_TYPE_P(singletons_zv) == IS_ARRAY) {
        zval **singleton;
        if (zend_hash_find(Z_ARRVAL_P(singletons_zv), NII_SS(class), (void **)&singleton) == SUCCESS) {
            RETVAL_ZVAL(*singleton, 1, 0);
            return;
        }
    }

    NII_NEW_ARRAY(singletons_zv);
    zval *class_zv;
    NII_NEW_STRING(class_zv, class);

    if (!params) NII_NEW_ARRAY(params);
    if (!config) NII_NEW_ARRAY(config);

    zval *definitions_zv;
    definitions_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_definitions"), 0 TSRMLS_CC);

    if (Z_TYPE_P(definitions_zv) != IS_ARRAY) {
        NII_NEW_ARRAY(definitions_zv);
    }

    if ( zend_hash_exists(Z_ARRVAL_P(definitions_zv), NII_SS(class)) == 1 ) {
        zval *retval;
        if (nii_call_class_method_3(getThis(), "build", &retval, class_zv, params, config TSRMLS_CC) == SUCCESS) {
            RETVAL_ZVAL(retval, 1, 0);
            NII_PTR_DTOR(retval);
            NII_PTR_DTOR(class_zv);
            NII_PTR_DTOR(params);
            NII_PTR_DTOR(config);
            return;
        }
        NII_PTR_DTOR(retval);
        NII_PTR_DTOR(class_zv);
    }
    
    zval **definition;
    if (zend_hash_find(Z_ARRVAL_P(definitions_zv), NII_SS(class), (void **)&definition) != FAILURE) {
        return;
    }

    zval *mergeparams_retval, *params_retval, *object_retval;
    if (Z_TYPE_P(*definition) == IS_CALLABLE) {
        

        if (nii_call_class_method_2(getThis(), "mergeParams", &mergeparams_retval, class_zv, params TSRMLS_CC) != SUCCESS) {
            return;
        }

        if (nii_call_class_method_1(getThis(), "resolveDependencies", &params_retval, mergeparams_retval TSRMLS_CC) != SUCCESS) {
            return;
        }

        // $object = call_user_func($definition, $this, $params, $config);
        if (nii_call_user_fun_4("call_user_func", &object_retval, *definition, getThis(), params_retval, config TSRMLS_CC) != SUCCESS) {
            return;
        }
    }
    else if (Z_TYPE_P(*definition) == IS_ARRAY) {
        zval **concrete_retval;
        zval *concrete;
        if (zend_hash_find(Z_ARRVAL_P(*definition), NII_SS("class"), (void **)&concrete_retval) != SUCCESS) {
            return;
        }

        MAKE_STD_ZVAL(concrete);
        ZVAL_ZVAL(concrete, *concrete_retval, 1, 0);
        if ( zend_hash_del(Z_ARRVAL_P(*definition), "class", sizeof("class")) != SUCCESS) {
            return;
        }
        
        zval *config_retval;
        if (nii_call_user_fun_2("array_merge", &config_retval, *definition, config TSRMLS_CC) != SUCCESS) {
            return;
        }

        if (nii_call_class_method_2(getThis(), "mergeParams", &params_retval, class_zv, params TSRMLS_CC) != SUCCESS) {
            return;
        }

        zval compare_result;
        is_identical_function(&compare_result, concrete, class_zv TSRMLS_CC);
        if (Z_LVAL(compare_result)) {
            if (nii_call_class_method_3(getThis(), "build", &object_retval, class_zv, params_retval, config_retval TSRMLS_CC) != SUCCESS) {
                return;
            }
        }
        else {
            if (nii_call_class_method_3(getThis(), "get", &object_retval, concrete, params_retval, config_retval TSRMLS_CC) != SUCCESS) {
                return;
            }
        }
    }
    else if (Z_TYPE_P(*definition) == IS_OBJECT) {
        add_assoc_zval(singletons_zv, class, *definition);
        zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_singletons"), singletons_zv TSRMLS_CC);
        RETVAL_ZVAL(singletons_zv, 1, 0);
        return;
    } else {
        zval *type_name_zv;
        if (nii_call_user_fun_1("gettype", &type_name_zv, *definition TSRMLS_CC) != SUCCESS) {
            return;
        }
        zval *message_zv;
        NII_NEW_STRING(message_zv, "Unexpected object definition type: ");

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

    zval *key_exists_retval;
    if (nii_call_user_fun_2("array_key_exists", &key_exists_retval, class_zv, singletons_zv TSRMLS_CC) != SUCCESS) {
        return;
    }

    if (Z_TYPE_P(key_exists_retval) == IS_BOOL && Z_BVAL_P(key_exists_retval) == 1) {
        add_assoc_zval(singletons_zv, class, object_retval);
        zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_singletons"), singletons_zv TSRMLS_CC);
    }

    RETVAL_ZVAL(object_retval, 1, 0);
    return;
}
/* }}} */

/** {{{ public Container::set()
    public function set($class, $definition = [], array $params = [])
    {
        $this->_definitions[$class] = $this->normalizeDefinition($class, $definition);
        $this->_params[$class] = $params;
        unset($this->_singletons[$class]);
        return $this;
    }
*/
PHP_METHOD(Container, set){
    zval *params = NULL, *definition = NULL;
    char *class;
    int class_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|za", &class, &class_len, &definition, &params) == FAILURE) {
        return;
    }

    zval *singletons_zv;
    singletons_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_singletons"), 0 TSRMLS_CC);

    if (Z_TYPE_P(singletons_zv) != IS_ARRAY) {
        NII_NEW_ARRAY(singletons_zv);
    }

    zval *class_zv;
    NII_NEW_STRING(class_zv, class);

    if (params == NULL) {
        NII_NEW_ARRAY(params);
    }

    if (definition == NULL) {
        NII_NEW_ARRAY(definition);
    }

    zval *definitions_zv;
    definitions_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_definitions"), 0 TSRMLS_CC);

    if (Z_TYPE_P(definitions_zv) != IS_ARRAY) {
        NII_NEW_ARRAY(definitions_zv);
    }

    zval *normalizedefinition_retval;

    if (nii_call_class_method_2(getThis(), "normalizeDefinition", &normalizedefinition_retval, class_zv, definition TSRMLS_CC) != SUCCESS) {
        return;
    }
    
    NII_PTR_DTOR(class_zv);

    add_assoc_zval(definitions_zv, class, normalizedefinition_retval);
    zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_definitions"), definitions_zv TSRMLS_CC);
    NII_PTR_DTOR(definitions_zv);
    NII_PTR_DTOR(normalizedefinition_retval);

    zval *_params;
    _params = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_params"), 0 TSRMLS_CC);
    if (Z_TYPE_P(_params) != IS_ARRAY) {
        NII_NEW_ARRAY(_params);
    }
    add_assoc_zval(_params, class, params);
    zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_params"), _params TSRMLS_CC);
    NII_PTR_DTOR(_params);

    if ( zend_hash_exists(Z_ARRVAL_P(singletons_zv), NII_SS(class)) == 1) {
        if ( zend_hash_del(Z_ARRVAL_P(singletons_zv), class, class_len+1) != SUCCESS) {
            return;
        }
        zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_singletons"), singletons_zv TSRMLS_CC);
    }
    NII_PTR_DTOR(singletons_zv);

    NII_PTR_DTOR(params);
    NII_PTR_DTOR(definition);
    RETVAL_ZVAL(getThis(), 1, 0);
    return;
}
/* }}} */

/** {{{ public Container::setSingleton()
    public function setSingleton($class, $definition = [], array $params = [])
    {
        $this->_definitions[$class] = $this->normalizeDefinition($class, $definition);;
        $this->_params[$class] = $params;
        $this->_singletons[$class] = null;
        return $this;
    }
*/
PHP_METHOD(Container, setSingleton){


}
/* }}} */

/** {{{ public Container::has()
    public function has($class)
    {
        return isset($this->_singletons[$class]);
    }
*/
PHP_METHOD(Container, has){
    char *class;
    int class_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &class, &class_len) == FAILURE) {
        return;
    }

    zval *singletons_zv;
    singletons_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_singletons"), 0 TSRMLS_CC);

    if (Z_TYPE_P(singletons_zv) != IS_ARRAY) {
        NII_NEW_ARRAY(singletons_zv);
    }

    if (zend_hash_exists(Z_ARRVAL_P(singletons_zv), NII_SS(class)) == 0) {
        RETVAL_TRUE;
    }
    else {
        RETVAL_FALSE;
    }

    NII_PTR_DTOR(singletons_zv);
    return;
}
/* }}} */

/** {{{ public Container::hasSingleton()
    public function hasSingleton($class, $checkInstance = false)
    {
        return $checkInstance ? isset($this->_singletons[$class]) : array_key_exists($class, $this->_singletons);
    }
*/
PHP_METHOD(Container, hasSingleton){

}
/* }}} */

/** {{{ public Container::clear()
    public function clear($class)
    {
        unset($this->_definitions[$class], $this->_singletons[$class]);
    }
*/
PHP_METHOD(Container, clear){
    char *class;
    int class_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &class, &class_len) == FAILURE) {
        return;
    }

    zval *singletons_zv;
    singletons_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_singletons"), 0 TSRMLS_CC);

    zval *definitions_zv;
    definitions_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_definitions"), 0 TSRMLS_CC);

    if (Z_TYPE_P(singletons_zv) != IS_ARRAY) {
        NII_NEW_ARRAY(singletons_zv)
    }

    if (Z_TYPE_P(definitions_zv) != IS_ARRAY) {
        NII_NEW_ARRAY(definitions_zv)
    }

    if ( zend_hash_del(Z_ARRVAL_P(singletons_zv), "class", sizeof("class")) != SUCCESS) {
        NII_PTR_DTOR(singletons_zv);
        NII_PTR_DTOR(definitions_zv);
        return;
    }

    if ( zend_hash_del(Z_ARRVAL_P(definitions_zv), "class", sizeof("class")) != SUCCESS) {
        NII_PTR_DTOR(singletons_zv);
        NII_PTR_DTOR(definitions_zv);
        return;
    }

    zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_singletons"), singletons_zv TSRMLS_CC);
    zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_definitions"), definitions_zv TSRMLS_CC);

    NII_PTR_DTOR(singletons_zv);
    NII_PTR_DTOR(definitions_zv);
    return;
}
/* }}} */

/** {{{ public Container::normalizeDefinition()
    protected function normalizeDefinition($class, $definition)
    {
        if (empty($definition)) {
            return ['class' => $class];
        } elseif (is_string($definition)) {
            return ['class' => $definition];
        } elseif (is_callable($definition, true) || is_object($definition)) {
            return $definition;
        } elseif (is_array($definition)) {
            if (!isset($definition['class'])) {
                if (strpos($class, '\\') !== false) {
                    $definition['class'] = $class;
                } else {
                    throw new InvalidConfigException("A class definition requires a \"class\" member.");
                }
            }
            return $definition;
        } else {
            throw new InvalidConfigException("Unsupported definition type for \"$class\": " . gettype($definition));
        }
    }
*/
PHP_METHOD(Container, normalizeDefinition){
    char *class;
    int class_len;
    zval *definition;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &class, &class_len, &definition) == FAILURE) {
        return;
    }

    if (nii_isnot_empty(definition) == 0) {
        NII_DEBUG_PRINTF(("1 in normalizeDefinition\n"));
        array_init(return_value);
        add_assoc_stringl(return_value, "class", class, class_len, 1);
        return;
    }
    else if (Z_TYPE_P(definition) == IS_STRING) {
        //NII_DEBUG_PRINTF(("2 in normalizeDefinition\n"));
        array_init(return_value);
        add_assoc_zval(return_value, "class", definition);
        return;
    }
    else if (Z_TYPE_P(definition) == IS_CALLABLE || Z_TYPE_P(definition) == IS_OBJECT) {
        RETVAL_ZVAL(definition, 1, 0);
        NII_DEBUG_PRINTF(("3 in normalizeDefinition\n"));
        return;
    }
    else if (Z_TYPE_P(definition) == IS_ARRAY) {
        NII_DEBUG_PRINTF(("4 in normalizeDefinition\n"));
        if (zend_hash_exists(Z_ARRVAL_P(definition), NII_SS("class")) == 0) {
            //if (strpos($class, '\\') !== false) {
            zval *find_zv, *class_zv, *strpos_retval;
            NII_NEW_STRING(find_zv, "\\");
            NII_NEW_STRING(class_zv, class);
            if (nii_call_user_fun_2("strpos", &strpos_retval, class_zv, find_zv TSRMLS_CC) != SUCCESS) {
                return;
            }
            NII_PTR_DTOR(class_zv);
            NII_PTR_DTOR(find_zv);
            if (Z_TYPE_P(strpos_retval) == IS_LONG && Z_LVAL_P(strpos_retval) == 0) {
                add_assoc_stringl(definition, "class", class, class_len, 1);
                RETVAL_ZVAL(definition, 1, 0);
                NII_PTR_DTOR(strpos_retval);
                return;
            }
            else {
                NII_PTR_DTOR(strpos_retval);

                zval *message_zv;
                NII_NEW_STRING(message_zv, "A class definition requires a \"class\" member.");

                zval *invalidconfigexception_zv;
                MAKE_STD_ZVAL(invalidconfigexception_zv);
                if (nii_new_class_instance_1(&invalidconfigexception_zv, "nii\\base\\InvalidConfigException", message_zv TSRMLS_CC) == SUCCESS) {
                    NII_DEBUG_PRINTF(("33333333333333333\n"));
                    php_var_dump(&invalidconfigexception_zv, 1 TSRMLS_CC);
                    zend_throw_exception_object(invalidconfigexception_zv TSRMLS_CC);
                }
                return;
            }
        }
        RETVAL_ZVAL(definition, 1, 0);
        return;
    }
    else {
        NII_DEBUG_PRINTF(("5 in normalizeDefinition\n"));
        zval *type_name_zv;
        if (nii_call_user_fun_1("gettype", &type_name_zv, definition TSRMLS_CC) != SUCCESS) {
            return;
        }

        char *message;
        int message_len;
        zval *message_zv;

        message_len = spprintf(&message, 0, "Unsupported definition type for \"%s\": ", class);
        NII_NEW_STRING(message_zv, message);
        nii_concat(&message_zv, type_name_zv TSRMLS_CC);

        zval *invalidconfigexception_zv;
        MAKE_STD_ZVAL(invalidconfigexception_zv);
        if (nii_new_class_instance_1(&invalidconfigexception_zv, "nii\\base\\InvalidConfigException", message_zv TSRMLS_CC) == SUCCESS) {
            zend_throw_exception_object(invalidconfigexception_zv TSRMLS_CC);
            return;
        }

        NII_PTR_DTOR(invalidconfigexception_zv);
        NII_PTR_DTOR(message_zv);
        efree(message);
    }

}
/* }}} */

/** {{{ public Container::getDefinitions()
    public function getDefinitions()
    {
        return $this->_definitions;
    }
*/
PHP_METHOD(Container, getDefinitions){

}
/* }}} */

/** {{{ public Container::build()
    protected function build($class, $params, $config)
    {
        list ($reflection, $dependencies) = $this->getDependencies($class);

        foreach ($params as $index => $param) {
            $dependencies[$index] = $param;
        }

        $dependencies = $this->resolveDependencies($dependencies, $reflection);

        if (!empty($config) && !empty($dependencies) && is_a($class, 'yii\base\Object', true)) {
            // set $config as the last parameter (existing one will be overwritten)
            $dependencies[count($dependencies) - 1] = $config;
            return $reflection->newInstanceArgs($dependencies);
        } else {
            $object = $reflection->newInstanceArgs($dependencies);
            foreach ($config as $name => $value) {
                $object->$name = $value;
            }
            return $object;
        }
    }
*/
PHP_METHOD(Container, build){

}
/* }}} */

/** {{{ public Container::mergeParams()
    protected function mergeParams($class, $params)
    {
        if (empty($this->_params[$class])) {
            return $params;
        } elseif (empty($params)) {
            return $this->_params[$class];
        } else {
            $ps = $this->_params[$class];
            foreach ($params as $index => $value) {
                $ps[$index] = $value;
            }
            return $ps;
        }
    }
*/
PHP_METHOD(Container, mergeParams){
    zval *params;
    char *class;
    int class_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &class, &class_len, &params) == FAILURE) {
        return;
    }


    zval *params_zv;
    params_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("_params"), 0 TSRMLS_CC);

    if (Z_TYPE_P(params_zv) != IS_ARRAY) {
        NII_NEW_ARRAY(params_zv);
    }

    if (zend_hash_exists(Z_ARRVAL_P(params_zv), NII_SS(class)) == 0) {
        RETURN_ZVAL(params, 1, 0);
    }
    else if (nii_isnot_empty(params)==0) {
        zval **params_retval;
        if ( zend_hash_find(Z_ARRVAL_P(params_zv), NII_SS(class), (void **)&params_retval) == SUCCESS) {
            RETVAL_ZVAL(*params_retval, 1, 0);
            return;
        }
        NII_PTR_DTOR(*params_retval);
    }
    else {
        zval **params_retval;
        if ( zend_hash_find(Z_ARRVAL_P(params_zv), NII_SS(class), (void **)&params_retval) != SUCCESS) {
            return;
        }

        if (Z_TYPE_P(params) != IS_ARRAY) return;
        if (Z_TYPE_P(*params_retval) != IS_ARRAY) return;

        HashPosition    pos;
        zval **entry;
        char *string_key;
        uint string_key_len;
        ulong index;

        zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(params), &pos);
        while (zend_hash_get_current_data_ex(Z_ARRVAL_P(params), (void **)&entry, &pos) == SUCCESS) {
            if (zend_hash_get_current_key_ex(Z_ARRVAL_P(params), &string_key, &string_key_len, &index, 0, &pos) == HASH_KEY_IS_STRING) {
                add_assoc_zval(*params_retval, string_key, *entry);
            }
            else {
                add_index_zval(*params_retval, index, *entry);
            }
            zend_hash_move_forward_ex(Z_ARRVAL_P(params), &pos);
        }

        RETVAL_ZVAL(*params_retval, 1, 0);
        return;
    }
}
/* }}} */

/** {{{ public Container::getDependencies()
    protected function getDependencies($class)
    {
        if (isset($this->_reflections[$class])) {
            return [$this->_reflections[$class], $this->_dependencies[$class]];
        }

        $dependencies = [];
        $reflection = new ReflectionClass($class);

        $constructor = $reflection->getConstructor();
        if ($constructor !== null) {
            foreach ($constructor->getParameters() as $param) {
                if ($param->isDefaultValueAvailable()) {
                    $dependencies[] = $param->getDefaultValue();
                } else {
                    $c = $param->getClass();
                    $dependencies[] = Instance::of($c === null ? null : $c->getName());
                }
            }
        }

        $this->_reflections[$class] = $reflection;
        $this->_dependencies[$class] = $dependencies;

        return [$reflection, $dependencies];
    }
*/
PHP_METHOD(Container, getDependencies){
}
/* }}} */

/** {{{ public Container::resolveDependencies()
    protected function resolveDependencies($dependencies, $reflection = null)
    {
        foreach ($dependencies as $index => $dependency) {
            if ($dependency instanceof Instance) {
                if ($dependency->id !== null) {
                    $dependencies[$index] = $this->get($dependency->id);
                } elseif ($reflection !== null) {
                    $name = $reflection->getConstructor()->getParameters()[$index]->getName();
                    $class = $reflection->getName();
                    throw new InvalidConfigException("Missing required parameter \"$name\" when instantiating \"$class\".");
                }
            }
        }
        return $dependencies;
    }
*/
PHP_METHOD(Container, resolveDependencies){

}
/* }}} */


/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(di_container){
	PHP_ME(Container, get, arginfo_di_container_get, ZEND_ACC_PUBLIC)
	PHP_ME(Container, set, arginfo_di_container_set, ZEND_ACC_PUBLIC)
	PHP_ME(Container, setSingleton, arginfo_di_container_setSingleton, ZEND_ACC_PUBLIC)
	PHP_ME(Container, has, arginfo_di_container_has, ZEND_ACC_PUBLIC)
	PHP_ME(Container, hasSingleton, arginfo_di_container_hasSingleton, ZEND_ACC_PUBLIC)
	PHP_ME(Container, clear, arginfo_di_container_clear, ZEND_ACC_PUBLIC)
	PHP_ME(Container, normalizeDefinition, arginfo_di_container_normalizeDefinition, ZEND_ACC_PROTECTED)
	PHP_ME(Container, getDefinitions, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Container, build, arginfo_di_container_build, ZEND_ACC_PROTECTED)
	PHP_ME(Container, mergeParams, arginfo_di_container_mergeParams, ZEND_ACC_PROTECTED)
	PHP_ME(Container, getDependencies, arginfo_di_container_getDependencies, ZEND_ACC_PROTECTED)
	PHP_ME(Container, resolveDependencies, arginfo_di_container_resolveDependencies, ZEND_ACC_PROTECTED)

	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\\di\\Container
*/
NII_CLASS_FUNCTION(di_container) {

	//AUTO CALL CLASS METHOD
	NII_NS_CLASS_DECLARE_EX(nii\\di, Container, di_container, NII_CLASS_ENTRY(base_component), "nii\\base\\Component", 0);
	//private $_singletons = [];
	zend_declare_property_null(NII_CLASS_ENTRY(di_container), NII_SL("_singletons"), ZEND_ACC_PRIVATE TSRMLS_CC);
	//private $_definitions = [];
	zend_declare_property_null(NII_CLASS_ENTRY(di_container), NII_SL("_definitions"), ZEND_ACC_PRIVATE TSRMLS_CC);
	//private $_params = [];
	zend_declare_property_null(NII_CLASS_ENTRY(di_container), NII_SL("_params"), ZEND_ACC_PRIVATE TSRMLS_CC);
	//private $_reflections = [];
	zend_declare_property_null(NII_CLASS_ENTRY(di_container), NII_SL("_reflections"), ZEND_ACC_PRIVATE TSRMLS_CC);
	//private $_dependencies = [];
	zend_declare_property_null(NII_CLASS_ENTRY(di_container), NII_SL("_dependencies"), ZEND_ACC_PRIVATE TSRMLS_CC);
	return SUCCESS;
}
/* }}} */
