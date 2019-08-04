#include "numeric.h"
#include "../../utils.h"

#include <string.h>
#include <strings.h>

/* ==================================================================================================== */

const char *extcss3_numeric_dimensions[19] = {
    "em",
	"ex",
	"ch",
	"rem",

	"vw",
	"vh",
	"vmin",
	"vmax",

	"cm",
	"mm",
	"q",
	"in",
	"pc",
	"pt",
	"px",

	"deg",
	"grad",
	"rad",
	"turn"
};

static inline bool _extcss3_minify_numeric_preserve_dimension(extcss3_token *token)
{
	unsigned int i, elements;

	if (token->type != EXTCSS3_TYPE_DIMENSION) {
		return EXTCSS3_FAILURE;
	}

	elements = sizeof(extcss3_numeric_dimensions) / sizeof(extcss3_numeric_dimensions[0]);

	for (i = elements; i--; ) {
		if (
			(strlen(extcss3_numeric_dimensions[i]) == token->info.len) &&
			(EXTCSS3_SUCCESS == extcss3_ascii_strncasecmp(token->info.str, extcss3_numeric_dimensions[i], token->info.len))
		) {
			return EXTCSS3_FAILURE;
		}
	}

	return EXTCSS3_SUCCESS;
}

/* ==================================================================================================== */

bool extcss3_minify_numeric(extcss3_intern *intern, extcss3_token *token, bool preserve_sign, bool preserve_dimension, unsigned int *error)
{
	char         *base, *last;
	double        num;
	unsigned int  val_is_signed = 0;

	if (token == NULL) {
		*error = EXTCSS3_ERR_NULL_PTR;
		return EXTCSS3_FAILURE;
	} else if (
		(token->type != EXTCSS3_TYPE_NUMBER)     &&
		(token->type != EXTCSS3_TYPE_PERCENTAGE) &&
		(token->type != EXTCSS3_TYPE_DIMENSION)
	) {
		return EXTCSS3_SUCCESS;
	} else if (token->data.len <= 1) {
		return EXTCSS3_SUCCESS; // No minifying potential
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if ((num = atof(token->data.str)) == 0) {
		if (preserve_sign && ((*token->data.str == '-') || (*token->data.str == '+'))) {
			token->user.len = 1 + token->info.len + (int)preserve_sign;
		} else {
			token->user.len = 1 + token->info.len;
		}

		if ((token->user.str = (char *)mpz_pmalloc(intern->pool, (token->user.len + token->info.len) * sizeof(char))) == NULL) {
			*error = EXTCSS3_ERR_MEMORY;
			return EXTCSS3_FAILURE;
		}

		if (preserve_sign && ((*token->data.str == '-') || (*token->data.str == '+'))) {
			token->user.str[0] = *token->data.str;
			token->user.str[0 + (int)preserve_sign] = '0';
		} else {
			*token->user.str = '0';
		}
	} else {
		if ((*token->data.str == '-') || (preserve_sign && (*token->data.str == '+'))) {
			val_is_signed = 1;
		} else if (*token->data.str == '+') {
			token->data.str++;
			token->data.len--;
		}

		// Remove trailing and leading '0' digits
		base = token->data.str + val_is_signed;
		while (*base == '0') {
			base++;
		}

		last = token->data.str + token->data.len - token->info.len;
		while (last != base) {
			if (*last == '.') {
				break;
			}
			last--;
		}

		if (*last == '.') {
			last = token->data.str + token->data.len - token->info.len;

			while ((last-1)[0] == '0') {
				last--;
			}
			if ((last-1)[0] == '.') {
				last--;
			}
		} else {
			last = token->data.str + token->data.len - token->info.len;
		}

		if (base == (token->data.str + val_is_signed)) {
			if (last == (token->data.str + token->data.len - token->info.len)) {
				return EXTCSS3_SUCCESS; // Nothing to do
			}
		}

		token->user.len = last - base + val_is_signed + token->info.len;

		if ((token->user.str = (char *)mpz_pmalloc(intern->pool, token->user.len * sizeof(char))) == NULL) {
			*error = EXTCSS3_ERR_MEMORY;
			return EXTCSS3_FAILURE;
		}

		if (val_is_signed > 0) {
			*token->user.str = *token->data.str;
		}

		memcpy(token->user.str + val_is_signed, base, token->user.len - val_is_signed - token->info.len);
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if (!preserve_dimension) {
		preserve_dimension = (num == 0) && (token->type == EXTCSS3_TYPE_PERCENTAGE);
	}

	if (preserve_dimension || (num != 0) || (EXTCSS3_SUCCESS == _extcss3_minify_numeric_preserve_dimension(token))) {
		memcpy(token->user.str + token->user.len - token->info.len, token->info.str, token->info.len);
	} else if (token->user.str != NULL) {
		token->user.len -= token->info.len;
	}

	return EXTCSS3_SUCCESS;
}
