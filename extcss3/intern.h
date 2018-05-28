#ifndef EXTCSS3_INTERN_H
#define EXTCSS3_INTERN_H

#include "types.h"

/* ==================================================================================================== */

extcss3_intern *extcss3_create_intern(void);
extcss3_vendor *extcss3_create_vendor(mpz_pool_t *pool);
extcss3_token *extcss3_create_token(mpz_pool_t *pool);
extcss3_ctxt *extcss3_create_ctxt(mpz_pool_t *pool);
extcss3_rule *extcss3_create_rule(mpz_pool_t *pool);
extcss3_block *extcss3_create_block(mpz_pool_t *pool);
extcss3_decl *extcss3_create_decl(mpz_pool_t *pool);

void extcss3_release_intern(extcss3_intern *intern);
void extcss3_release_vendor(mpz_pool_t *pool, extcss3_vendor *vendor);
void extcss3_release_vendors_list(mpz_pool_t *pool, extcss3_vendor *list);
void extcss3_release_token(mpz_pool_t *pool, extcss3_token *token);
void extcss3_release_tokens_list(mpz_pool_t *pool, extcss3_token *list);
void extcss3_release_ctxt(mpz_pool_t *pool, extcss3_ctxt *ctxt);
void extcss3_release_ctxts_list(mpz_pool_t *pool, extcss3_ctxt *list);
void extcss3_release_rule(mpz_pool_t *pool, extcss3_rule *rule);
void extcss3_release_rules_list(mpz_pool_t *pool, extcss3_rule *list);
void extcss3_release_block(mpz_pool_t *pool, extcss3_block *block);
void extcss3_release_decl(mpz_pool_t *pool, extcss3_decl *decl);
void extcss3_release_decls_list(mpz_pool_t *pool, extcss3_decl *list);
void extcss3_release_signal(mpz_pool_t *pool, extcss3_not *notifier, extcss3_sig *sig);
void extcss3_release_signals_list(mpz_pool_t *pool, extcss3_not *notifier);

bool extcss3_set_css_string(extcss3_intern *intern, char *css, size_t len, unsigned int *error);
bool extcss3_set_notifier(extcss3_intern *intern, unsigned int type, void *callable, unsigned int *error);
bool extcss3_set_modifier(extcss3_intern *intern, unsigned int type, void *callable, unsigned int *error);
bool extcss3_set_vendor_string(extcss3_intern *intern, char *name, size_t len, unsigned int *error);

/* ==================================================================================================== */

#endif /* EXTCSS3_INTERN_H */
