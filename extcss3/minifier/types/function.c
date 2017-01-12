#include "function.h"
#include "hash.h"
#include "../../intern.h"
#include "../../utils.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* ==================================================================================================== */

bool extcss3_minify_function_rgb_a(extcss3_token **token, extcss3_decl *decl, int *error)
{
	extcss3_token *temp, *curr = (*token)->next;
	double value;
	char hex[9];
	unsigned short int idx = 0, percentages = 0, numbers = 0;
	bool validity = true;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	while ((curr->next != NULL) && (curr->type != EXTCSS3_TYPE_BR_RC)) {
		if ((curr->type == EXTCSS3_TYPE_NUMBER) || (curr->type == EXTCSS3_TYPE_PERCENTAGE)) {
			if (!validity || (((*token)->data.len == 3) && (idx > 4)) || (((*token)->data.len == 4) && (idx > 6))) {
				return EXTCSS3_FAILURE; // Too much values
			}

			value = atof(curr->data.str);

			if (value < 0) {
				return EXTCSS3_FAILURE; // Invalid value: 0 - 255
			} else if ((curr->type == EXTCSS3_TYPE_NUMBER) && (value > 255)) {
				return EXTCSS3_FAILURE; // Invalid value: 0 - 255
			} else if ((curr->type == EXTCSS3_TYPE_PERCENTAGE) && (value > 100)) {
				return EXTCSS3_FAILURE; // Invalid value: 0% - 100%
			} else if ((idx > 4) && (curr->type == EXTCSS3_TYPE_NUMBER) && ((int)value > 1)) {
				return EXTCSS3_FAILURE; // Invalid value: 0 - 1
			} else if ((idx < 6) && ((int)value != value)) {
				return EXTCSS3_FAILURE; // Invalid value: 0 - 1
			}

			if (curr->type == EXTCSS3_TYPE_PERCENTAGE) {
				value = roundf(value * 2.55);

				percentages++;
			} else {
				if (idx > 4) {
					value = roundf(value * 255);
				}

				numbers++;
			}

			if (percentages && numbers) {
				return EXTCSS3_FAILURE; // Invalid value: No mixed types!
			}

			sprintf(&hex[idx], "%02x", (int)value);
			idx += 2;

			validity = false;
		} else if (!_EXTCSS3_TYPE_EMPTY(curr->type)) {
			if (validity) {
				return EXTCSS3_FAILURE;
			} else if (curr->type == EXTCSS3_TYPE_COMMA) {
				validity = true;
			}
		}

		if (curr == decl->last) {
			break;
		}

		curr = curr->next;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if ((((*token)->data.len == 3) && (idx == 6)) || (((*token)->data.len == 4) && (idx == 8))) {
		if (EXTCSS3_SUCCESS != extcss3_minify_hash(hex, idx, *token, error)) {
			return EXTCSS3_FAILURE;
		}

		// It was nothing to do for the extcss3_minify_hash() function
		if ((*token)->user.str == NULL) {
			(*token)->user.len = idx + 1;

			if (((*token)->user.str = (char *)calloc(1, sizeof(char) * (*token)->user.len)) == NULL) {
				*error = EXTCSS3_ERR_MEMORY;
				return EXTCSS3_FAILURE;
			}

			(*token)->user.str[0] = '#';
			memcpy((*token)->user.str + 1, hex, idx);
		}

		(*token)->type = EXTCSS3_TYPE_HASH;

		while (curr != *token) {
			if (curr == decl->last) {
				decl->last = curr->prev;
			}

			curr->next->prev = curr->prev;
			curr->prev->next = curr->next;

			temp = curr->prev;
			extcss3_release_token(curr);
			curr = temp;
		}

		if ((*token != decl->last) && ((*token)->next != decl->last) && !_EXTCSS3_TYPE_EMPTY((*token)->next->type)) {
			temp = (*token)->next;

			// Create a new whitespace token
			if (((*token)->next = extcss3_create_token()) == NULL) {
				extcss3_release_tokens_list(temp);

				*error = EXTCSS3_ERR_MEMORY;
				return EXTCSS3_FAILURE;
			}

			(*token)->next->type = EXTCSS3_TYPE_WS;
			(*token)->next->prev = (*token);
			(*token)->next->next = temp;
			temp->prev = (*token)->next;
		}

		*token = (*token)->next;

		return EXTCSS3_SUCCESS;
	}

	return EXTCSS3_FAILURE;
}
