#ifndef EXTCSS3_MINIFIER_TREE_H
#define EXTCSS3_MINIFIER_TREE_H

#include "../types.h"

/* ==================================================================================================== */

extcss3_rule *extcss3_create_tree(extcss3_token **token, extcss3_token *max, int level, int *error);

#endif /* EXTCSS3_MINIFIER_TREE_H */
