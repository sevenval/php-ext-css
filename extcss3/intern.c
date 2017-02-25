#include "intern.h"

#include <stdio.h>
#include <string.h>

/* ==================================================================================================== */

extcss3_intern *extcss3_create_intern(void)
{
	return (extcss3_intern *)calloc(1, sizeof(extcss3_intern));
}

extcss3_vendor *extcss3_create_vendor(void)
{
	return (extcss3_vendor *)calloc(1, sizeof(extcss3_vendor));
}

extcss3_token *extcss3_create_token(void)
{
	return (extcss3_token *)calloc(1, sizeof(extcss3_token));
}

extcss3_ctxt *extcss3_create_ctxt(void)
{
	return (extcss3_ctxt *)calloc(1, sizeof(extcss3_ctxt));
}

extcss3_rule *extcss3_create_rule(void)
{
	return (extcss3_rule *)calloc(1, sizeof(extcss3_rule));
}

extcss3_block *extcss3_create_block(void)
{
	return (extcss3_block *)calloc(1, sizeof(extcss3_block));
}

extcss3_decl *extcss3_create_decl(void)
{
	return (extcss3_decl *)calloc(1, sizeof(extcss3_decl));
}

/* ==================================================================================================== */

void extcss3_release_intern(extcss3_intern *intern)
{
	if (intern == NULL) {
		return;
	}

	if (intern->copy.str != NULL) {
		free(intern->copy.str);
	}

	if (intern->base_vendor != NULL) {
		extcss3_release_vendors_list(intern->base_vendor);
	}

	if (intern->base_token != NULL) {
		extcss3_release_tokens_list(intern->base_token);
	}

	if (intern->base_ctxt != NULL) {
		extcss3_release_ctxts_list(intern->base_ctxt);
	}

	if (intern->modifier.destructor != NULL) {
		if (intern->modifier.string != NULL) {
			intern->modifier.destructor(intern->modifier.string);
		}

		if (intern->modifier.bad_string != NULL) {
			intern->modifier.destructor(intern->modifier.bad_string);
		}

		if (intern->modifier.url != NULL) {
			intern->modifier.destructor(intern->modifier.url);
		}

		if (intern->modifier.bad_url != NULL) {
			intern->modifier.destructor(intern->modifier.bad_url);
		}

		if (intern->modifier.comment != NULL) {
			intern->modifier.destructor(intern->modifier.comment);
		}
	}

	free(intern);
}

void extcss3_release_vendor(extcss3_vendor *vendor)
{
	if (vendor == NULL) {
		return;
	}

	if (vendor->name.str != NULL) {
		free(vendor->name.str);
	}

	free(vendor);
}

void extcss3_release_vendors_list(extcss3_vendor *list)
{
	extcss3_vendor *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_vendor(list->next);
		list->next = next;
	}

	extcss3_release_vendor(list);
}

void extcss3_release_token(extcss3_token *token)
{
	if (token == NULL) {
		return;
	}

	if (token->user.str != NULL) {
		free(token->user.str);
	}

	free(token);
}

void extcss3_release_tokens_list(extcss3_token *list)
{
	extcss3_token *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_token(list->next);
		list->next = next;
	}

	extcss3_release_token(list);
}

void extcss3_release_ctxt(extcss3_ctxt *ctxt)
{
	if (ctxt == NULL) {
		return;
	}

	free(ctxt);
}

void extcss3_release_ctxts_list(extcss3_ctxt *list)
{
	extcss3_ctxt *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_ctxt(list->next);
		list->next = next;
	}

	extcss3_release_ctxt(list);
}

void extcss3_release_rule(extcss3_rule *rule)
{
	if (rule == NULL) {
		return;
	}

	if (rule->block != NULL) {
		extcss3_release_block(rule->block);
	}

	free(rule);
}

void extcss3_release_rules_list(extcss3_rule *list)
{
	extcss3_rule *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_rule(list->next);
		list->next = next;
	}

	extcss3_release_rule(list);
}

void extcss3_release_block(extcss3_block *block)
{
	if (block == NULL) {
		return;
	}

	if (block->rules != NULL) {
		extcss3_release_rules_list(block->rules);
	}

	if (block->decls != NULL) {
		extcss3_release_decls_list(block->decls);
	}

	free(block);
}

