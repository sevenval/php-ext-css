#include "intern.h"
#include "utils.h"

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
		extcss3_release_vendor(intern->base_vendor, true);
	}

	if (intern->base_token != NULL) {
		extcss3_release_token(intern->base_token, true);
	}

	if (intern->base_ctxt != NULL) {
		extcss3_release_ctxt(intern->base_ctxt, true);
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

void extcss3_release_vendor(extcss3_vendor *vendor, bool recursive)
{
	if (vendor == NULL) {
		return;
	}

	if (vendor->name.str != NULL) {
		free(vendor->name.str);
	}

	if (recursive && (vendor->next != NULL)) {
		extcss3_release_vendor(vendor->next, recursive);
	}

	free(vendor);
}

void extcss3_release_token(extcss3_token *token, bool recursive)
{
	if (token == NULL) {
		return;
	}

	if (token->user.str != NULL) {
		free(token->user.str);
	}

	if (recursive && (token->next != NULL)) {
		extcss3_release_token(token->next, recursive);
	}

	free(token);
}

void extcss3_release_ctxt(extcss3_ctxt *ctxt, bool recursive)
{
	if (ctxt == NULL) {
		return;
	}

	if (recursive && (ctxt->next != NULL)) {
		extcss3_release_ctxt(ctxt->next, recursive);
	}

	free(ctxt);
}

void extcss3_release_rule(extcss3_rule *rule, bool recursive)
{
	if (rule == NULL) {
		return;
	}

	if (rule->block != NULL) {
		extcss3_release_block(rule->block);
	}

	if (recursive && (rule->next != NULL)) {
		extcss3_release_rule(rule->next, recursive);
	}

	free(rule);
}

void extcss3_release_block(extcss3_block *block)
{
	if (block == NULL) {
		return;
	}

	if (block->rules != NULL) {
		extcss3_release_rule(block->rules, true);
	}

	if (block->decls != NULL) {
		extcss3_release_decl(block->decls, true);
	}

	free(block);
}

void extcss3_release_decl(extcss3_decl *decl, bool recursive)
{
	if (decl == NULL) {
		return;
	}

	if (recursive && (decl->next != NULL)) {
		extcss3_release_decl(decl->next, recursive);
	}

	free(decl);
}

/* ==================================================================================================== */

bool extcss3_set_css_string(extcss3_intern *intern, char *css, size_t len)
{
	if ((intern == NULL) || (css == NULL)) {
		return false;
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
	intern->copy.str = (char *)calloc(1, sizeof(char) * (intern->copy.len + 1));

	if (intern->copy.str == NULL) {
		return false;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	intern->state.rest = len;
	intern->state.cursor = intern->orig.str;
	intern->state.reader = intern->state.writer = intern->copy.str;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	intern->base_token = intern->last_token = NULL;
	intern->base_ctxt = intern->last_ctxt = NULL;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	return true;
}

bool extcss3_set_modifier(extcss3_intern *intern, short int type, void *callable)
{
	if ((intern == NULL) || (intern->modifier.destructor == NULL) || (intern->modifier.callback == NULL) || (callable == NULL)) {
		return false;
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
			return false;
	}

	return true;
}

bool extcss3_set_vendor_string(extcss3_intern *intern, char *name, size_t len)
{
	if ((intern == NULL) || (name == NULL)) {
		return false;
	}

	intern->last_vendor->name.len = len;
	intern->last_vendor->name.str = (char *)calloc(1, sizeof(char) * (intern->last_vendor->name.len));

	if (intern->last_vendor->name.str == NULL) {
		return false;
	}

	memcpy(intern->last_vendor->name.str, name, intern->last_vendor->name.len);

	return true;
}
