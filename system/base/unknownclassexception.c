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
#include "system/base/unknownclassexception.h"


NII_CLASS_DECLARE_ENTRY(base_unknownclassexception);


/** {{{ string static public UnknownClassException::className()
*/
PHP_METHOD(UnknownClassException, getName){
	const char *name = "Unknown Class";
	RETVAL_STRING(name, 1);
	return;
}
/* }}} */


/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(base_unknownclassexception){
	PHP_ME(UnknownClassException, getName, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\\base\\UnknownClassException
*/
NII_CLASS_FUNCTION(base_unknownclassexception) {
	//AUTO CALL CLASS METHOD
	NII_NS_CLASS_DECLARE_EX(nii\\base, UnknownClassException, base_unknownclassexception, NII_CLASS_ENTRY(base_exception), "nii\\base\\Exception", 0);
	return SUCCESS;
}
/* }}} */