void extcss3_release_decl(extcss3_decl *decl)
{
	if (decl == NULL) {
		return;
	}

	free(decl);
}

void extcss3_release_decls_list(extcss3_decl *list)
{
	extcss3_decl *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_decl(list->next);
		list->next = next;
	}

	extcss3_release_decl(list);
}

/* ==================================================================================================== */

bool extcss3_set_css_string(extcss3_intern *intern, char *css, size_t len, unsigned int *error)
{
	if ((intern == NULL) || (css == NULL)) {
		*error = EXTCSS3_ERR_NULL_PTR;

		return EXTCSS3_FAILURE;
	} else if (intern->copy.str != NULL) {
		free(intern->copy.str);
	}

	intern->orig.str = css;
	intern->orig.len = len;

	/* Force EOF at the end of the original string */
	intern->orig.str[intern->orig.len] = '\0';

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	/**
	 * We need at least 3x more memory for the copy-string to be able to put the
	 * U+FFFD REPLACEMENT CHARACTERS without expensive memory movement operations.
	 */

	intern->copy.len = len * 3;
	intern->copy.str = (char *)calloc(intern->copy.len + 1, sizeof(char));

	if (intern->copy.str == NULL) {
		*error = EXTCSS3_ERR_MEMORY;

		return EXTCSS3_FAILURE;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	intern->state.rest = len;
	intern->state.cursor = intern->orig.str;
	intern->state.reader = intern->state.writer = intern->copy.str;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	intern->base_token = intern->last_token = NULL;
	intern->base_ctxt = intern->last_ctxt = NULL;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	return EXTCSS3_SUCCESS;
}

bool extcss3_set_modifier(extcss3_intern *intern, unsigned int type, void *callable, unsigned int *error)
{
	if ((intern == NULL) || (intern->modifier.destructor == NULL) || (intern->modifier.callback == NULL) || (callable == NULL)) {
		*error = EXTCSS3_ERR_NULL_PTR;

		return EXTCSS3_FAILURE;
	}

	switch (type) {
		case EXTCSS3_TYPE_STRING:
		{
			if (intern->modifier.string != NULL) {
				intern->modifier.destructor(intern->modifier.string);
			}

			intern->modifier.string = callable;
			break;
		}
		case EXTCSS3_TYPE_BAD_STRING:
		{
			if (intern->modifier.bad_string != NULL) {
				intern->modifier.destructor(intern->modifier.bad_string);
			}

			intern->modifier.bad_string = callable;
			break;
		}
		case EXTCSS3_TYPE_URL:
		{
			if (intern->modifier.url != NULL) {
				intern->modifier.destructor(intern->modifier.url);
			}

			intern->modifier.url = callable;
			break;
		}
		case EXTCSS3_TYPE_BAD_URL:
		{
			if (intern->modifier.bad_url != NULL) {
				intern->modifier.destructor(intern->modifier.bad_url);
			}

			intern->modifier.bad_url = callable;
			break;
		}
		case EXTCSS3_TYPE_COMMENT:
		{
			if (intern->modifier.comment != NULL) {
				intern->modifier.destructor(intern->modifier.comment);
			}

			intern->modifier.comment = callable;
			break;
		}
		default:
		{
			*error = EXTCSS3_ERR_INV_PARAM;

			return EXTCSS3_FAILURE;
		}
	}

	return EXTCSS3_SUCCESS;
}

bool extcss3_set_vendor_string(extcss3_intern *intern, char *name, size_t len, unsigned int *error)
{
	if ((intern == NULL) || (name == NULL)) {
		*error = EXTCSS3_ERR_NULL_PTR;

		return EXTCSS3_FAILURE;
	}

	intern->last_vendor->name.len = len;
	intern->last_vendor->name.str = (char *)calloc(intern->last_vendor->name.len, sizeof(char));

	if (intern->last_vendor->name.str == NULL) {
		*error = EXTCSS3_ERR_MEMORY;

		return EXTCSS3_FAILURE;
	}

	memcpy(intern->last_vendor->name.str, name, intern->last_vendor->name.len);

	return EXTCSS3_SUCCESS;
}
