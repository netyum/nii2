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
#include "system/base/object.h"
#include "system/base/component.h"
#include "system/base/behavior.h"


NII_CLASS_DECLARE_ENTRY(base_behavior);

/** {{{ ARG_INFO
*/
//attach
ZEND_BEGIN_ARG_INFO_EX(arginfo_base_behavior_attach, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, owner, nii\\base\\Component, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ public Behavior::events()
	public function events()
    {
        return [];
    }
*/
PHP_METHOD(Behavior, events){
	zval *events_zv;
	NII_NEW_ARRAY(events_zv);
	RETVAL_ZVAL(events_zv, 1, 0);
	NII_PTR_DTOR(events_zv);
	return;
}
/* }}} */

/** {{{ public Behavior::attach()
    public function attach($owner)
    {
        $this->owner = $owner;
        foreach ($this->events() as $event => $handler) {
            $owner->on($event, is_string($handler) ? [$this, $handler] : $handler);
        }
    }
*/
PHP_METHOD(Behavior, attach) {
	zval *owner_zv;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &owner_zv) == FAILURE) {
	  return;
	}
	zend_update_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("owner"), owner_zv TSRMLS_CC);

	zval *events_zv;
	if (nii_call_class_method_0(getThis(), "events", &events_zv TSRMLS_CC) != SUCCESS) {
		return;
	}

	if (Z_TYPE_P(events_zv) != IS_ARRAY) return;

	HashPosition    pos;
	zval **entry;
	char *string_key;
	uint string_key_len;
	ulong index;

    zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(events_zv), &pos);
    while (zend_hash_get_current_data_ex(Z_ARRVAL_P(events_zv), (void **)&entry, &pos) == SUCCESS) {
        if (zend_hash_get_current_key_ex(Z_ARRVAL_P(events_zv), &string_key, &string_key_len, &index, 0, &pos) == HASH_KEY_IS_STRING) {
        	zval *event_zv;
	       	NII_NEW_STRING(event_zv, string_key);

        	if ((*entry)->type != IS_STRING) {
        		zval *tmp_array_zv;
        		NII_NEW_ARRAY(tmp_array_zv);
        		add_next_index_zval(tmp_array_zv, getThis());
        		add_next_index_zval(tmp_array_zv, *entry);

				if (nii_call_class_method_2_no(owner_zv, "on", event_zv, tmp_array_zv TSRMLS_CC) != SUCCESS) {
					NII_PTR_DTOR(tmp_array_zv);
					return;
				}
				NII_PTR_DTOR(tmp_array_zv);
           	}
           	else {
				if (nii_call_class_method_2_no(owner_zv, "on", event_zv, *entry TSRMLS_CC) != SUCCESS) {
					NII_PTR_DTOR(*entry);
					return;
				}
				NII_PTR_DTOR(*entry);
           	}

           	NII_PTR_DTOR(event_zv);
        }
        zend_hash_move_forward_ex(Z_ARRVAL_P(events_zv), &pos);
    }
}
/* }}} */

/** {{{ public Behavior::detach()
    public function detach()
    {
        if ($this->owner) {
            foreach ($this->events() as $event => $handler) {
                $this->owner->off($event, is_string($handler) ? [$this, $handler] : $handler);
            }
            $this->owner = null;
        }
    }
*/
PHP_METHOD(Behavior, detach){
	zval *owner_zv = NULL;
	owner_zv = zend_read_property(Z_OBJCE_P(getThis()), getThis(), NII_SL("owner"), 0 TSRMLS_CC);
	if (Z_TYPE_P(owner_zv) == IS_OBJECT) {
		zval *events_zv;
		if (nii_call_class_method_0(getThis(), "events", &events_zv TSRMLS_CC) != SUCCESS) {
			return;
		}

		if (Z_TYPE_P(events_zv) != IS_ARRAY) return;

		HashPosition    pos;
		zval **entry;
		char *string_key;
		uint string_key_len;
		ulong index;

	    zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(events_zv), &pos);
	    while (zend_hash_get_current_data_ex(Z_ARRVAL_P(events_zv), (void **)&entry, &pos) == SUCCESS) {
	        if (zend_hash_get_current_key_ex(Z_ARRVAL_P(events_zv), &string_key, &string_key_len, &index, 0, &pos) == HASH_KEY_IS_STRING) {
	        	zval *event_zv;
	        	NII_NEW_STRING(event_zv, string_key);

	        	if ((*entry)->type != IS_STRING) {
	        		zval *tmp_array_zv;
	        		NII_NEW_ARRAY(tmp_array_zv);
	        		add_next_index_zval(tmp_array_zv, getThis());
	        		add_next_index_zval(tmp_array_zv, *entry);

					if (nii_call_class_method_2_no(owner_zv, "off", event_zv, tmp_array_zv TSRMLS_CC) != SUCCESS) {
						NII_PTR_DTOR(tmp_array_zv);
						return;
					}
					NII_PTR_DTOR(tmp_array_zv);
	           	}
	           	else {
					if (nii_call_class_method_2_no(owner_zv, "off", event_zv, *entry TSRMLS_CC) != SUCCESS) {
						NII_PTR_DTOR(*entry);
						return;
					}
					NII_PTR_DTOR(*entry);
	           	}

	     		NII_PTR_DTOR(event_zv);
	        }
	        zend_hash_move_forward_ex(Z_ARRVAL_P(events_zv), &pos);
	    }

		zend_update_property_null(Z_OBJCE_P(getThis()), getThis(), NII_SL("owner") TSRMLS_CC);
	}
}
/* }}} */

/** {{{ NII_CLASS_METHODS
*/
NII_CLASS_METHODS(base_behavior){
	PHP_ME(Behavior, events, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Behavior, attach, arginfo_base_behavior_attach, ZEND_ACC_PUBLIC)
	PHP_ME(Behavior, detach, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
/* }}} */

/** {{{ Class nii\\base\\Behavior
*/
NII_CLASS_FUNCTION(base_behavior) {


	//AUTO CALL CLASS METHOD
	NII_NS_CLASS_DECLARE_EX(nii\\base, Behavior, base_behavior, NII_CLASS_ENTRY(base_object), "nii\\base\\Object", 0);
	zend_declare_property_null(NII_CLASS_ENTRY(base_behavior), NII_SL("owner"), ZEND_ACC_PUBLIC TSRMLS_CC);
	return SUCCESS;
}
/* }}} */
