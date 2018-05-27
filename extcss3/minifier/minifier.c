#include "minifier.h"
#include "tree.h"
#include "types/numeric.h"
#include "types/hash.h"
#include "types/function.h"
#include "../intern.h"
#include "../utils.h"
#include "../tokenizer/tokenizer.h"
#include "../dumper/dumper.h"

#include <string.h>
#include <strings.h>

/* ==================================================================================================== */

static void *_extcss3_set_error_code(unsigned int *error, unsigned int code, extcss3_rule *tree, extcss3_intern *intern);

static extcss3_rule *_extcss3_minify_tree(extcss3_intern *intern, extcss3_rule **tree, unsigned int *error);
static extcss3_rule *_extcss3_minify_rule(extcss3_intern *intern, extcss3_rule *rule, unsigned int *error);
static extcss3_token *_extcss3_minify_selectors(extcss3_intern *intern, extcss3_rule *rule, unsigned int *error);
static extcss3_block *_extcss3_minify_declarations(extcss3_intern *intern, extcss3_block *block, unsigned int *error);
static extcss3_decl *_extcss3_minify_declaration(extcss3_intern *intern, extcss3_decl *decl, unsigned int *error);

static extcss3_token *_extcss3_get_decl_name(extcss3_decl *decl);
static extcss3_token *_extcss3_get_decl_sep(extcss3_token *name, extcss3_decl *decl);
static extcss3_token *_extcss3_get_decl_value(extcss3_token *last, extcss3_decl *decl);

static void _extcss3_trim_left(extcss3_token *curr);
static void _extcss3_trim_right(extcss3_token *curr, extcss3_token **last);
static void _extcss3_trim_around(extcss3_token *curr, extcss3_token **last);

static void _extcss3_remove_token(extcss3_token **base, extcss3_token **token, extcss3_token *repl);

static bool _extcss3_check_at_rule_is_valid_charset(extcss3_intern *intern, extcss3_rule *rule);
static bool _extcss3_check_at_rule_is_valid_import(extcss3_rule *rule);
static bool _extcss3_check_at_rule_is_valid_namespace(extcss3_rule *rule);

static bool _extcss3_check_minify_color(extcss3_token *name, extcss3_token *value);

/* ==================================================================================================== */

char *extcss3_minify(extcss3_intern *intern, unsigned int *error)
{
	extcss3_token *token;
	extcss3_rule *tree;
	char *result;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if ((intern == NULL) || (intern->copy.str == NULL)) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_NULL_PTR, NULL, NULL);
	} else if (EXTCSS3_SUCCESS != extcss3_tokenize(intern, error)) {
		return _extcss3_set_error_code(error, *error, NULL, NULL);
	} else if ((token = intern->base_token) == NULL) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_NULL_PTR, NULL, NULL);
	} else if ((tree = extcss3_create_tree(&token, NULL, 0, error)) == NULL) {
		return _extcss3_set_error_code(error, *error, NULL, intern);
	} else if (tree->base_selector == NULL) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_NULL_PTR, tree, intern);
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	_extcss3_minify_tree(intern, &tree, error);

	if (*error != 0) {
		return _extcss3_set_error_code(error, *error, tree, intern);
	} else if (tree == NULL) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_NULL_PTR, NULL, intern);
	} else if ((result = extcss3_dump_rules(intern, tree, error)) == NULL) {
		return _extcss3_set_error_code(error, *error, tree, intern);
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	_extcss3_set_error_code(error, 0, tree, intern);

	return result;
}

/* ==================================================================================================== */

static inline void *_extcss3_set_error_code(unsigned int *error, unsigned int code, extcss3_rule *tree, extcss3_intern *intern)
{
	*error = code;

	if (tree != NULL) {
		extcss3_release_rules_list(tree);
	}

	if ((intern != NULL) && (intern->base_token != NULL)) {
		extcss3_release_tokens_list(intern->base_token);
		intern->base_token = NULL;
	}

	return NULL;
}

/* ==================================================================================================== */

