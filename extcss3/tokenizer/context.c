#include "context.h"
#include "../intern.h"

/* ==================================================================================================== */

static inline void _extcss3_ctxt_parent(extcss3_intern *intern)
{
	extcss3_ctxt *prev;

	if ((intern->last_ctxt != NULL) && (intern->last_ctxt->prev != NULL)) {
		prev = intern->last_ctxt->prev;
		prev->next = NULL;
		extcss3_release_ctxt(intern->pool, intern->last_ctxt);
		intern->last_ctxt = prev;
	}
}

bool extcss3_ctxt_update(extcss3_intern *intern, unsigned int *error)
{
	if ((intern == NULL) || (intern->last_token == NULL) || (intern->last_ctxt == NULL)) {
		*error = EXTCSS3_ERR_NULL_PTR;

		return EXTCSS3_FAILURE;
	}

	switch (intern->last_token->type) {
		case EXTCSS3_TYPE_AT_KEYWORD:
		case EXTCSS3_TYPE_FUNCTION:
		case EXTCSS3_TYPE_BR_RO:
		case EXTCSS3_TYPE_BR_SO:
		case EXTCSS3_TYPE_BR_CO:
		{
			if ((intern->last_ctxt->next = extcss3_create_ctxt(intern->pool)) == NULL) {
				*error = EXTCSS3_ERR_MEMORY;

				return EXTCSS3_FAILURE;
			}

			intern->last_ctxt->next->level = intern->last_ctxt->level + 1;
			intern->last_ctxt->next->token = intern->last_token;
			intern->last_ctxt->next->prev = intern->last_ctxt;
			intern->last_ctxt = intern->last_ctxt->next;

			break;
		}
		case EXTCSS3_TYPE_BR_RC:
		case EXTCSS3_TYPE_BR_SC:
		{
			_extcss3_ctxt_parent(intern);

			break;
		}
		case EXTCSS3_TYPE_BR_CC:
		{
			if (
				(intern->last_ctxt->prev != NULL) &&
				(intern->last_ctxt->prev->token != NULL) &&
				(intern->last_ctxt->prev->token->type == EXTCSS3_TYPE_AT_KEYWORD)
			) {
				_extcss3_ctxt_parent(intern);
				_extcss3_ctxt_parent(intern);
			} else {
				_extcss3_ctxt_parent(intern);
			}

			break;
		}
		case EXTCSS3_TYPE_SEMICOLON:
		{
			if ((intern->last_ctxt->token != NULL) && (intern->last_ctxt->token->type == EXTCSS3_TYPE_AT_KEYWORD)) {
				_extcss3_ctxt_parent(intern);
			}

			break;
		}
	}

	return EXTCSS3_SUCCESS;
}
