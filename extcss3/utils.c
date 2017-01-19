#include "utils.h"

/* ==================================================================================================== */

/**
 * Return the size of the given (UTF-8 multibyte) character in bytes
 */
inline unsigned short int extcss3_char_len(char c)
{
	unsigned short int i = 7, j = 0;

	while ((i >= 0) && ((c >> i) & 1)) {
		i--;
		j++;
	}

	return (j == 0 ? 1 : j);
}

/**
 * Compare up to "n" ASCII characters of "str1" and "str2" without sensitivity to case
 */
bool extcss3_ascii_strncasecmp(const char *str1, const char *str2, unsigned char n)
{
	unsigned char i;

	if ((str1 == NULL) || (str2 == NULL)) {
		return EXTCSS3_FAILURE;
	}

	for (i = 0; i < n; i++) {
		if ((str1[i] == '\0') || (str2[i] == '\0')) {
			return str1[i] == str2[i];
		} else if (str1[i] != str2[i]) {
			if (!EXTCSS3_CHARS_EQ(str1[i], str2[i])) {
				return EXTCSS3_FAILURE;
			}
		}
	}

	return EXTCSS3_SUCCESS;
}

/**
 * Get the string representation of a token type
 */
char *extcss3_get_type_str(unsigned short int type)
{
	switch (type) {
		case EXTCSS3_TYPE_IDENT:
			return EXTCSS3_TYPE_IDENT_STR;
		case EXTCSS3_TYPE_FUNCTION:
			return EXTCSS3_TYPE_FUNCTION_STR;
		case EXTCSS3_TYPE_AT_KEYWORD:
			return EXTCSS3_TYPE_AT_KEYWORD_STR;
		case EXTCSS3_TYPE_HASH:
			return EXTCSS3_TYPE_HASH_STR;
		case EXTCSS3_TYPE_STRING:
			return EXTCSS3_TYPE_STRING_STR;
		case EXTCSS3_TYPE_BAD_STRING:
			return EXTCSS3_TYPE_BAD_STRING_STR;
		case EXTCSS3_TYPE_URL:
			return EXTCSS3_TYPE_URL_STR;
		case EXTCSS3_TYPE_BAD_URL:
			return EXTCSS3_TYPE_BAD_URL_STR;
		case EXTCSS3_TYPE_DELIM:
			return EXTCSS3_TYPE_DELIM_STR;
		case EXTCSS3_TYPE_NUMBER:
			return EXTCSS3_TYPE_NUMBER_STR;
		case EXTCSS3_TYPE_PERCENTAGE:
			return EXTCSS3_TYPE_PERCENTAGE_STR;
		case EXTCSS3_TYPE_DIMENSION:
			return EXTCSS3_TYPE_DIMENSION_STR;
		case EXTCSS3_TYPE_UNICODE_RANGE:
			return EXTCSS3_TYPE_UNICODE_RANGE_STR;
		case EXTCSS3_TYPE_INCLUDE_MATCH:
			return EXTCSS3_TYPE_INCLUDE_MATCH_STR;
		case EXTCSS3_TYPE_DASH_MATCH:
			return EXTCSS3_TYPE_DASH_MATCH_STR;
		case EXTCSS3_TYPE_PREFIX_MATCH:
			return EXTCSS3_TYPE_PREFIX_MATCH_STR;
		case EXTCSS3_TYPE_SUFFIX_MATCH:
			return EXTCSS3_TYPE_SUFFIX_MATCH_STR;
		case EXTCSS3_TYPE_SUBSTR_MATCH:
			return EXTCSS3_TYPE_SUBSTR_MATCH_STR;
		case EXTCSS3_TYPE_COLUMN:
			return EXTCSS3_TYPE_COLUMN_STR;
		case EXTCSS3_TYPE_WS:
			return EXTCSS3_TYPE_WS_STR;
		case EXTCSS3_TYPE_CDO:
			return EXTCSS3_TYPE_CDO_STR;
		case EXTCSS3_TYPE_CDC:
			return EXTCSS3_TYPE_CDC_STR;
		case EXTCSS3_TYPE_COLON:
			return EXTCSS3_TYPE_COLON_STR;
		case EXTCSS3_TYPE_SEMICOLON:
			return EXTCSS3_TYPE_SEMICOLON_STR;
		case EXTCSS3_TYPE_COMMA:
			return EXTCSS3_TYPE_COMMA_STR;
		case EXTCSS3_TYPE_BR_RO:
			return EXTCSS3_TYPE_BR_RO_STR;
		case EXTCSS3_TYPE_BR_RC:
			return EXTCSS3_TYPE_BR_RC_STR;
		case EXTCSS3_TYPE_BR_SO:
			return EXTCSS3_TYPE_BR_SO_STR;
		case EXTCSS3_TYPE_BR_SC:
			return EXTCSS3_TYPE_BR_SC_STR;
		case EXTCSS3_TYPE_BR_CO:
			return EXTCSS3_TYPE_BR_CO_STR;
		case EXTCSS3_TYPE_BR_CC:
			return EXTCSS3_TYPE_BR_CC_STR;
		case EXTCSS3_TYPE_COMMENT:
			return EXTCSS3_TYPE_COMMENT_STR;
		case EXTCSS3_TYPE_EOF:
			return EXTCSS3_TYPE_EOF_STR;
		default:
			return EXTCSS3_UNDEFINED_STR;
	}
}

char *extcss3_get_flag_str(unsigned short int flag)
{
	switch (flag) {
		case EXTCSS3_FLAG_ID:
			return EXTCSS3_FLAG_ID_STR;
		case EXTCSS3_FLAG_UNRESTRICTED:
			return EXTCSS3_FLAG_UNRESTRICTED_STR;
		case EXTCSS3_FLAG_INTEGER:
			return EXTCSS3_FLAG_INTEGER_STR;
		case EXTCSS3_FLAG_NUMBER:
			return EXTCSS3_FLAG_NUMBER_STR;
		case EXTCSS3_FLAG_STRING:
			return EXTCSS3_FLAG_STRING_STR;
		case EXTCSS3_FLAG_AT_URL_STRING:
			return EXTCSS3_FLAG_AT_URL_STRING_STR;
		default:
			return EXTCSS3_UNDEFINED_STR;
	}
}
