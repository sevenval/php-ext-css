#ifndef EXTCSS3_MINIFIER_TYPES_HASH_H
#define EXTCSS3_MINIFIER_TYPES_HASH_H

#include "../../types.h"

/* ==================================================================================================== */

bool extcss3_minify_hash(char *str, size_t len, extcss3_token *token, int *error);
bool extcss3_minify_color(extcss3_token *token, int *error);

#endif /* EXTCSS3_MINIFIER_TYPES_HASH_H */
