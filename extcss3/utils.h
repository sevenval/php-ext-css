#ifndef EXTCSS3_UTILS_H
#define EXTCSS3_UTILS_H

#include "types.h"

/* ==================================================================================================== */

/* https://www.w3.org/TR/css-syntax-3/#whitespace */
#define EXTCSS3_IS_WS(c)			(((c) == ' ') || ((c) == '\n') || ((c) == '\t'))

/* https://www.w3.org/TR/css-syntax-3/#digit */
#define EXTCSS3_IS_DIGIT(c)			(((c) >= '0') && ((c) <= '9'))

/* https://www.w3.org/TR/css-syntax-3/#letter */
#define EXTCSS3_IS_LETTER(c)		((((c) >= 'a') && ((c) <= 'z')) || (((c) >= 'A') && ((c) <= 'Z')))

/* https://www.w3.org/TR/css-syntax-3/#non-printable-code-point */
#define EXTCSS3_NON_PRINTABLE(c)	((((c) >= 0x00) && ((c) <= 0x08)) || ((c) == 0x0B) || (((c) >= 0x0E) && ((c) <= 0x1F)) || ((c) == 0x7F))

/* https://www.w3.org/TR/css-syntax-3/#maximum-allowed-code-point (U+10FFFF) */
#define EXTCSS3_MAX_ALLOWED_CP		((int)1114111)

/* https://www.w3.org/TR/css-syntax-3/#surrogate-code-point (U+D800 - U+DFFF) */
#define EXTCSS3_FOR_SURROGATE_CP(i)	(((i) >= 55296) && ((i) <= 57343))

#define EXTCSS3_IS_HEX(c)			(EXTCSS3_IS_DIGIT(c) || (((c) >= 'a') && ((c) <= 'f')) || (((c) >= 'A') && ((c) <= 'F')))

#define EXTCSS3_IS_NON_ASCII(c)		((((c) >> 7) & 1) != 0)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EXTCSS3_HAS_MODIFIER(intern) (			\
	((intern->modifier.string)		!= NULL) ||	\
	((intern->modifier.bad_string)	!= NULL) ||	\
	((intern->modifier.bad_url)		!= NULL) ||	\
	((intern->modifier.url)			!= NULL) ||	\
	((intern->modifier.comment)		!= NULL)	\
)

#define EXTCSS3_TYPE_IS_MODIFIABLE(i) (	   \
	((i) == EXTCSS3_TYPE_STRING)		|| \
	((i) == EXTCSS3_TYPE_BAD_STRING)	|| \
	((i) == EXTCSS3_TYPE_URL)			|| \
	((i) == EXTCSS3_TYPE_BAD_URL)		|| \
	((i) == EXTCSS3_TYPE_COMMENT)		   \
)

#define EXTCSS3_CHARS_EQ(a, b)	( ((a) == (b)) || \
	( ((a) >= 'a') && ((a) <= 'z') && ((b) >= 'A') && ((b) <= 'Z') && (((b) + 32) == (a)) ) || \
	( ((a) >= 'A') && ((a) <= 'Z') && ((b) >= 'a') && ((b) <= 'z') && (((a) + 32) == (b)) ) \
)

#define _EXTCSS3_TYPE_EMPTY(type)		(((type) == EXTCSS3_TYPE_WS) || ((type) == EXTCSS3_TYPE_COMMENT))

#define _EXTCSS3_TYPE_EMPTY_EX(token)	(			\
	((token)->type == EXTCSS3_TYPE_WS) ||			\
	(												\
		((token)->type == EXTCSS3_TYPE_COMMENT) &&	\
		((token)->user.str == NULL)					\
	)												\
)

/* ==================================================================================================== */

unsigned int extcss3_char_len(char c);

bool extcss3_ascii_strncasecmp(const char *str1, const char *str2, unsigned char n);

char *extcss3_get_type_str(unsigned int type);
char *extcss3_get_flag_str(unsigned int flag);

/* ==================================================================================================== */

#endif /* EXTCSS3_UTILS_H */
