#ifndef EXTCSS3_DUMPER_H
#define EXTCSS3_DUMPER_H

#include "../types.h"

/* ==================================================================================================== */

char *extcss3_dump_tokens(extcss3_intern *intern, int *error);
char *extcss3_dump_rules(extcss3_intern *intern, extcss3_rule *rule, int *error);

#endif /* EXTCSS3_DUMPER_H */
