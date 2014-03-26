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

#include "system/base/exception.h"
#include "system/base/invalidconfigexception.h"


NII_CLASS_DECLARE_ENTRY(base_invalidconfigexception);


/** {{{ string static public InvalidConfigException::className()
*/
PHP_METHOD(InvalidConfigException, getName){
	const char *name = "Invalid Configuration";
	RETVAL_STRING(name, 1);
	return;
}
/* }}} */


/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(base_invalidconfigexception){
	PHP_ME(InvalidConfigException, getName, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\\base\\InvalidConfigException
*/
NII_CLASS_FUNCTION(base_invalidconfigexception) {
	//AUTO CALL CLASS METHOD
	NII_NS_CLASS_DECLARE_EX(nii\\base, InvalidConfigException, base_invalidconfigexception, NII_CLASS_ENTRY(base_exception), "nii\\base\\Exception", 0);
	return SUCCESS;
}
/* }}} */
