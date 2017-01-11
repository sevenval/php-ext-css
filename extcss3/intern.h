#ifndef EXTCSS3_INTERN_H
#define EXTCSS3_INTERN_H

#include "types.h"

/* ==================================================================================================== */

extcss3_intern *extcss3_create_intern(void);
extcss3_vendor *extcss3_create_vendor(void);
extcss3_token *extcss3_create_token(void);
extcss3_ctxt *extcss3_create_ctxt(void);
extcss3_rule *extcss3_create_rule(void);
extcss3_block *extcss3_create_block(void);
extcss3_decl *extcss3_create_decl(void);

void extcss3_release_intern(extcss3_intern *intern);
void extcss3_release_vendor(extcss3_vendor *vendor, bool recursive);
void extcss3_release_token(extcss3_token *token, bool recursive);
void extcss3_release_ctxt(extcss3_ctxt *ctxt, bool recursive);
void extcss3_release_rule(extcss3_rule *rule, bool recursive);
void extcss3_release_block(extcss3_block *block);
void extcss3_release_decl(extcss3_decl *decl, bool recursive);

bool extcss3_set_css_string(extcss3_intern *intern, char *css, size_t len, int *error);
bool extcss3_set_modifier(extcss3_intern *intern, short int type, void *callable, int *error);
bool extcss3_set_vendor_string(extcss3_intern *intern, char *name, size_t len, int *error);

/* ==================================================================================================== */

#endif /* EXTCSS3_INTERN_H */