static extcss3_rule *_extcss3_minify_tree(extcss3_intern *intern, extcss3_rule **tree, unsigned int *error)
{
	extcss3_rule *next, *curr = *tree;

	while ((curr != NULL) && (curr->base_selector != NULL) && (curr->base_selector->type != EXTCSS3_TYPE_EOF)) {
		if (_extcss3_minify_rule(intern, curr, error) == NULL) {
			if (*error > 0) {
				return NULL;
			}

			if (curr->prev != NULL) {
				curr->prev->next = curr->next;
			}
			if (curr->next != NULL) {
				curr->next->prev = curr->prev;
			}

			if (curr == *tree) {
				// Uncouple current rule from the tree root
				*tree = curr->next;
			}

			next = curr->next;
			extcss3_release_rule(curr);

			curr = next;
			continue;
		}

		curr = curr->next;
	}

	if ((*tree)->base_selector == NULL) {
		extcss3_release_rules_list(*tree);

		*tree = NULL;
	}

	return *tree;
}

static extcss3_rule *_extcss3_minify_rule(extcss3_intern *intern, extcss3_rule *rule, unsigned int *error)
{
	if ((rule == NULL) && (rule->base_selector == NULL)) {
		return NULL;
	} else if (rule->base_selector->type == EXTCSS3_TYPE_EOF) {
		return rule;
	} else if ((rule->base_selector = _extcss3_minify_selectors(intern, rule, error)) == NULL) {
		return NULL;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if (rule->block != NULL) {
		if (rule->block->rules != NULL) {
			if ((rule->block->rules = _extcss3_minify_tree(intern, &(rule->block->rules), error)) == NULL) {
				return NULL;
			}
		} else if (rule->block->decls != NULL) {
			if ((rule->block = _extcss3_minify_declarations(intern, rule->block, error)) == NULL) {
				return NULL;
			}
		} else {
			return NULL;
		}
	}

	return rule;
}

static inline extcss3_token *_extcss3_minify_selectors(extcss3_intern *intern, extcss3_rule *rule, unsigned int *error)
{
	extcss3_token *selector, *range_base, *range_last;
	extcss3_vendor *vendor;
	bool preserve_sign;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	// Check <at-keyword> tokens "@charset", "@import" and "@namespace"
	if (rule->base_selector->type == EXTCSS3_TYPE_AT_KEYWORD) {
		if (
			(rule->base_selector->data.len == 8 /* strlen("@charset") */) &&
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(rule->base_selector->data.str, "@charset", 8)) &&
			(
				(rule->level != 0) || (EXTCSS3_SUCCESS != _extcss3_check_at_rule_is_valid_charset(intern, rule))
			)
		) {
			return rule->base_selector = rule->last_selector = NULL;
		} else if (
			(rule->base_selector->data.len == 7 /* strlen("@import") */) &&
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(rule->base_selector->data.str, "@import", 7)) &&
			(
				(rule->level != 0) || (EXTCSS3_SUCCESS != _extcss3_check_at_rule_is_valid_import(rule))
			)
		) {
			return rule->base_selector = rule->last_selector = NULL;
		} else if (
			(rule->base_selector->data.len == 10 /* strlen("@namespace") */) &&
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(rule->base_selector->data.str, "@namespace", 10)) &&
			(
				(rule->level != 0) || (EXTCSS3_SUCCESS != _extcss3_check_at_rule_is_valid_namespace(rule))
			)
		) {
			return rule->base_selector = rule->last_selector = NULL;
		}
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	selector = rule->last_selector;

	while ((selector != NULL) && _EXTCSS3_TYPE_EMPTY_EX(selector)) {
		// Trim the trailing whitespace and comments
		rule->last_selector = selector = selector->prev;
	}

	// Bad case: The last selector token is of type <comma>
	if (rule->last_selector->type == EXTCSS3_TYPE_COMMA) {
		return rule->base_selector = rule->last_selector = NULL;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	selector = rule->base_selector;

	while (selector != NULL) {
		// Remove whitespace and comments after...
		if (
			(selector->type == EXTCSS3_TYPE_BR_SO)		||
			(selector->type == EXTCSS3_TYPE_BR_RO)		||
			(selector->type == EXTCSS3_TYPE_COLON)		||
			(selector->type == EXTCSS3_TYPE_FUNCTION)	||
			(selector->type == EXTCSS3_TYPE_STRING)
		) {
			_extcss3_trim_right(selector, &rule->last_selector);
		}

		// Remove whitespace and comments before...
		if (
			(selector->type == EXTCSS3_TYPE_BR_SC)		||
			(selector->type == EXTCSS3_TYPE_BR_RC)		||
			(selector->type == EXTCSS3_TYPE_BAD_STRING)	||
			(
				(selector->type == EXTCSS3_TYPE_STRING) &&
				(EXTCSS3_SUCCESS != _extcss3_check_at_rule_is_valid_charset(intern, rule))
			) ||
			(
				(selector->type == EXTCSS3_TYPE_BR_RO) &&
				(selector->prev != NULL) &&
				(selector->prev->prev != NULL) &&
				(_EXTCSS3_TYPE_EMPTY_EX(selector->prev)) &&
				(selector->prev->prev->type != EXTCSS3_TYPE_IDENT)
			)
		) {
			_extcss3_trim_left(selector);
		}

		// Remove whitespace and comments around...
		if (
			(selector->flag == EXTCSS3_FLAG_AT_URL_STRING)	||
			(selector->type == EXTCSS3_TYPE_COMMA)			||
			(selector->type == EXTCSS3_TYPE_SUFFIX_MATCH)	||
			(selector->type == EXTCSS3_TYPE_SUBSTR_MATCH)	||
			(selector->type == EXTCSS3_TYPE_PREFIX_MATCH)	||
			(selector->type == EXTCSS3_TYPE_DASH_MATCH)		||
			(selector->type == EXTCSS3_TYPE_INCLUDE_MATCH)	||
			(selector->type == EXTCSS3_TYPE_COLUMN)			||
			(
				(selector->type == EXTCSS3_TYPE_DELIM) &&
				(
					(*selector->data.str == '>') ||
					(*selector->data.str == '+') ||
					(*selector->data.str == '~') ||
					(*selector->data.str == '=')
				)
			) ||
			(
				(selector->type == EXTCSS3_TYPE_BR_RC) &&
				(rule->base_selector->type == EXTCSS3_TYPE_AT_KEYWORD) &&
				(rule->base_selector->data.len == 6 /*strlen("@media")*/) &&
				(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(rule->base_selector->data.str, "@media", 6))
			)
		) {
			_extcss3_trim_around(selector, &rule->last_selector);
		}

		if (selector->type == EXTCSS3_TYPE_COMMENT) {
			if ((selector->prev != NULL) && (selector->prev->type == EXTCSS3_TYPE_WS)) {
				if ((selector->next != NULL) && (selector->next->type == EXTCSS3_TYPE_WS)) {
					_extcss3_trim_right(selector, &rule->last_selector);
				}
			}

			if (selector->user.str == NULL) {
				_extcss3_remove_token(&rule->base_selector, &selector, selector->next);
				continue;
			}
		}

		if (
			(selector->type == EXTCSS3_TYPE_BR_SC)	&&
			(selector->prev != NULL)				&&
			(selector->prev->user.str == NULL)		&&
			(selector->prev->type == EXTCSS3_TYPE_STRING)
		) {
			size_t i = 1, n = selector->prev->data.len - 1;

			while (i < n) {
				if (!EXTCSS3_IS_LETTER(selector->prev->data.str[i]) && (selector->prev->data.str[i] != '-')) {
					break;
				}

				i++;
			}

			if (i == n) {
				selector->prev->data.str += 1;
				selector->prev->data.len -= 2;
			}
		}

		if (selector->type == EXTCSS3_TYPE_COMMA) {
			// Bad case: Two <comma> tokens
			if ((selector->next != NULL) && (selector->next->type == EXTCSS3_TYPE_COMMA)) {
				return rule->base_selector = rule->last_selector = NULL;
			}
		}

		if (
			(selector->type == EXTCSS3_TYPE_NUMBER)		||
			(selector->type == EXTCSS3_TYPE_PERCENTAGE)	||
			(selector->type == EXTCSS3_TYPE_DIMENSION)
		) {
			if (
				(selector->prev != NULL) &&
				(selector->prev->type == EXTCSS3_TYPE_IDENT) &&
				(
					(
						(selector->prev->data.len == 1) &&
						(selector->prev->data.str[0] == 'n')
					) ||
					(
						(selector->prev->data.len == 2) &&
						((selector->prev->data.str[0] == '-') || (selector->prev->data.str[0] == '+')) &&
						(selector->prev->data.str[1] == 'n')
					)
				)
			) {
				preserve_sign = true;
			} else if (
				(selector->prev != NULL) &&
				(selector->prev->type == EXTCSS3_TYPE_DIMENSION) &&
				(selector->prev->info.len == 1) &&
				(selector->prev->info.str[0] == 'n')
			) {
				preserve_sign = true;
			} else {
				preserve_sign = false;
			}

			if (EXTCSS3_SUCCESS != extcss3_minify_numeric(selector, preserve_sign, false, error)) {
				return NULL;
			}
		}

		if ((selector->type == EXTCSS3_TYPE_AT_KEYWORD) && (selector->data.str[1] == '-')) {
			if ((vendor = intern->base_vendor) != NULL) {
				while (vendor != NULL) {
					if (
						(selector->data.len > vendor->name.len)				&&
						(selector->data.str[vendor->name.len + 1] == '-')	&&
						(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(selector->data.str + 2, vendor->name.str + 1, vendor->name.len - 1))
					) {
						return rule->base_selector = rule->last_selector = NULL;
					}

					vendor = vendor->next;
				}
			}
		} else if (
			(selector->type == EXTCSS3_TYPE_COLON)				&&
			(selector->prev != NULL)							&&
			(selector->prev->type == EXTCSS3_TYPE_COLON)		&&
			(selector->next != NULL)							&&
			(selector->next->type == EXTCSS3_TYPE_IDENT)		&&
			(selector->next->data.str[0] == '-')
		) {
			if ((vendor = intern->base_vendor) != NULL) {
				while (vendor != NULL) {
					if (
						(selector->next->data.len > vendor->name.len)		&&
						(selector->next->data.str[vendor->name.len] == '-')	&&
						(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(selector->next->data.str + 1, vendor->name.str + 1, vendor->name.len - 1))
					) {
						range_base = range_last = selector;

						while (range_base != NULL) {
							range_base = range_base->prev;

							if ((range_base == rule->base_selector) || (range_base->type == EXTCSS3_TYPE_COMMA)) {
								break;
							}
						}

						while (selector != NULL) {
							range_last = selector;

							if (selector == rule->last_selector) {
								break;
							} else if (selector->type == EXTCSS3_TYPE_COMMA) {
								_extcss3_trim_around(selector, &rule->last_selector);
								break;
							}

							selector = selector->next;
						}

						if ((range_base == rule->base_selector) && (range_last == rule->last_selector)) {
							// |------|
							// A------B
							// XXXXXXXX
							return rule->base_selector = rule->last_selector = NULL;
						} else if (range_base == rule->base_selector) {
							// |--,----|
							// A-B,----|
							// >>>>|---|
							rule->base_selector = range_last->next;
						} else if (range_last == rule->last_selector) {
							// |---,---|
							// |----,A-B
							// |---|<<<<
							rule->last_selector = range_base->prev;

							selector = rule->last_selector;
						} else {
							// |-,---,-|
							// |-,A-B,-|
							// |-,XXXX-|
							if ((range_base->type == EXTCSS3_TYPE_COMMA) && (range_last->type == EXTCSS3_TYPE_COMMA)) {
								range_base = range_base->next;
							}

							while ((range_base != NULL) && (range_base != range_last)) {
								_extcss3_remove_token(&rule->base_selector, &range_base, range_base->next);

								selector = range_base;
							}

							if ((range_base != NULL) && (range_base == range_last)) {
								selector = range_last->next;

								_extcss3_remove_token(&rule->base_selector, &range_base, NULL);
							}
						}

						break;
					}

					vendor = vendor->next;
				}
			}
		}

		if (selector == rule->last_selector) {
			break;
		}

		if (selector != NULL) {
			selector = selector->next;
		}
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	return rule->base_selector;
}

static inline extcss3_block *_extcss3_minify_declarations(extcss3_intern *intern, extcss3_block *block, unsigned int *error)
{
	extcss3_decl *curr, *temp;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	curr = block->decls;

	while (curr != NULL) {
		if (_extcss3_minify_declaration(intern, curr, error) == NULL) {
			if (*error > 0) {
				extcss3_release_block(block);
				return NULL;
			}

			if (curr->next != NULL) {
				curr->next->prev = curr->prev;
			}
			if (curr->prev != NULL) {
				curr->prev->next = curr->next;

				if (curr->next == NULL) {
					// Remove last semicolon in the previous declaration
					curr->prev->last = curr->prev->last->prev;
				}
			}

			if (block->decls == curr) {
				block->decls = curr->next;
			}

			temp = curr->next;
			extcss3_release_decl(curr);
			curr = temp;

			continue;
		}

		curr = curr->next;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if (block->decls == NULL) {
		extcss3_release_block(block);
		return NULL;
	}

	return block;
}

static inline extcss3_decl *_extcss3_minify_declaration(extcss3_intern *intern, extcss3_decl *decl, unsigned int *error)
{
	extcss3_token *name = NULL, *sep = NULL, *value = NULL, *temp;
	extcss3_vendor *vendor;
	int preserve_dimension = 0;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	name = _extcss3_get_decl_name(decl);

	if (name != NULL) {
		_extcss3_trim_around(name, &decl->last);

		sep = _extcss3_get_decl_sep(name, decl);

		if (sep != NULL) {
			_extcss3_trim_around(sep, &decl->last);

			value = _extcss3_get_decl_value(sep, decl);
		}
	}

	if (
		(name == NULL) || (sep == NULL) || (value == NULL) ||
		(value->type == EXTCSS3_TYPE_COLON) ||
		(value->type == EXTCSS3_TYPE_SEMICOLON)
	) {
		return NULL;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if ((name->type == EXTCSS3_TYPE_IDENT) && (name->data.str[0] == '-')) {
		if ((vendor = intern->base_vendor) != NULL) {
			while (vendor != NULL) {
				if (
					(name->data.len > vendor->name.len)			&&
					(name->data.str[vendor->name.len] == '-')	&&
					(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(name->data.str + 1, vendor->name.str + 1, vendor->name.len - 1))
				) {
					return NULL;
				}

				vendor = vendor->next;
			}
		}
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	// Remove the leading whitespace and comments
	while (decl->last != decl->base) {
		if (_EXTCSS3_TYPE_EMPTY_EX(decl->last)) {
			decl->last = decl->last->prev;
			continue;
		}

		break;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	while (value != NULL) {
		if ((value->type == EXTCSS3_TYPE_BAD_STRING) || (value->type == EXTCSS3_TYPE_BAD_URL)) {
			return NULL;
		}

		// Handle round braces while processing "calc()" function
		if (preserve_dimension) {
			if (value->type == EXTCSS3_TYPE_BR_RO) {
				preserve_dimension++;
			} else if (value->type == EXTCSS3_TYPE_BR_RC) {
				preserve_dimension--;
			}
		}

		// Remove whitespace and comments after...
		if (
			(value->type == EXTCSS3_TYPE_FUNCTION)	||
			(value->type == EXTCSS3_TYPE_BR_RO)		||
			(value->type == EXTCSS3_TYPE_STRING)
		) {
			_extcss3_trim_right(value, &decl->last);
		}

		// Remove whitespace and comments before...
		if (
			(value->type == EXTCSS3_TYPE_BR_RC) ||
			((value->type == EXTCSS3_TYPE_DELIM) && (*value->data.str == '!'))
		) {
			_extcss3_trim_left(value);

			if ((*value->data.str == '+') || (*value->data.str == '-')) {
				temp = value->next;

				while ((temp != NULL) &&_EXTCSS3_TYPE_EMPTY(temp->type)) {
					temp = temp->next;
				}

				if (
					(temp->type != EXTCSS3_TYPE_NUMBER)		&&
					(temp->type != EXTCSS3_TYPE_PERCENTAGE)	&&
					(temp->type != EXTCSS3_TYPE_DIMENSION)
				) {
					_extcss3_trim_right(value, &decl->last);
					value = temp;
					continue;
				}
			}
		}

		// Remove whitespace and comments around...
		if (
			(value->type == EXTCSS3_TYPE_COMMA) ||
			(value->type == EXTCSS3_TYPE_SEMICOLON) ||
			(
				(value->type == EXTCSS3_TYPE_DELIM) &&
				(
					(*value->data.str == '*') ||
					(*value->data.str == '/') ||
					(*value->data.str == '=')
				)
			)
		) {
			_extcss3_trim_around(value, &decl->last);
		}

		// Remove the trailing whitespace
		if (value == decl->last) {
			if (value->type == EXTCSS3_TYPE_WS) {
				decl->last = decl->last->prev;

				_extcss3_trim_left(value);
			}
		}

		/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

		if ((value->data.str[0] == '-')) {
			if ((vendor = intern->base_vendor) != NULL) {
				while (vendor != NULL) {
					if (
						(value->data.len > vendor->name.len)		&&
						(value->data.str[vendor->name.len] == '-')	&&
						(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(value->data.str + 1, vendor->name.str + 1, vendor->name.len - 1))
					) {
						return NULL;
					}

					vendor = vendor->next;
				}
			}
		}

		/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

		if (value->user.str != NULL) {
			value = _extcss3_get_decl_value(value, decl);
			continue;
		} else if ((decl->next == NULL) && (value->type == EXTCSS3_TYPE_SEMICOLON)) {
			decl->last = value->prev;
			break;
		} else if (value->type == EXTCSS3_TYPE_HASH) {
			if (EXTCSS3_SUCCESS != extcss3_minify_hash(value->data.str + 1, value->data.len - 1, value, error)) {
				return NULL;
			}
		} else if (EXTCSS3_SUCCESS == _extcss3_check_minify_color(name, value)) {
			if (EXTCSS3_SUCCESS != extcss3_minify_color(value, error)) {
				return NULL;
			}
		} else if (
			(value->type == EXTCSS3_TYPE_NUMBER)		||
			(value->type == EXTCSS3_TYPE_PERCENTAGE)	||
			(value->type == EXTCSS3_TYPE_DIMENSION)
		) {
			if (EXTCSS3_SUCCESS != extcss3_minify_numeric(value, false, (preserve_dimension > 0), error)) {
				return NULL;
			}
		} else if (value->type == EXTCSS3_TYPE_FUNCTION) {
			// RGB and RGBA version:
			//if (
			//	((value->data.len == 3) || (value->data.len == 4)) &&
			//	(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(value->data.str, "rgba", value->data.len))
			//)

			// RGB only version:
			if (
				(value->data.len == 3) &&
				(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(value->data.str, "rgb", 3))
			) {
				if (EXTCSS3_SUCCESS != extcss3_minify_function_rgb_a(&value, decl, error)) {
					return NULL;
				}

				if ((decl->next == NULL) && (value->type == EXTCSS3_TYPE_SEMICOLON)) {
					decl->last = value->prev;
					break;
				} else if (value->type == EXTCSS3_TYPE_BR_CC) {
					break;
				}
			} else if (
				(value->data.len == 4) &&
				(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(value->data.str, "calc", 4))
			) {
				preserve_dimension = 1;
			}
		}

		value = _extcss3_get_decl_value(value, decl);
	}

	return decl;
}

/* ==================================================================================================== */

static inline extcss3_token *_extcss3_get_decl_name(extcss3_decl *decl)
{
	extcss3_token *curr = decl->base, *term = decl->last->next;

	while ((curr != NULL) && (curr != term)) {
		if (_EXTCSS3_TYPE_EMPTY(curr->type)) {
			curr = curr->next;
			continue;
		} else if ((curr->type == EXTCSS3_TYPE_IDENT) || (curr->type == EXTCSS3_TYPE_DELIM)) {
			return curr;
		}

		break;
	}

	return NULL;
}

static inline extcss3_token *_extcss3_get_decl_sep(extcss3_token *name, extcss3_decl *decl)
{
	extcss3_token *curr = name->next, *term = decl->last->next;

	while ((curr != NULL) && (curr != term)) {
		if (
			_EXTCSS3_TYPE_EMPTY(curr->type) ||
			(
				(name->type == EXTCSS3_TYPE_DELIM) &&
				(curr->type == EXTCSS3_TYPE_IDENT) &&
				(curr == name->next)
			)
		) {
			curr = curr->next;
			continue;
		} else if (curr->type == EXTCSS3_TYPE_COLON) {
			return curr;
		}

		break;
	}

	return NULL;
}

static inline extcss3_token *_extcss3_get_decl_value(extcss3_token *last, extcss3_decl *decl)
{
	extcss3_token *curr = last->next, *term = decl->last->next;

	while ((curr != NULL) && (curr != term)) {
		if (curr->type == EXTCSS3_TYPE_WS) {
			curr = curr->next;
			continue;
		} else if (curr->type == EXTCSS3_TYPE_COMMENT) {
			if ((curr->prev != NULL) && (curr->prev->type == EXTCSS3_TYPE_WS)) {
				if ((curr->next != NULL) && (curr->next->type == EXTCSS3_TYPE_WS)) {
					_extcss3_trim_right(curr, &decl->last);
				}
			}

			if (curr->user.str == NULL) {
				if (
					((curr->prev != NULL) && (curr->prev->type == EXTCSS3_TYPE_WS)) ||
					((curr->next != NULL) && (curr->next->type == EXTCSS3_TYPE_WS))
				) {
					_extcss3_remove_token(&decl->base, &curr, curr->next);
					continue;
				}
			}
		}

		return curr;
	}

	return NULL;
}

/* ==================================================================================================== */

static inline void _extcss3_trim_left(extcss3_token *curr)
{
	extcss3_token *temp;

	while ((curr->prev != NULL) && _EXTCSS3_TYPE_EMPTY_EX(curr->prev)) {
		if ((temp = curr->prev->prev) != NULL) {
			temp->next = curr;
			extcss3_release_token(curr->prev);
			curr->prev = temp;
		}
	}
}

static inline void _extcss3_trim_right(extcss3_token *curr, extcss3_token **last)
{
	extcss3_token *temp;

	while ((curr->next != NULL) && _EXTCSS3_TYPE_EMPTY_EX(curr->next)) {
		if ((temp = curr->next->next) != NULL) {
			if ((last != NULL) && (*last != NULL) && (curr->next == *last)) {
				*last = curr;
			}

			temp->prev = curr;
			extcss3_release_token(curr->next);
			curr->next = temp;
		}
	}
}

static inline void _extcss3_trim_around(extcss3_token *curr, extcss3_token **last)
{
	_extcss3_trim_left(curr);
	_extcss3_trim_right(curr, last);
}

/* ==================================================================================================== */

static inline void _extcss3_remove_token(extcss3_token **base, extcss3_token **token, extcss3_token *repl)
{
	if ((base != NULL) && (*base == *token)) {
		*base = (*token)->next;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if ((*token)->next != NULL) {
		(*token)->next->prev = (*token)->prev;
	}
	if ((*token)->prev != NULL) {
		(*token)->prev->next = (*token)->next;
	}

	extcss3_release_token(*token);

	*token = repl;
}

/* ==================================================================================================== */

static inline bool _extcss3_check_at_rule_is_valid_charset(extcss3_intern *intern, extcss3_rule *rule)
{
	if (
		(rule->level == 0)														&&
		(rule->base_selector->data.len == 8 /* strlen("@charset") */)			&&
		(rule->base_selector->next)												&&
		(rule->base_selector->next->type == EXTCSS3_TYPE_WS)					&&
		(rule->base_selector->next->data.len == 1)								&&
		(rule->base_selector->next->data.str[0] == ' ')							&&
		(rule->base_selector->next->next)										&&
		(rule->base_selector->next->next->type == EXTCSS3_TYPE_STRING)			&&
		(rule->base_selector->next->next->data.str[0] == '"')					&&
		(rule->base_selector->next->next->next)									&&
		(rule->base_selector->next->next->next->type == EXTCSS3_TYPE_SEMICOLON)	&&
		(rule->base_selector->data.str == intern->copy.str)						&&
		(memcmp(rule->base_selector->data.str, "@charset", 8) == 0)
	) {
		return EXTCSS3_SUCCESS;
	}

	return EXTCSS3_FAILURE;
}

static inline bool _extcss3_check_at_rule_is_valid_import(extcss3_rule *rule)
{
	extcss3_rule *prev = rule->prev;

	while ((prev != NULL) && _EXTCSS3_TYPE_EMPTY(prev->base_selector->type)) {
		prev = prev->prev;
	}

	if (rule->level != 0) {
		return EXTCSS3_FAILURE;
	} else if (prev != NULL) {
		if (
			(prev->base_selector->data.len == 7 /* strlen("@import") */) &&
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(prev->base_selector->data.str, "@import", 7))
		) {
			// Do nothing. The previous "@import" was already checked.
		} else if (
			(prev->base_selector->data.len == 8 /* strlen("@charset") */) &&
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(prev->base_selector->data.str, "@charset", 8))
		) {
			// Do nothing. The previous "@charset" was already checked.
		} else {
			return EXTCSS3_FAILURE;
		}
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if (
		(rule->base_selector->data.len == 7) &&
		(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(rule->base_selector->data.str, "@import", 7))
	) {
		return EXTCSS3_SUCCESS;
	}

	return EXTCSS3_FAILURE;
}

static inline bool _extcss3_check_at_rule_is_valid_namespace(extcss3_rule *rule)
{
	extcss3_rule *prev = rule->prev;

	while ((prev != NULL) && _EXTCSS3_TYPE_EMPTY(prev->base_selector->type)) {
		prev = prev->prev;
	}

	if (rule->level != 0) {
		return EXTCSS3_FAILURE;
	} else if (prev != NULL) {
		if (
			(prev->base_selector->data.len == 10 /* strlen("@namespace") */) &&
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(prev->base_selector->data.str, "@namespace", 10))
		) {
			// Do nothing. The previous "@namespace" was already checked.
		} else if (
			(prev->base_selector->data.len == 7 /* strlen("@import") */) &&
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(prev->base_selector->data.str, "@import", 7))
		) {
			// Do nothing. The previous "@import" was already checked.
		} else if (
			(prev->base_selector->data.len == 8 /* strlen("@charset") */) &&
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(prev->base_selector->data.str, "@charset", 8))
		) {
			// Do nothing. The previous "@charset" was already checked.
		} else {
			return EXTCSS3_FAILURE;
		}
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if (
		(rule->base_selector->data.len == 10) &&
		(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(rule->base_selector->data.str, "@namespace", 10))
	) {
		return EXTCSS3_SUCCESS;
	}

	return EXTCSS3_FAILURE;
}

static inline bool _extcss3_check_minify_color(extcss3_token *name, extcss3_token *value)
{
	if (
		(name->data.len >= 10)				&&
		(value->user.str == NULL)			&&
		(value->type == EXTCSS3_TYPE_IDENT)	&&
		(value->data.len > 4)				&&
		(
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(name->data.str + name->data.len - 10, "background", 10)) ||
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(name->data.str + name->data.len - 10, "decoration", 10))
		)
	) {
		return EXTCSS3_SUCCESS;
	} else if (
		(name->data.len >= 6)				&&
		(value->user.str == NULL)			&&
		(value->type == EXTCSS3_TYPE_IDENT)	&&
		(value->data.len > 4)				&&
		(
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(name->data.str + name->data.len - 6, "shadow", 6)) ||
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(name->data.str + name->data.len - 6, "filter", 6))
		)
	) {
		return EXTCSS3_SUCCESS;
	} else if (
		(name->data.len >= 5)				&&
		(value->user.str == NULL)			&&
		(value->type == EXTCSS3_TYPE_IDENT)	&&
		(value->data.len > 4)				&&
		(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(name->data.str + name->data.len - 5, "color", 5))
	) {
		return EXTCSS3_SUCCESS;
	}

	return EXTCSS3_FAILURE;
}
