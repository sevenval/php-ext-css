#include "php_extcss3.h"

#include "extcss3/intern.h"
#include "extcss3/utils.h"
#include "extcss3/dumper/dumper.h"
#include "extcss3/minifier/minifier.h"

#include <Zend/zend_exceptions.h>
#include <ext/standard/info.h>

/* ==================================================================================================== */

zend_class_entry *ce_extcss3;
zend_object_handlers oh_extcss3;

#define EXTCSS3_REGISTER_LONG_CLASS_CONST(name, value) \
	zend_declare_class_constant_long(ce_extcss3, name, strlen(name), value);

inline extcss3_object *extcss3_object_fetch(zend_object *object)
{
	return (extcss3_object *)((char *)(object) - XtOffsetOf(extcss3_object, std));
}

zend_object *extcss3_object_new(zend_class_entry *ce)
{
	extcss3_object *object = (extcss3_object *)ecalloc(1, sizeof(extcss3_object) + zend_object_properties_size(ce));

	object->intern = NULL;

	zend_object_std_init(&object->std, ce);
	object_properties_init(&object->std, ce);
	object->std.handlers = &oh_extcss3;

	return &object->std;
}

void extcss3_object_free(zend_object *zo)
{
	extcss3_object *object = extcss3_object_fetch(zo);

	if (object->intern) {
		extcss3_release_intern(object->intern);
	}

	zend_object_std_dtor(&object->std);
}

/* ==================================================================================================== */

static inline void php_extcss3_throw_exception(unsigned int error)
{
	switch (error) {
		case EXTCSS3_ERR_MEMORY:
			zend_throw_exception(zend_ce_exception, "extcss3: Memory allocation failed", EXTCSS3_ERR_MEMORY);
			break;
		case EXTCSS3_ERR_BYTES_CORRUPTION:
			zend_throw_exception(zend_ce_exception, "extcss3: Invalid or corrupt UTF-8 string detected", EXTCSS3_ERR_BYTES_CORRUPTION);
			break;
		case EXTCSS3_ERR_NULL_PTR:
			zend_throw_exception(zend_ce_exception, "extcss3: Unexpected NULL pointer received", EXTCSS3_ERR_NULL_PTR);
			break;
		case EXTCSS3_ERR_INV_PARAM:
			zend_throw_exception(zend_ce_exception, "extcss3: Invalid paramenter or parameter type given", EXTCSS3_ERR_INV_PARAM);
			break;
		default:
			zend_throw_exception(zend_ce_exception, "extcss3: Unknown internal error", 0);
	}
}

static inline void php_extcss3_make_data_array(extcss3_intern *intern, zval *data)
{
	extcss3_ctxt *ctxt = intern->last_ctxt;
	zval empty, info, context, contexts, value;

	array_init(&empty);

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	ZVAL_ARR(data, zend_array_dup(Z_ARRVAL(empty)));

	ZVAL_LONG(&value, intern->last_token->type);
	zend_hash_str_update(Z_ARRVAL_P(data), "type", 4 /*strlen("type")*/, &value);

	ZVAL_STRING(&value, extcss3_get_type_str(intern->last_token->type));
	zend_hash_str_update(Z_ARRVAL_P(data), "name", 4 /*strlen("name")*/, &value);

	ZVAL_STRINGL(&value, intern->last_token->data.str, intern->last_token->data.len);
	zend_hash_str_update(Z_ARRVAL_P(data), "value", 5 /*strlen("value")*/, &value);

	if (intern->last_token->user.str) {
		ZVAL_STRINGL(&value, intern->last_token->user.str, intern->last_token->user.len);
		zend_hash_str_update(Z_ARRVAL_P(data), "user", 4 /*strlen("user")*/, &value);
	}

	if (intern->last_token->flag) {
		ZVAL_ARR(&info, zend_array_dup(Z_ARRVAL(empty)));

		ZVAL_LONG(&value, intern->last_token->flag);
		zend_hash_str_update(Z_ARRVAL(info), "flag", 4 /*strlen("flag")*/, &value);

		ZVAL_STRING(&value, extcss3_get_flag_str(intern->last_token->flag));
		zend_hash_str_update(Z_ARRVAL(info), "name", 4 /*strlen("name")*/, &value);

		if (intern->last_token->info.len) {
			ZVAL_STRINGL(&value, intern->last_token->info.str, intern->last_token->info.len);
			zend_hash_str_update(Z_ARRVAL(info), "value", 5 /*strlen("value")*/, &value);
		}

		zend_hash_str_update(Z_ARRVAL_P(data), "info", 4 /*strlen("info")*/, &info);
	}

	if (ctxt && ctxt->level) {
		ZVAL_ARR(&contexts, zend_array_dup(Z_ARRVAL(empty)));

		while (ctxt->level) {
			ZVAL_ARR(&context, zend_array_dup(Z_ARRVAL(empty)));

			ZVAL_LONG(&value, ctxt->level);
			zend_hash_str_update(Z_ARRVAL(context), "level", 5 /*strlen("level")*/, &value);

			ZVAL_LONG(&value, ctxt->token->type);
			zend_hash_str_update(Z_ARRVAL(context), "type", 4 /*strlen("type")*/, &value);

			ZVAL_STRING(&value, extcss3_get_type_str(ctxt->token->type));
			zend_hash_str_update(Z_ARRVAL(context), "name", 4 /*strlen("name")*/, &value);

			ZVAL_STRINGL(&value, ctxt->token->data.str, ctxt->token->data.len);
			zend_hash_str_update(Z_ARRVAL(context), "value", 5 /*strlen("value")*/, &value);

			zend_hash_next_index_insert(Z_ARRVAL(contexts), &context);

			ctxt = ctxt->prev;
		}

		zend_hash_str_update(Z_ARRVAL_P(data), "context", 7 /*strlen("context")*/, &contexts);
	}

	zval_ptr_dtor(&empty);
}

