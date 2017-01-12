#include "dumper.h"
#include "../intern.h"
#include "../utils.h"
#include "../tokenizer/tokenizer.h"

#include <string.h>

/* ==================================================================================================== */

static void *_extcss3_set_error_code(int *error, int code);

static void _extcss3_write_rules(extcss3_rule *rule, char **result);
static void _extcss3_write_decls(extcss3_decl *decl, char **result);
static void _extcss3_write_token(extcss3_token *token, char **result);

/* ==================================================================================================== */

char *extcss3_dump_rules(extcss3_intern *intern, extcss3_rule *rule, int *error)
{
	char *result, *curr;

	if ((intern == NULL) || (intern->copy.str == NULL) || (rule == NULL)) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_NULL_PTR);
	} else if ((result = curr = (char *)calloc(1, sizeof(char) * (intern->copy.len + 1))) == NULL) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_MEMORY);
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	_extcss3_write_rules(rule, &curr);

	return result;
}

char *extcss3_dump_tokens(extcss3_intern *intern, int *error)
{
	extcss3_token *token;
	char *result, *pos;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if ((intern == NULL) || (intern->copy.str == NULL)) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_NULL_PTR);
	} else if (EXTCSS3_SUCCESS != extcss3_tokenize(intern, error)) {
		return _extcss3_set_error_code(error, *error);
	} else if ((token = intern->base_token) == NULL) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_NULL_PTR);
	} else if ((result = pos = (char *)calloc(1, sizeof(char) * (intern->copy.len + 1))) == NULL) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_MEMORY);
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	while (token != NULL) {
		if ((token->type == EXTCSS3_TYPE_URL) || (token->type == EXTCSS3_TYPE_BAD_URL)) {
			if (token->flag != EXTCSS3_FLAG_AT_URL_STRING) {
				memcpy(pos, "url(", 4);
				pos += 4;
			}
		}

		if (!token->user.str) {
			memcpy(pos, token->data.str, token->data.len);
			pos += token->data.len;
		} else {
			memcpy(pos, token->user.str, token->user.len);
			pos += token->user.len;
		}

		if ((token->type == EXTCSS3_TYPE_URL)) {
			if (token->flag != EXTCSS3_FLAG_AT_URL_STRING) {
				*pos = ')';
				pos += 1;
			}
		} else if (!token->user.str && (token->type == EXTCSS3_TYPE_FUNCTION)) {
			*pos = '(';
			pos += 1;
		}

		token = token->next;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	extcss3_release_tokens_list(intern->base_token);
	intern->base_token = NULL;

	return result;
}

/* ==================================================================================================== */

static inline void *_extcss3_set_error_code(int *error, int code)
{
	*error = code;

	return NULL;
}

/* ==================================================================================================== */

static inline void _extcss3_write_rules(extcss3_rule *rule, char **result)
{
	extcss3_token *token;

	while (rule != NULL) {
		token = rule->base_selector;

		while (token != NULL) {
			_extcss3_write_token(token, result);

			if (token == rule->last_selector) {
				break;
			}

			token = token->next;
		}

		if (rule->block != NULL) {
			memcpy(*result, rule->block->base->data.str, rule->block->base->data.len);
			*result += rule->block->base->data.len;

			if (rule->block->rules != NULL) {
				_extcss3_write_rules(rule->block->rules, result);
			} else if (rule->block->decls != NULL) {
				_extcss3_write_decls(rule->block->decls, result);
			}

			memcpy(*result, rule->block->last->data.str, rule->block->last->data.len);
			*result += rule->block->last->data.len;
		}

		rule = rule->next;
	}
}

static inline void _extcss3_write_decls(extcss3_decl *decl, char **result)
{
	extcss3_token *token;

	while (decl != NULL) {
		token = decl->base;

		while (token != NULL) {
			_extcss3_write_token(token, result);

			if (token == decl->last) {
				break;
			}

			token = token->next;
		}

		decl = decl->next;
	}
}

static inline void _extcss3_write_token(extcss3_token *token, char **result)
{
	size_t writer, reader;

	if ((token->type == EXTCSS3_TYPE_URL) || (token->type == EXTCSS3_TYPE_BAD_URL)) {
		if (token->flag != EXTCSS3_FLAG_AT_URL_STRING) {
			memcpy(*result, "url(", 4);
			*result += 4;
		}
	}

	if (_EXTCSS3_TYPE_EMPTY_EX(token)) {
		**result = ' ';
		*result += 1;
	} else if (token->user.str == NULL) {
		for (writer = 0, reader = 0; reader < token->data.len; writer++, reader++) {
			if ((token->type == EXTCSS3_TYPE_URL) || (token->type == EXTCSS3_TYPE_STRING)) {
				if (token->data.str[reader] == '\\' && token->data.str[reader + 1] == '\n') {
					writer--;
					reader++;
					continue;
				}
			}

			if (reader + 1 < token->data.len && token->data.str[reader] == '\\' && token->data.str[reader + 1] == '0') {
				(*result)[writer++] = token->data.str[reader++];

				while (reader < token->data.len && token->data.str[reader] == '0') {
					reader++;
				}
			}

			(*result)[writer] = token->data.str[reader];
		}

		*result += writer;
	} else {
		memcpy(*result, token->user.str, token->user.len);
		*result += token->user.len;
	}

	if ((token->type == EXTCSS3_TYPE_URL)) {
		if (token->flag != EXTCSS3_FLAG_AT_URL_STRING) {
			**result = ')';
			*result += 1;
		}
	} else if ((token->user.str == NULL) && (token->type == EXTCSS3_TYPE_FUNCTION)) {
		**result = '(';
		*result += 1;
	}
}
