uint nii_get_class(zval *ce, const char **classname TSRMLS_DC);
int nii_getter(zval *ce, char *name, zval **retval TSRMLS_DC);
int nii_getter_exception(zval *ce, char *name, const char *classname TSRMLS_DC);
int nii_setter(zval *ce, char *name, zval *value TSRMLS_DC);
int nii_setter_exception(zval *ce, char *name, const char *classname TSRMLS_DC);
void nii_getter_unknow_exception(zval *ce, char *name, const char *classname TSRMLS_DC);
void nii_setter_unknow_exception(zval *ce, char *name, const char *classname TSRMLS_DC);