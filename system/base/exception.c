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

#include "zend_exceptions.h"
#include "system/base/exception.h"


NII_CLASS_DECLARE_ENTRY(base_exception);


/** {{{ string static public Exception::className()
*/
PHP_METHOD(Exception, getName){
	const char *name = "Exception";
	RETVAL_STRING(name, 1);
	return;
}
/* }}} */


/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(base_exception){
	PHP_ME(Exception, getName, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\\base\\Exception
*/
NII_CLASS_FUNCTION(base_exception) {


	//AUTO CALL CLASS METHOD
	NII_NS_CLASS_DECLARE_EX(nii\\base, Exception, base_exception, zend_exception_get_default(TSRMLS_C), "\\Exception", 0);
	return SUCCESS;
}
/* }}} */