static void php_extcss3_modifier_callback(extcss3_intern *intern)
{
	zval data, retval, *args, *callable = NULL;

	if ((intern == NULL) || (intern->last_token == NULL)) {
		return;
	}

	switch (intern->last_token->type) {
		case EXTCSS3_TYPE_STRING:
			callable = intern->modifier.string;
			break;
		case EXTCSS3_TYPE_BAD_STRING:
			callable = intern->modifier.bad_string;
			break;
		case EXTCSS3_TYPE_URL:
			callable = intern->modifier.url;
			break;
		case EXTCSS3_TYPE_BAD_URL:
			callable = intern->modifier.bad_url;
			break;
		case EXTCSS3_TYPE_COMMENT:
			callable = intern->modifier.comment;
			break;
		default:
			return;
	}

	if (callable == NULL) {
		return;
	}

	php_extcss3_make_data_array(intern, &data);

	args = safe_emalloc(sizeof(zval), 1, 0);
	ZVAL_COPY(&args[0], &data);

	if (SUCCESS == call_user_function_ex(EG(function_table), NULL, callable, &retval, 1, args, 0, NULL)) {
		if (Z_TYPE(retval) == IS_STRING) {
			intern->last_token->user.len = Z_STRLEN(retval);
			intern->last_token->user.str = (char *)calloc(intern->last_token->user.len, sizeof(char));

			memcpy(intern->last_token->user.str, Z_STRVAL(retval), Z_STRLEN(retval));

			intern->modifier.user_strlen_diff -= intern->last_token->data.len;
			intern->modifier.user_strlen_diff += intern->last_token->user.len;
		}
	}

	zval_ptr_dtor(&data);
	zval_ptr_dtor(&args[0]);
	zval_ptr_dtor(&retval);

	efree(args);
}

static void php_extcss3_modifier_destructor(void *modifier)
{
	zval *ptr = (zval *)modifier;

	if (ptr == NULL) {
		return;
	}

	zval_ptr_dtor(ptr);
	efree(ptr);
}

/* ==================================================================================================== */

