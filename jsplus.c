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
#include "php_jsplus.h"

ZEND_DECLARE_MODULE_GLOBALS(jsplus)

/* True global resources - no need for thread safety here */
static int le_jsplus;
zend_ast_process_t original_ast_process = NULL;

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_BOOLEAN("jsplus.enabled","1", PHP_INI_SYSTEM, OnUpdateBool, enabled, zend_jsplus_globals, jsplus_globals)
PHP_INI_END()
/* }}} */

static zval *jsplus_get_zval_ptr_tmpvar(zend_execute_data *execute_data, uint32_t var)
{
	zval *ret = EX_VAR(var);

	ZVAL_DEREF(ret);

	return ret;
}

static zval *jsplus_get_zval_ptr_cv(zend_execute_data *execute_data, uint32_t var)
{
        zval *ret = EX_VAR(var);

        if (UNEXPECTED(Z_TYPE_P(ret) == IS_UNDEF)) {
			return NULL;
        } else {
			ZVAL_DEREF(ret);
        }
        return ret;
}

static zval *jsplus_get_zval_ptr(zend_execute_data *execute_data, int op_type, znode_op op)
{
	if (op_type & (IS_TMP_VAR|IS_VAR)) {
		return jsplus_get_zval_ptr_tmpvar(execute_data, op.var);
	} else {
		if (op_type == IS_CONST) {
			return EX_CONSTANT(op);
		} else if (op_type == IS_CV) {
			return jsplus_get_zval_ptr_cv(execute_data, op.var);
		} else {
			return NULL;
		}
	}
}

static int jsplus_add_handler(zend_execute_data *execute_data)
{
	zval *op1, *op2;
	if (JSPLUS_G(enabled)) {
		const zend_op *cur_opcode = EG(current_execute_data)->opline;
		const zend_op *opline = execute_data->opline;

		op1 = jsplus_get_zval_ptr(execute_data, opline->op1_type, opline->op1);
		op2 = jsplus_get_zval_ptr(execute_data, opline->op2_type, opline->op2);

		if (Z_TYPE_P(op1) == IS_STRING || Z_TYPE_P(op2) == IS_STRING) {
			if (Z_TYPE_P(op1) != IS_STRING && opline->op1_type == IS_CONST) {
				ZVAL_STR(op1, zval_get_string(op1));
			}
			if (Z_TYPE_P(op2) != IS_STRING && opline->op2_type == IS_CONST) {
				ZVAL_STR(op2, zval_get_string(op2));
			}
			if (cur_opcode->opcode == ZEND_ADD) {
				return ZEND_USER_OPCODE_DISPATCH_TO | ZEND_CONCAT;
			} else if (cur_opcode->opcode == ZEND_ASSIGN_ADD) {
				return ZEND_USER_OPCODE_DISPATCH_TO | ZEND_ASSIGN_CONCAT;
			}
		}
	}
	return ZEND_USER_OPCODE_DISPATCH;
}

static int ast_is_decl(zend_ast *ast) /* {{{ */
{
    return (
        ast->kind == ZEND_AST_FUNC_DECL ||
        ast->kind == ZEND_AST_CLOSURE ||
        ast->kind == ZEND_AST_METHOD ||
        ast->kind == ZEND_AST_CLASS
    );
}
/* }}} */

void jsplus_ast_process(zend_ast *ast)
{
	int i, num_children = 0;
	zend_ast **children;

	if (!JSPLUS_G(enabled)) {
		return;
	}

	if (zend_ast_is_list(ast)) {
		zend_ast_list *list = (zend_ast_list*)ast;
		num_children = list->children;
		children = list->child;
	} else if (ast_is_decl(ast)) {
		zend_ast_decl *decl = (zend_ast_decl*)ast;
		num_children = 4;
		children = decl->child;
	} else {
		num_children = zend_ast_get_num_children(ast);
		children = ast->child;
	}

	for (i = 0; i < num_children; i++) {
		if (children[i]) {
			jsplus_ast_process(children[i]);
		}
	}
	if (ast->kind == ZEND_AST_BINARY_OP) {
		uint32_t opcode = ast->attr;
		if (opcode == ZEND_ADD &&
			ast->child[0]->kind == ZEND_AST_ZVAL &&
			ast->child[1]->kind == ZEND_AST_ZVAL) {
			zval *op1, *op2;
			op1 = zend_ast_get_zval(ast->child[0]);
			op2 = zend_ast_get_zval(ast->child[1]);
			if (Z_TYPE_P(op1) == IS_STRING || Z_TYPE_P(op2) == IS_STRING) {
				ast->attr = ZEND_CONCAT;
			}
		}
	}
}

/* {{{ php_jsplus_init_globals
 */
static void php_jsplus_init_globals(zend_jsplus_globals *jsplus_globals)
{
	jsplus_globals->enabled = 1;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(jsplus)
{
	ZEND_INIT_MODULE_GLOBALS(jsplus, php_jsplus_init_globals, NULL);

	REGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(jsplus)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(jsplus)
{
#if defined(COMPILE_DL_JSPLUS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	original_ast_process = zend_ast_process;
	zend_ast_process = jsplus_ast_process;

	zend_set_user_opcode_handler(ZEND_ADD, jsplus_add_handler);
	zend_set_user_opcode_handler(ZEND_ASSIGN_ADD, jsplus_add_handler);

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(jsplus)
{
	zend_ast_process = original_ast_process;
	original_ast_process = NULL;

	zend_set_user_opcode_handler(ZEND_ADD, NULL);
	zend_set_user_opcode_handler(ZEND_ASSIGN_ADD, NULL);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(jsplus)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "jsplus support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ jsplus_functions[]
 *
 * Every user visible function must have an entry in jsplus_functions[].
 */
const zend_function_entry jsplus_functions[] = {
	PHP_FE_END	/* Must be the last line in jsplus_functions[] */
};
/* }}} */

/* {{{ jsplus_module_entry
 */
zend_module_entry jsplus_module_entry = {
	STANDARD_MODULE_HEADER,
	"jsplus",
	jsplus_functions,
	PHP_MINIT(jsplus),
	PHP_MSHUTDOWN(jsplus),
	PHP_RINIT(jsplus),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(jsplus),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(jsplus),
	PHP_JSPLUS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_JSPLUS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(jsplus)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
