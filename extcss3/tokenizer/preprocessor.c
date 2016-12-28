#include "../utils.h"
#include "preprocessor.h"

#include <string.h>

/* ==================================================================================================== */

/**
 * Check the validity of the bytes in the non-ASCII (UTF-8) character
 */
static inline int _extcss3_check_bytes_corruption(extcss3_intern *intern)
{
	unsigned short int i, j;

	if (((*intern->state.cursor >> 7) & 1) != 0) {
		i = 7;
		j = 0;

		while ((i >= 0) && ((*intern->state.cursor >> i) & 1)) {
			if (j > intern->state.rest) {
				return EXTCSS3_ERR_BYTES_CORRUPTION;
			}

			if (j && ((((intern->state.cursor[j] >> 7) & 1) != 1) || (((intern->state.cursor[j] >> 6) & 1) != 0))) {
				return EXTCSS3_ERR_BYTES_CORRUPTION;
			}

			j++;
			i--;
		}

		if ((j == 1) || (j > intern->state.rest)) {
			return EXTCSS3_ERR_BYTES_CORRUPTION;
		}
	}

	return 0;
}

/**
 * Copy a part of the input string to the intern CSS string and move the intern pointers to the new positions
 */
static inline int _extcss3_copy_and_move(extcss3_intern *intern, const char *src_str, size_t src_len, int processed)
{
	memcpy(intern->state.writer, src_str, src_len);

	intern->state.rest -= processed;

	intern->state.writer += src_len;
	intern->state.cursor += processed;

	*intern->state.writer = '\0';

	return 0;
}

/**
 * Extended version of https://www.w3.org/TR/css-syntax-3/#input-preprocessing
 */
int extcss3_preprocess(extcss3_intern *intern)
{
	int ret, len;

	if ((intern == NULL) || (intern->state.cursor == NULL)) {
		return EXTCSS3_ERR_NULL_PTR;
	} else if ((intern->state.rest > 0) && (intern->state.rest <= intern->orig.len)) {
		if ((ret = _extcss3_check_bytes_corruption(intern)) != 0) {
			return ret;
		}
	} else if (intern->state.rest == 0) {
		return 0;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if ((len = extcss3_char_len(*intern->state.cursor)) != 1) {
		/* Skip the following checks for the multibyte characters */
		return _extcss3_copy_and_move(intern, intern->state.cursor, len, len);
	} else if ((intern->state.rest > 0) && (*intern->state.cursor == '\0')) {
		/* Add a U+FFFD REPLACEMENT CHARACTER (3 bytes) instead of the inside '\0' (1 byte) */
		return _extcss3_copy_and_move(intern, EXTCSS3_REPLACEMENT_CHR, EXTCSS3_REPLACEMENT_LEN, 1);
	} else if (*intern->state.cursor == '\r') { 
		if (intern->state.cursor[1] == '\n') {
			/* Add '\n' (1 byte) instead of "\r\n" (2 bytes) */
			return _extcss3_copy_and_move(intern, "\n", 1, 2);
		} else {
			/* Add '\n' (1 byte) instead of '\r' (1 byte) */
			return _extcss3_copy_and_move(intern, "\n", 1, 1);
		}
	} else if (*intern->state.cursor == '\f') {
		/* Add '\n' (1 byte) instead of '\f' (1 byte) */
		return _extcss3_copy_and_move(intern, "\n", 1, 1);
	} else {
		return _extcss3_copy_and_move(intern, intern->state.cursor, 1, 1);
	}
}