ZEND_BEGIN_ARG_INFO_EX(arginfo_EXTCSS3_void, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_EXTCSS3_setModifier, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, callable, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_EXTCSS3_dump, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, css, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_EXTCSS3_minify, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, css, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, vendors, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

/* ==================================================================================================== */

PHP_METHOD(CSS3Processor, __construct)
{
	extcss3_object *object = extcss3_object_fetch(Z_OBJ_P(getThis()));
	extcss3_intern *intern = extcss3_create_intern();

	if (intern == NULL) {
		php_extcss3_throw_exception(EXTCSS3_ERR_MEMORY);
	} else {
		intern->modifier.callback	= php_extcss3_modifier_callback;
		intern->modifier.destructor	= php_extcss3_modifier_destructor;

		object->intern = intern;
	}
}

PHP_METHOD(CSS3Processor, setModifier)
{
	extcss3_object *object = extcss3_object_fetch(Z_OBJ_P(getThis()));
	extcss3_intern *intern = object->intern;
	zval *callable, *copy;
	size_t type;
	unsigned int error = 0;

	if (SUCCESS != zend_parse_parameters(ZEND_NUM_ARGS(), "lz", &type, &callable)) {
		return;
	} else if (intern == NULL) {
		php_extcss3_throw_exception(EXTCSS3_ERR_NULL_PTR);
		return;
	} else if (!EXTCSS3_TYPE_IS_MODIFIABLE(type)) {
		zend_throw_exception(zend_ce_exception, "Setting the modifier for an unmodifiable type failed", 0);
		return;
	}

	copy = (zval *)ecalloc(1, sizeof(zval));

	if (copy == NULL) {
		php_extcss3_throw_exception(EXTCSS3_ERR_MEMORY);
		return;
	}

	ZVAL_COPY(copy, callable);

	if (EXTCSS3_SUCCESS != extcss3_set_modifier(intern, type, copy, &error)) {
		intern->modifier.destructor(copy);

		php_extcss3_throw_exception(error);
		return;
	}

	RETURN_TRUE;
}

PHP_METHOD(CSS3Processor, dump)
{
	extcss3_object *object = extcss3_object_fetch(Z_OBJ_P(getThis()));
	extcss3_intern *intern = object->intern;
	char *css, *result;
	size_t len;
	unsigned int error = 0;

	if (SUCCESS != zend_parse_parameters(ZEND_NUM_ARGS(), "s", &css, &len)) {
		return;
	} else if (intern == NULL) {
		php_extcss3_throw_exception(EXTCSS3_ERR_NULL_PTR);
	} else if (!len) {
		RETURN_EMPTY_STRING();
	} else if (EXTCSS3_SUCCESS != extcss3_set_css_string(intern, css, len, &error)) {
		php_extcss3_throw_exception(error);
	} else if ((result = extcss3_dump_tokens(intern, &error)) == NULL) {
		php_extcss3_throw_exception(error);
	} else {
		RETVAL_STRING(result);
		free(result);
	}
}

PHP_METHOD(CSS3Processor, minify)
{
	extcss3_object *object = extcss3_object_fetch(Z_OBJ_P(getThis()));
	extcss3_intern *intern = object->intern;
	zval *name, *vendors = NULL;
	char *css, *result;
	size_t len;
	unsigned int error = 0;

	if (SUCCESS != zend_parse_parameters(ZEND_NUM_ARGS(), "s|a", &css, &len, &vendors)) {
		return;
	} else if (intern == NULL) {
		php_extcss3_throw_exception(EXTCSS3_ERR_NULL_PTR);
		return;
	} else if (!len) {
		RETURN_EMPTY_STRING();
	} else if (EXTCSS3_SUCCESS != extcss3_set_css_string(intern, css, len, &error)) {
		php_extcss3_throw_exception(error);
		return;
	}

	if ((intern->base_vendor != NULL) || (intern->last_vendor != NULL)) {
		extcss3_release_vendors_list(intern->base_vendor);
		intern->base_vendor = intern->last_vendor = NULL;
	}

	if ((vendors != NULL) && (Z_TYPE_P(vendors) == IS_ARRAY)) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(vendors), name) {
			if (Z_TYPE_P(name) != IS_STRING) {
				convert_to_string(name);
			}

			if (intern->last_vendor == NULL) {
				intern->base_vendor = intern->last_vendor = extcss3_create_vendor();
			} else {
				intern->last_vendor->next = extcss3_create_vendor();
				intern->last_vendor = intern->last_vendor->next;
			}

			if ((intern->last_vendor == NULL) || (EXTCSS3_SUCCESS != extcss3_set_vendor_string(intern, Z_STRVAL_P(name), Z_STRLEN_P(name), &error))) {
				php_extcss3_throw_exception(error);
				return;
			}
		} ZEND_HASH_FOREACH_END();
	}

	if ((result = extcss3_minify(intern, &error)) == NULL) {
		php_extcss3_throw_exception(error);
	} else {
		RETVAL_STRING(result);
		free(result);
	}
}

/* ==================================================================================================== */

