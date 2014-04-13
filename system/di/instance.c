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
#include "system/di/instance.h"


NII_CLASS_DECLARE_ENTRY(di_instance);

/** {{{ ARG_INFO
*/
//__construct
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_instance___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

//of
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_instance_of, 0, 0, 1)
    ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

//ensure
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_instance_ensure, 0, 0, 1)
    ZEND_ARG_INFO(0, reference)
    ZEND_ARG_INFO(0, type)
    ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

//get
ZEND_BEGIN_ARG_INFO_EX(arginfo_di_instance_get, 0, 0, 0)
    ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ public Instance::__construct()
    protected function __construct($id)
    {
        $this->id = $id;
    }
*/
PHP_METHOD(Instance, __construct){

}
/* }}} */

/** {{{ public static Instance::of()
    public static function of($id)
    {
        return new static($id);
    }
*/
PHP_METHOD(Instance, of){

}
/* }}} */



/** {{{ public static Instance::ensure()
    public static function ensure($reference, $type = null, $container = null)
    {
        if ($reference instanceof $type) {
            return $reference;
        } elseif (empty($reference)) {
            throw new InvalidConfigException('The required component is not specified.');
        }

        if (is_string($reference)) {
            $reference = new static($reference);
        }

        if ($reference instanceof self) {
            $component = $reference->get($container);
            if ($component instanceof $type || $type === null) {
                return $component;
            } else {
                throw new InvalidConfigException('"' . $reference->id . '" refers to a ' . get_class($component) . " component. $type is expected.");
            }
        }

        $valueType = is_object($reference) ? get_class($reference) : gettype($reference);
        throw new InvalidConfigException("Invalid data type: $valueType. $type is expected.");
    }
*/
PHP_METHOD(Instance, ensure){

}
/* }}} */

/** {{{ public Instance::get()
    public function get($container = null)
    {
        if ($container) {
            return $container->get($this->id);
        }
        if (Yii::$app && Yii::$app->has($this->id)) {
            return Yii::$app->get($this->id);
        } else {
            return Yii::$container->get($this->id);
        }
    }
*/
PHP_METHOD(Instance, get){

}
/* }}} */

/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(di_instance){
	PHP_ME(Instance, __construct, arginfo_di_instance___construct, ZEND_ACC_PROTECTED) //构造方法
	PHP_ME(Instance, of, arginfo_di_instance_of, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC) //静态方法
	PHP_ME(Instance, ensure, arginfo_di_instance_ensure, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Instance, get, arginfo_di_instance_get, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\\di\\Instance
*/
NII_CLASS_FUNCTION(di_instance) {


	//AUTO CALL CLASS METHOD
	NII_NS_CLASS_DECLARE(nii\\di, Instance, di_instance, 0);
	//public $id;
	zend_declare_property_null(NII_CLASS_ENTRY(di_instance), NII_SL("id"), ZEND_ACC_PUBLIC TSRMLS_CC);
	return SUCCESS;
}
/* }}} */
