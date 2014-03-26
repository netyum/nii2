#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */
#include "zend_globals.h"
#include "php_nii.h"
#include "func.h"
#include "system/base/exception.h"


NII_CLASS_DECLARE_ENTRY(base_exception);

/** {{{ ARG_INFO
*/
/* }}} */


/** {{{ public Exception::getName()
*/
PHP_METHOD(Exception, getName){
    char *name = "Exception";
    RETURN_STRINGL(name, strlen(name), 0);
}
/* }}} */

/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(base_exception){
	PHP_ME(Exception, getName, NULL, ZEND_ACC_PUBLIC) //getName方法
	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\base\Exception
*/
NII_CLASS_FUNCTION(exception) {
	//AUTO CALL CLASS METHOD
	//NII_NS_CLASS_DECLARE_EX(nii\\base, Exception, base_exception, zend_exception_get_default(), "\\Exception", 0);
	NII_NS_CLASS_DECLARE(nii\\base, Exception, base_exception, 0);
	return SUCCESS;
}
/* }}} */