zend_function_entry extcss3_methods[] = {
	PHP_ME(CSS3Processor, __construct, arginfo_EXTCSS3_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(CSS3Processor, setModifier, arginfo_EXTCSS3_setModifier, ZEND_ACC_PUBLIC)
	PHP_ME(CSS3Processor, dump, arginfo_EXTCSS3_dump, ZEND_ACC_PUBLIC)
	PHP_ME(CSS3Processor, minify, arginfo_EXTCSS3_minify, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* ==================================================================================================== */

PHP_MINIT_FUNCTION(extcss3)
{
	zend_class_entry ce;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	memcpy(&oh_extcss3, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	oh_extcss3.offset = XtOffsetOf(extcss3_object, std);
	oh_extcss3.dtor_obj = zend_objects_destroy_object;
	oh_extcss3.free_obj = extcss3_object_free;

	INIT_CLASS_ENTRY(ce, "CSS3Processor", extcss3_methods);
	ce.create_object = extcss3_object_new;
	ce_extcss3 = zend_register_internal_class_ex(&ce, NULL);

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_IDENT",			EXTCSS3_TYPE_IDENT);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_FUNCTION",		EXTCSS3_TYPE_FUNCTION);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_AT_KEYWORD",	EXTCSS3_TYPE_AT_KEYWORD);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_HASH",			EXTCSS3_TYPE_HASH);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_STRING",		EXTCSS3_TYPE_STRING);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_BAD_STRING",	EXTCSS3_TYPE_BAD_STRING);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_URL",			EXTCSS3_TYPE_URL);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_BAD_URL",		EXTCSS3_TYPE_BAD_URL);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_DELIM",			EXTCSS3_TYPE_DELIM);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_NUMBER",		EXTCSS3_TYPE_NUMBER);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_PERCENTAGE",	EXTCSS3_TYPE_PERCENTAGE);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_DIMENSION",		EXTCSS3_TYPE_DIMENSION);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_UNICODE_RANGE",	EXTCSS3_TYPE_UNICODE_RANGE);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_INCLUDE_MATCH",	EXTCSS3_TYPE_INCLUDE_MATCH);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_DASH_MATCH",	EXTCSS3_TYPE_DASH_MATCH);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_PREFIX_MATCH",	EXTCSS3_TYPE_PREFIX_MATCH);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_SUFFIX_MATCH",	EXTCSS3_TYPE_SUFFIX_MATCH);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_SUBSTR_MATCH",	EXTCSS3_TYPE_SUBSTR_MATCH);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_COLUMN",		EXTCSS3_TYPE_COLUMN);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_WS",			EXTCSS3_TYPE_WS);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_CDO",			EXTCSS3_TYPE_CDO);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_CDC",			EXTCSS3_TYPE_CDC);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_COLON",			EXTCSS3_TYPE_COLON);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_SEMICOLON",		EXTCSS3_TYPE_SEMICOLON);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_COMMA",			EXTCSS3_TYPE_COMMA);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_BR_RO",			EXTCSS3_TYPE_BR_RO);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_BR_RC",			EXTCSS3_TYPE_BR_RC);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_BR_SO",			EXTCSS3_TYPE_BR_SO);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_BR_SC",			EXTCSS3_TYPE_BR_SC);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_BR_CO",			EXTCSS3_TYPE_BR_CO);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_BR_CC",			EXTCSS3_TYPE_BR_CC);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_COMMENT",		EXTCSS3_TYPE_COMMENT);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("TYPE_EOF",			EXTCSS3_TYPE_EOF);

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	EXTCSS3_REGISTER_LONG_CLASS_CONST("FLAG_ID",			EXTCSS3_FLAG_ID);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("FLAG_UNRESTRICTED",	EXTCSS3_FLAG_UNRESTRICTED);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("FLAG_INTEGER",		EXTCSS3_FLAG_INTEGER);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("FLAG_NUMBER",		EXTCSS3_FLAG_NUMBER);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("FLAG_STRING",		EXTCSS3_FLAG_STRING);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("FLAG_AT_URL_STRING",	EXTCSS3_FLAG_AT_URL_STRING);
	
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	EXTCSS3_REGISTER_LONG_CLASS_CONST("ERR_MEMORY",				EXTCSS3_ERR_MEMORY);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("ERR_BYTES_CORRUPTION",	EXTCSS3_ERR_BYTES_CORRUPTION);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("ERR_NULL_PTR",			EXTCSS3_ERR_NULL_PTR);
	EXTCSS3_REGISTER_LONG_CLASS_CONST("ERR_INV_PARAM",			EXTCSS3_ERR_INV_PARAM);

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	return SUCCESS;
}

PHP_MINFO_FUNCTION(extcss3)
{
	php_info_print_table_start();
		php_info_print_table_header(2, PHP_EXTCSS3_EXTNAME, "enabled");
		php_info_print_table_row(2, "Version", PHP_EXTCSS3_EXTVER);
	php_info_print_table_end();
}

/* ==================================================================================================== */

zend_module_entry extcss3_module_entry =
{
	STANDARD_MODULE_HEADER,
	PHP_EXTCSS3_EXTNAME,
	NULL,					/* Functions */
	PHP_MINIT(extcss3),
	NULL,					/* MSHUTDOWN */
	NULL,					/* RINIT */
	NULL,					/* RSHUTDOWN */
	PHP_MINFO(extcss3),
	PHP_EXTCSS3_EXTVER,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_EXTCSS3
	ZEND_GET_MODULE(extcss3)
#endif /* COMPILE_DL_EXTCSS3 */
