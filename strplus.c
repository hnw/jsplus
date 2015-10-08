/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_strplus.h"

ZEND_DECLARE_MODULE_GLOBALS(strplus)

/* True global resources - no need for thread safety here */
static int le_strplus;

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_BOOLEAN("strplus.enabled","1", PHP_INI_SYSTEM, OnUpdateBool, enabled, zend_strplus_globals, strplus_globals)
PHP_INI_END()
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_strplus_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_strplus_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "strplus", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

static zval *strplus_get_zval_ptr_tmpvar(zend_execute_data *execute_data, uint32_t var)
{
	zval *ret = EX_VAR(var);

	ZVAL_DEREF(ret);

	return ret;
}

static zval *strplus_get_zval_ptr_cv(zend_execute_data *execute_data, uint32_t var)
{
        zval *ret = EX_VAR(var);

        if (UNEXPECTED(Z_TYPE_P(ret) == IS_UNDEF)) {
			return NULL;
        } else {
			ZVAL_DEREF(ret);
        }
        return ret;
}

static zval *strplus_get_zval_ptr(zend_execute_data *execute_data, int op_type, znode_op op)
{
	if (op_type & (IS_TMP_VAR|IS_VAR)) {
		return strplus_get_zval_ptr_tmpvar(execute_data, op.var);
	} else {
		if (op_type == IS_CONST) {
			return EX_CONSTANT(op);
		} else if (op_type == IS_CV) {
			return strplus_get_zval_ptr_cv(execute_data, op.var);
		} else {
			return NULL;
		}
	}
}

static int strplus_add_handler(zend_execute_data *execute_data)
{
	zval *op1, *op2;
	if (STRPLUS_G(enabled)) {
		const zend_op *cur_opcode = EG(current_execute_data)->opline;
		const zend_op *opline = execute_data->opline;

		op1 = strplus_get_zval_ptr(execute_data, opline->op1_type, opline->op1);
		op2 = strplus_get_zval_ptr(execute_data, opline->op2_type, opline->op2);

		if (Z_TYPE_P(op1) == IS_STRING && Z_TYPE_P(op2) == IS_STRING) {
			if (cur_opcode->opcode == ZEND_ADD) {
				return ZEND_USER_OPCODE_DISPATCH_TO | ZEND_CONCAT;
			} else if (cur_opcode->opcode == ZEND_ASSIGN_ADD) {
				return ZEND_USER_OPCODE_DISPATCH_TO | ZEND_ASSIGN_CONCAT;
			}
		}
	}
	return ZEND_USER_OPCODE_DISPATCH;
}

/* {{{ php_strplus_init_globals
 */
static void php_strplus_init_globals(zend_strplus_globals *strplus_globals)
{
	strplus_globals->enabled = 1;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(strplus)
{
	ZEND_INIT_MODULE_GLOBALS(strplus, php_strplus_init_globals, NULL);

	REGISTER_INI_ENTRIES();

	zend_set_user_opcode_handler(ZEND_ADD, strplus_add_handler);
	zend_set_user_opcode_handler(ZEND_ASSIGN_ADD, strplus_add_handler);
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(strplus)
{
	zend_set_user_opcode_handler(ZEND_ADD, NULL);
	zend_set_user_opcode_handler(ZEND_ASSIGN_ADD, NULL);
	
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(strplus)
{
#if defined(COMPILE_DL_STRPLUS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(strplus)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(strplus)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "strplus support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ strplus_functions[]
 *
 * Every user visible function must have an entry in strplus_functions[].
 */
const zend_function_entry strplus_functions[] = {
	PHP_FE(confirm_strplus_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in strplus_functions[] */
};
/* }}} */

/* {{{ strplus_module_entry
 */
zend_module_entry strplus_module_entry = {
	STANDARD_MODULE_HEADER,
	"strplus",
	strplus_functions,
	PHP_MINIT(strplus),
	PHP_MSHUTDOWN(strplus),
	PHP_RINIT(strplus),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(strplus),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(strplus),
	PHP_STRPLUS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STRPLUS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(strplus)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
