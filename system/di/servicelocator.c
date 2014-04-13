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
#include "system/base/component.h"
#include "system/di/servicelocator.h"

NII_CLASS_DECLARE_ENTRY(di_servicelocator);

/** {{{ ARG_INFO
*/
//__get
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_servicelocator___get, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//__isset
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_servicelocator___isset, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

//has
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_servicelocator_has, 0, 0, 1)
    ZEND_ARG_INFO(0, id)
    ZEND_ARG_INFO(0, checkInstance)
ZEND_END_ARG_INFO()

//get
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_servicelocator_get, 0, 0, 1)
    ZEND_ARG_INFO(0, id)
    ZEND_ARG_INFO(0, throwException)
ZEND_END_ARG_INFO()

//set
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_servicelocator_set, 0, 0, 2)
	ZEND_ARG_INFO(0, id)
    ZEND_ARG_INFO(0, definition)
ZEND_END_ARG_INFO()

//clear
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_servicelocator_clear, 0, 0, 1)
	ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

//getComponents
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_servicelocator_getComponents, 0, 0, 0)
	ZEND_ARG_INFO(0, returnDefinitions)
ZEND_END_ARG_INFO()

//setComponents
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_servicelocator_setComponents, 0, 0, 1)
	ZEND_ARG_INFO(0, components)
ZEND_END_ARG_INFO()
/* }}} */


/** {{{ public ServiceLocator::__get()
    public function __get($name)
    {
        if ($this->has($name)) {
            return $this->get($name);
        } else {
            return parent::__get($name);
        }
    }
*/
PHP_METHOD(ServiceLocator, __get){

}
/* }}} */

/** {{{ public ServiceLocator::__isset()
    public function __isset($name)
    {
        if ($this->has($name, true)) {
            return true;
        } else {
            return parent::__isset($name);
        }
    }
*/
PHP_METHOD(ServiceLocator, __isset){

}
/* }}} */

/** {{{ public ServiceLocator::has()
    public function has($id, $checkInstance = false)
    {
        return $checkInstance ? isset($this->_components[$id]) : isset($this->_definitions[$id]);
    }
*/
PHP_METHOD(ServiceLocator, has){

}
/* }}} */

/** {{{ public ServiceLocator::get()
    public function get($id, $throwException = true)
    {
        if (isset($this->_components[$id])) {
            return $this->_components[$id];
        }

        if (isset($this->_definitions[$id])) {
            $definition = $this->_definitions[$id];
            if (is_object($definition) && !$definition instanceof Closure) {
                return $this->_components[$id] = $definition;
            } else {
                return $this->_components[$id] = Yii::createObject($definition);
            }
        } elseif ($throwException) {
            throw new InvalidConfigException("Unknown component ID: $id");
        } else {
            return null;
        }
    }
*/
PHP_METHOD(ServiceLocator, get){

}
/* }}} */

/** {{{ public ServiceLocator::set()
    public function set($id, $definition)
    {
        if ($definition === null) {
            unset($this->_components[$id], $this->_definitions[$id]);
            return;
        }

        if (is_object($definition) || is_callable($definition, true)) {
            // an object, a class name, or a PHP callable
            $this->_definitions[$id] = $definition;
        } elseif (is_array($definition)) {
            // a configuration array
            if (isset($definition['class'])) {
                $this->_definitions[$id] = $definition;
            } else {
                throw new InvalidConfigException("The configuration for the \"$id\" component must contain a \"class\" element.");
            }
        } else {
            throw new InvalidConfigException("Unexpected configuration type for the \"$id\" component: " . gettype($definition));
        }
    }
*/
PHP_METHOD(ServiceLocator, set){

}
/* }}} */

/** {{{ public ServiceLocator::clear()
    public function clear($id)
    {
        unset($this->_definitions[$id], $this->_components[$id]);
    }
*/
PHP_METHOD(ServiceLocator, clear){

}
/* }}} */

/** {{{ public ServiceLocator::getComponents()
    public function getComponents($returnDefinitions = true)
    {
        return $returnDefinitions ? $this->_definitions : $this->_components;
    }
*/
PHP_METHOD(ServiceLocator, getComponents){

}
/* }}} */

/** {{{ public ServiceLocator::setComponents()
    public function setComponents($components)
    {
        foreach ($components as $id => $component) {
            $this->set($id, $component);
        }
    }
*/
PHP_METHOD(ServiceLocator, setComponents){

}
/* }}} */


/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(di_servicelocator){
	PHP_ME(ServiceLocator, __get, arginfo_di_servicelocator___get, ZEND_ACC_PUBLIC)
	PHP_ME(ServiceLocator, __isset, arginfo_di_servicelocator___isset, ZEND_ACC_PUBLIC)
	PHP_ME(ServiceLocator, has, arginfo_di_servicelocator_has, ZEND_ACC_PUBLIC)
    PHP_ME(ServiceLocator, get, arginfo_di_servicelocator_get, ZEND_ACC_PUBLIC)
    PHP_ME(ServiceLocator, set, arginfo_di_servicelocator_set, ZEND_ACC_PUBLIC)
	PHP_ME(ServiceLocator, clear, arginfo_di_servicelocator_clear, ZEND_ACC_PUBLIC)
	PHP_ME(ServiceLocator, getComponents, arginfo_di_servicelocator_getComponents, ZEND_ACC_PUBLIC)
	PHP_ME(ServiceLocator, setComponents, arginfo_di_servicelocator_setComponents, ZEND_ACC_PUBLIC)

	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\\di\\ServiceLocator
*/
NII_CLASS_FUNCTION(di_servicelocator) {

	//AUTO CALL CLASS METHOD
	NII_NS_CLASS_DECLARE_EX(nii\\di, ServiceLocator, di_servicelocator, NII_CLASS_ENTRY(base_component), "nii\\base\\Component", 0);
	//private $_components = [];
	zend_declare_property_null(NII_CLASS_ENTRY(di_servicelocator), NII_SL("_components"), ZEND_ACC_PRIVATE TSRMLS_CC);
	//private $_definitions = [];
	zend_declare_property_null(NII_CLASS_ENTRY(di_servicelocator), NII_SL("_definitions"), ZEND_ACC_PRIVATE TSRMLS_CC);
    return SUCCESS;
}
/* }}} */
