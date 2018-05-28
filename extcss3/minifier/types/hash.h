#ifndef EXTCSS3_MINIFIER_TYPES_HASH_H
#define EXTCSS3_MINIFIER_TYPES_HASH_H

#include "../../types.h"

/* ==================================================================================================== */

bool extcss3_minify_hash(extcss3_intern *intern, char *str, unsigned int len, extcss3_token *token, unsigned int *error);
bool extcss3_minify_color(extcss3_intern *intern, extcss3_token *token, unsigned int *error);

#endif /* EXTCSS3_MINIFIER_TYPES_HASH_H */
