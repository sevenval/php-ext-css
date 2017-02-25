#include "../utils.h"
#include "preprocessor.h"

#include <string.h>

/* ==================================================================================================== */

/**
 * Check the validity of the bytes in the non-ASCII (UTF-8) character
 */
static inline bool _extcss3_check_bytes_corruption(extcss3_intern *intern, unsigned int *error)
{
	unsigned int i, j;

	if (EXTCSS3_IS_NON_ASCII(*intern->state.cursor)) {
		i = 7;
		j = 0;

		while ((i >= 0) && ((*intern->state.cursor >> i) & 1)) {
			if (j > intern->state.rest) {
				*error = EXTCSS3_ERR_BYTES_CORRUPTION;

				return EXTCSS3_FAILURE;
			}

			if (j && ((((intern->state.cursor[j] >> 7) & 1) != 1) || (((intern->state.cursor[j] >> 6) & 1) != 0))) {
				*error = EXTCSS3_ERR_BYTES_CORRUPTION;

				return EXTCSS3_FAILURE;
			}

			j++;
			i--;
		}

		if ((j == 1) || (j > intern->state.rest)) {
			*error = EXTCSS3_ERR_BYTES_CORRUPTION;

			return EXTCSS3_FAILURE;
		}
	}

	return EXTCSS3_SUCCESS;
}

/**
 * Copy a part of the input string to the intern CSS string and move the intern pointers to the new positions
 */
static inline bool _extcss3_copy_and_move(extcss3_intern *intern, const char *src_str, size_t src_len, unsigned int processed)
{
	memcpy(intern->state.writer, src_str, src_len);

	intern->state.rest -= processed;

	intern->state.writer += src_len;
	intern->state.cursor += processed;

	*intern->state.writer = '\0';

	return EXTCSS3_SUCCESS;
}

/**
 * Extended version of https://www.w3.org/TR/css-syntax-3/#input-preprocessing
 */
bool extcss3_preprocess(extcss3_intern *intern, unsigned int *error)
{
	unsigned int len;

	if ((intern == NULL) || (intern->state.cursor == NULL) || (intern->state.writer == NULL)) {
		*error = EXTCSS3_ERR_NULL_PTR;

		return EXTCSS3_FAILURE;
	} else if ((intern->state.rest > 0) && (intern->state.rest <= intern->orig.len)) {
		if (EXTCSS3_SUCCESS != _extcss3_check_bytes_corruption(intern, error)) {
			return EXTCSS3_FAILURE;
		}
	} else if (intern->state.rest == 0) {
		return EXTCSS3_SUCCESS;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if (EXTCSS3_IS_NON_ASCII(*intern->state.cursor) && ((len = extcss3_char_len(*intern->state.cursor)) > 1)) {
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
