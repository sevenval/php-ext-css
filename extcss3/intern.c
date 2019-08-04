#include "intern.h"

#include <stdio.h>
#include <string.h>

/* ==================================================================================================== */

extcss3_intern *extcss3_reset_intern(extcss3_intern *intern, unsigned int *error)
{
	if (NULL == intern) {
		*error = EXTCSS3_ERR_NULL_PTR;
		return NULL;
	}

	mpz_pool_destroy(intern->pool);

	if (NULL == (intern->pool = mpz_pool_create())) {
		*error = EXTCSS3_ERR_MEMORY;
		return NULL;
	}

	intern->copy.str    = NULL;
	intern->base_token  = NULL;
	intern->base_vendor = NULL;

	return intern;
}

extcss3_intern *extcss3_create_intern(void)
{
	extcss3_intern *intern;

	if (NULL == (intern = malloc(sizeof(*intern)))) {
		return NULL;
	} else if (NULL == (intern->pool = mpz_pool_create())) {
		free(intern);
		return NULL;
	}

	intern->orig.str                  = NULL;
	intern->orig.len                  = 0;
	intern->copy.str                  = NULL;
	intern->copy.len                  = 0;

	intern->base_token                = NULL;
	intern->last_token                = NULL;
	intern->base_ctxt                 = NULL;
	intern->last_ctxt                 = NULL;
	intern->base_vendor               = NULL;
	intern->last_vendor               = NULL;

	intern->notifier.base             = NULL;
	intern->notifier.last             = NULL;
	intern->notifier.callback         = NULL;
	intern->notifier.destructor       = NULL;

	intern->modifier.string           = NULL;
	intern->modifier.bad_string       = NULL;
	intern->modifier.url              = NULL;
	intern->modifier.bad_url          = NULL;
	intern->modifier.comment          = NULL;
	intern->modifier.callback         = NULL;
	intern->modifier.destructor       = NULL;
	intern->modifier.user_strlen_diff = 0;

	intern->state.reader              = NULL;
	intern->state.cursor              = NULL;
	intern->state.writer              = NULL;
	intern->state.rest                = 0;

	return intern;
}

extcss3_vendor *extcss3_create_vendor(mpz_pool_t *pool)
{
	extcss3_vendor *vendor = (extcss3_vendor *)mpz_pmalloc(pool, sizeof(extcss3_vendor));

	if (NULL != vendor) {
		vendor->next     = NULL;
		vendor->name.str = NULL;
		vendor->name.len = 0;
	}

	return vendor;
}

extcss3_token *extcss3_create_token(mpz_pool_t *pool)
{
	extcss3_token *token = (extcss3_token *)mpz_pmalloc(pool, sizeof(extcss3_token));

	if (NULL != token) {
		token->data.str = NULL;
		token->data.len = 0;
		token->info.str = NULL;
		token->info.len = 0;
		token->user.str = NULL;
		token->user.len = 0;
		token->prev     = NULL;
		token->next     = NULL;
		token->type     = 0;
		token->flag     = 0;
	}

	return token;
}

extcss3_ctxt *extcss3_create_ctxt(mpz_pool_t *pool)
{
	extcss3_ctxt *ctxt = (extcss3_ctxt *)mpz_pmalloc(pool, sizeof(extcss3_ctxt));

	if (NULL != ctxt) {
		ctxt->level = 0;
		ctxt->token = NULL;
		ctxt->prev  = NULL;
		ctxt->next  = NULL;
	}

	return ctxt;
}

extcss3_rule *extcss3_create_rule(mpz_pool_t *pool)
{
	extcss3_rule *rule = (extcss3_rule *)mpz_pmalloc(pool, sizeof(extcss3_rule));

	if (NULL != rule) {
		rule->base_selector = NULL;
		rule->last_selector = NULL;
		rule->block         = NULL;
		rule->prev          = NULL;
		rule->next          = NULL;
		rule->level         = 0;
	}

	return rule;
}

extcss3_block *extcss3_create_block(mpz_pool_t *pool)
{
	extcss3_block *block = (extcss3_block *)mpz_pmalloc(pool, sizeof(extcss3_block));

	if (NULL != block) {
		block->base  = NULL;
		block->last  = NULL;
		block->rules = NULL;
		block->decls = NULL;
	}

	return block;
}

extcss3_decl *extcss3_create_decl(mpz_pool_t *pool)
{
	extcss3_decl *decl = (extcss3_decl *)mpz_pmalloc(pool, sizeof(extcss3_decl));

	if (NULL != decl) {
		decl->base = NULL;
		decl->last = NULL;
		decl->prev = NULL;
		decl->next = NULL;
	}

	return decl;
}

extcss3_sig *extcss3_create_signal(void)
{
	extcss3_sig *sig = (extcss3_sig *)malloc(sizeof(extcss3_sig));

	if (NULL != sig) {
		sig->type     = 0;
		sig->next     = NULL;
		sig->callable = NULL;
	}

	return sig;
}

/* ==================================================================================================== */

