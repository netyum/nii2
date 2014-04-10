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
#include "system/nii.h"


NII_CLASS_DECLARE_ENTRY(nii);

/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(nii){
    PHP_FE_END
};
/* }}} */

/** {{{ Class Nii
*/
NII_CLASS_FUNCTION(nii) {
	//AUTO CALL CLASS METHOD
	NII_CLASS_DECLARE_EX(Nii, nii, NII_CLASS_ENTRY(base_nii), "BaseNii", 0);
    return SUCCESS;
}
/* }}} */
