void nii_inherit_not_found(char *class_name, char *inherit_name);

void nii_concat(zval **left, zval *right TSRMLS_DC);

/* call global function */
int nii_call_user_fun(const char *function_name, zval **retval_ptr_ptr, zend_uint param_count, zval **params[] TSRMLS_DC);
int nii_call_user_fun_0(const char *func_name, zval **retval TSRMLS_DC);
int nii_call_user_fun_0_no(const char *func_name TSRMLS_DC);
int nii_call_user_fun_1(const char *func_name, zval **retval, zval *param TSRMLS_DC);
int nii_call_user_fun_1_no(const char *func_name, zval *param TSRMLS_DC);
int nii_call_user_fun_2(const char *func_name, zval **retval, zval *param1, zval *param2 TSRMLS_DC);
int nii_call_user_fun_2_no(const char *func_name, zval *param1, zval *param2 TSRMLS_DC);
int nii_call_user_fun_3(const char *func_name, zval **retval, zval *param1, zval *param2, zval *param3 TSRMLS_DC);
int nii_call_user_fun_3_no(const char *func_name, zval *param1, zval *param2, zval *param3 TSRMLS_DC);
int nii_call_user_fun_4(const char *func_name, zval **retval, zval *param1, zval *param2, zval *param3, zval *param4 TSRMLS_DC);
int nii_call_user_fun_4_no(const char *func_name, zval *param1, zval *param2, zval *param3, zval *param4 TSRMLS_DC);
int nii_call_user_fun_5(const char *func_name, zval **retval, zval *param1, zval *param2, zval *param3, zval *param4, zval *param5 TSRMLS_DC);
int nii_call_user_fun_5_no(const char *func_name, zval *param1, zval *param2, zval *param3, zval *param4, zval *param5 TSRMLS_DC);

/* call class method */
int nii_call_class_method(zval *object, char *method_name, zval **retval_prt_prt, zend_uint param_count, zval **params[] TSRMLS_DC);
int nii_call_class_method_no(zval *object, char *method_name, zend_uint param_count, zval **params[] TSRMLS_DC);
int nii_call_class_method_0(zval *object, char *method_name, zval **retval TSRMLS_DC);
int nii_call_class_method_0_no(zval *object, char *method_name TSRMLS_DC);
int nii_call_class_method_1(zval *object, char *method_name, zval **retval, zval *param TSRMLS_DC);
int nii_call_class_method_1_no(zval *object, char *method_name, zval *param TSRMLS_DC);
int nii_call_class_method_2(zval *object, char *method_name, zval **retval, zval *param1, zval *param2 TSRMLS_DC);
int nii_call_class_method_2_no(zval *object, char *method_name, zval *param1, zval *param2 TSRMLS_DC);

/* new class */
int nii_new_class_instance(zval **return_value, char *class_name, zend_uint param_count, zval **params[] TSRMLS_DC);
int nii_new_class_instance_0(zval **return_value, char *class_name TSRMLS_DC);
int nii_new_class_instance_1(zval **return_value, char *class_name, zval *param TSRMLS_DC);

/* call call static method */
int nii_call_class_static_method(zval *object, char *class_name, char *method_name, zval **retval_ptr_ptr, zend_uint param_count, zval **params[] TSRMLS_DC);
int nii_find_scope(zend_class_entry *ce, char *method_name TSRMLS_DC);

int nii_call_class_static_method_0(zval *object, char *class_name, char *method_name, zval **retval TSRMLS_DC);
int nii_call_class_static_method_0_no(zval *object, char *class_name, char *method_name TSRMLS_DC);
int nii_call_class_static_method_1(zval *object, char *class_name, char *method_name, zval **retval, zval *param TSRMLS_DC);
int nii_call_class_static_method_1_no(zval *object, char *class_name, char *method_name, zval *param TSRMLS_DC);
int nii_call_class_static_method_2(zval *object, char *class_name, char *method_name, zval **retval, zval *param1, zval *param2 TSRMLS_DC);
int nii_call_class_static_method_2_no(zval *object, char *class_name, char *method_name, zval *param1, zval *param2 TSRMLS_DC);


