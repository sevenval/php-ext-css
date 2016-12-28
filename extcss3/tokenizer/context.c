#include "context.h"
#include "../intern.h"

/* ==================================================================================================== */

int inline extcss3_ctxt_update(extcss3_intern *intern)
{
	switch (intern->last_token->type) {
		case EXTCSS3_TYPE_AT_KEYWORD:
		case EXTCSS3_TYPE_FUNCTION:
		case EXTCSS3_TYPE_BR_RO:
		case EXTCSS3_TYPE_BR_SO:
		case EXTCSS3_TYPE_BR_CO:
		{
			if ((intern->last_ctxt->next = extcss3_create_ctxt()) == NULL) {
				return EXTCSS3_ERR_MEMORY;
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
			extcss3_ctxt_parent(intern);

			break;
		}
		case EXTCSS3_TYPE_BR_CC:
		{
			if (
				intern->last_ctxt->prev &&
				intern->last_ctxt->prev->token &&
				(intern->last_ctxt->prev->token->type == EXTCSS3_TYPE_AT_KEYWORD)
			) {
				extcss3_ctxt_parent(intern);
				extcss3_ctxt_parent(intern);
			} else if (intern->last_ctxt) {
				extcss3_ctxt_parent(intern);
			}

			break;
		}
		case EXTCSS3_TYPE_SEMICOLON:
		{
			if (intern->last_ctxt->token && (intern->last_ctxt->token->type == EXTCSS3_TYPE_AT_KEYWORD)) {
				extcss3_ctxt_parent(intern);
			}

			break;
		}
	}

	return 0;
}

void inline extcss3_ctxt_parent(extcss3_intern *intern)
{
	extcss3_ctxt *prev;

	if (intern->last_ctxt->prev) {
		prev = intern->last_ctxt->prev;
		prev->next = NULL;
		extcss3_release_ctxt(intern->last_ctxt, false);
		intern->last_ctxt = prev;
	}
}