void extcss3_release_intern(extcss3_intern *intern)
{
	if (intern == NULL) {
		return;
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

	if (intern->notifier.base != NULL) {
		extcss3_release_signals_list(&intern->notifier);
	}

	mpz_pool_destroy(intern->pool);

	free(intern);
}

void extcss3_release_vendor(mpz_pool_t *pool, extcss3_vendor *vendor)
{
	if (vendor == NULL) {
		return;
	}

	if (vendor->name.str != NULL) {
		mpz_free(pool, vendor->name.str);
	}

	mpz_free(pool, vendor);
}

void extcss3_release_vendors_list(mpz_pool_t *pool, extcss3_vendor *list)
{
	extcss3_vendor *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_vendor(pool, list->next);
		list->next = next;
	}

	extcss3_release_vendor(pool, list);
}

void extcss3_release_token(mpz_pool_t *pool, extcss3_token *token)
{
	if (token == NULL) {
		return;
	}

	if (token->user.str != NULL) {
		mpz_free(pool, token->user.str);
	}

	mpz_free(pool, token);
}

void extcss3_release_tokens_list(mpz_pool_t *pool, extcss3_token *list)
{
	extcss3_token *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_token(pool, list->next);
		list->next = next;
	}

	extcss3_release_token(pool, list);
}

void extcss3_release_ctxt(mpz_pool_t *pool, extcss3_ctxt *ctxt)
{
	if (ctxt == NULL) {
		return;
	}

	mpz_free(pool, ctxt);
}

void extcss3_release_ctxts_list(mpz_pool_t *pool, extcss3_ctxt *list)
{
	extcss3_ctxt *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_ctxt(pool, list->next);
		list->next = next;
	}

	extcss3_release_ctxt(pool, list);
}

void extcss3_release_rule(mpz_pool_t *pool, extcss3_rule *rule)
{
	if (rule == NULL) {
		return;
	}

	if (rule->block != NULL) {
		extcss3_release_block(pool, rule->block);
	}

	mpz_free(pool, rule);
}

void extcss3_release_rules_list(mpz_pool_t *pool, extcss3_rule *list)
{
	extcss3_rule *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_rule(pool, list->next);
		list->next = next;
	}

	extcss3_release_rule(pool, list);
}

void extcss3_release_block(mpz_pool_t *pool, extcss3_block *block)
{
	if (block == NULL) {
		return;
	}

	if (block->rules != NULL) {
		extcss3_release_rules_list(pool, block->rules);
	}

	if (block->decls != NULL) {
		extcss3_release_decls_list(pool, block->decls);
	}

	mpz_free(pool, block);
}

void extcss3_release_decl(mpz_pool_t *pool, extcss3_decl *decl)
{
	if (decl == NULL) {
		return;
	}

	mpz_free(pool, decl);
}

void extcss3_release_decls_list(mpz_pool_t *pool, extcss3_decl *list)
{
	extcss3_decl *next;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_decl(pool, list->next);
		list->next = next;
	}

	extcss3_release_decl(pool, list);
}

void extcss3_release_signal(extcss3_not *notifier, extcss3_sig *sig)
{
	if (sig == NULL) {
		return;
	}

	if ((sig->callable != NULL) && (notifier != NULL) && (notifier->destructor != NULL)) {
		notifier->destructor(sig->callable);
	}

	free(sig);
}

void extcss3_release_signals_list(extcss3_not *notifier)
{
	extcss3_sig *next, *list = notifier->base;

	if (list == NULL) {
		return;
	}

	while (list->next != NULL) {
		next = list->next->next;
		extcss3_release_signal(notifier, list->next);
		list->next = next;
	}

	extcss3_release_signal(notifier, list);
}

/* ==================================================================================================== */

bool extcss3_set_css_string(extcss3_intern *intern, char *css, size_t len, unsigned int *error)
{
	if ((intern == NULL) || (css == NULL)) {
		*error = EXTCSS3_ERR_NULL_PTR;

		return EXTCSS3_FAILURE;
	} else if (intern->copy.str != NULL) {
		mpz_free(intern->pool, intern->copy.str);
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
	intern->copy.str = (char *)mpz_pmalloc(intern->pool, (intern->copy.len + 1) * sizeof(char));

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

bool extcss3_set_notifier(extcss3_intern *intern, unsigned int type, void *callable, unsigned int *error)
{
	extcss3_sig *sig;

	if ((intern == NULL) || (intern->notifier.destructor == NULL) || (intern->notifier.callback == NULL) || (callable == NULL)) {
		*error = EXTCSS3_ERR_NULL_PTR;

		return EXTCSS3_FAILURE;
	}

	if ((sig = extcss3_create_signal()) == NULL) {
		*error = EXTCSS3_ERR_MEMORY;

		return EXTCSS3_FAILURE;
	}

	sig->type     = type;
	sig->callable = callable;

	if (intern->notifier.base == NULL) {
		intern->notifier.base = sig;
	} else {
		intern->notifier.last->next = sig;
	}

	intern->notifier.last = sig;

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
	intern->last_vendor->name.str = (char *)mpz_pmalloc(intern->pool, intern->last_vendor->name.len * sizeof(char));

	if (intern->last_vendor->name.str == NULL) {
		*error = EXTCSS3_ERR_MEMORY;

		return EXTCSS3_FAILURE;
	}

	memcpy(intern->last_vendor->name.str, name, intern->last_vendor->name.len);

	return EXTCSS3_SUCCESS;
}
