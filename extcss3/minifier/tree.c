#include "tree.h"
#include "../intern.h"
#include "../utils.h"

/* ==================================================================================================== */

static void *_extcss3_set_error_code(int *error, int code, extcss3_rule *tree);

static int _extcss3_tree_fork_rule(extcss3_rule **rule);
static int _extcss3_tree_fork_decl(extcss3_decl **decl);

/* ==================================================================================================== */

extcss3_rule *extcss3_create_tree(extcss3_token **token, extcss3_token *max, int level, int *error)
{
	extcss3_token *search;
	extcss3_rule *tree, *rule;
	extcss3_decl *decl, *last;
	unsigned int levels, nested;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if ((tree = rule = extcss3_create_rule()) == NULL) {
		return _extcss3_set_error_code(error, EXTCSS3_ERR_MEMORY, tree);
	}

	rule->level = level;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	while ((*token != NULL) && (*token != max)) {
		// Every new rule initially has no "base_selector"
		if (rule->base_selector == NULL) {
			// Simulated rule for <commment> tokens contain user strings
			if (((*token)->type == EXTCSS3_TYPE_COMMENT) && ((*token)->user.str != NULL)) {
				rule->base_selector = rule->last_selector = *token;

				// Fork the next rule
				if ((*error = _extcss3_tree_fork_rule(&rule)) != 0) {
					return _extcss3_set_error_code(error, *error, tree);
				}

				*token = (*token)->next;

				continue;
			}
			// Consume an invalid {}-block without selectors
			else if ((*token)->type == EXTCSS3_TYPE_BR_CO) {
				// Consume the '{'
				*token = (*token)->next;

				levels = 1;

				while ((*token)->type != EXTCSS3_TYPE_EOF) {
					if ((*token)->type == EXTCSS3_TYPE_BR_CO) {
						levels++;
					} else if ((*token)->type == EXTCSS3_TYPE_BR_CC) {
						levels--;
					}

					if (levels == 0) {
						// Consume the '}'
						*token = (*token)->next;
						break;
					}

					*token = (*token)->next;
				}

				if ((*token == max)) {
					return tree;
				}
			}

			// Simulated rule for the <eof> token
			if ((*token)->type == EXTCSS3_TYPE_EOF) {
				rule->base_selector = rule->last_selector = *token;
				break;
			}
			// Beginn a new rule by a valid selector type
			else if (
				((*token)->type == EXTCSS3_TYPE_AT_KEYWORD)	||
				((*token)->type == EXTCSS3_TYPE_IDENT)		||
				((*token)->type == EXTCSS3_TYPE_HASH)		||
				((*token)->type == EXTCSS3_TYPE_DELIM)		||
				((*token)->type == EXTCSS3_TYPE_BR_SO)		||
				((*token)->type == EXTCSS3_TYPE_COLON)		||
				((*token)->type == EXTCSS3_TYPE_PERCENTAGE)
			) {
				rule->base_selector = rule->last_selector = *token;
			} else if ((*token)->type != EXTCSS3_TYPE_WS && (*token)->type != EXTCSS3_TYPE_COMMENT) {
				// Skip all tokens until the next '{'
				while (
					(*token != NULL) &&
					((*token)->type != EXTCSS3_TYPE_EOF) &&
					((*token)->type != EXTCSS3_TYPE_BR_CO)
				) {
					*token = (*token)->next;
				}

				// The block is consumed above as "invalid" block
				continue;
			}
		} else {
			// Consume a {}-block
			if ((*token)->type == EXTCSS3_TYPE_BR_CO) {
				// Look forward for nested or unbalanced {}-blocks
				levels = 1, nested = 0;
				search = (*token)->next;

				while ((search != NULL) && (search->type != EXTCSS3_TYPE_EOF)) {
					if (search->type == EXTCSS3_TYPE_BR_CO) {
						levels++;
						nested++;
					} else if (search->type == EXTCSS3_TYPE_BR_CC) {
						levels--;
					}

					if (levels == 0) {
						break;
					}

					search = search->next;
				}

				/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

				// Unbalanced {}-block detected
				if (levels != 0) {
					// Consume all tokens inside the unbalanced area
					while (*token != search) {
						*token = (*token)->next;
					}
				} else {
					if ((rule->block = extcss3_create_block()) == NULL) {
						return _extcss3_set_error_code(error, EXTCSS3_ERR_MEMORY, tree);
					}

					// Set the '{'
					rule->block->base = *token;
					// Set the '}'
					rule->block->last = search;

					// Consume the '{'
					*token = (*token)->next;

					/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

					// Consume all nested rules (recursive)
					if (nested > 0) {
						if ((rule->block->rules = extcss3_create_tree(token, search, level + 1, error)) == NULL) {
							return _extcss3_set_error_code(error, *error, tree);
						}
					}
					// Consume all declarations in the current block
					else {
						last = decl = NULL;

						while ((*token != NULL) && (*token != search)) {
							if (decl == NULL) {
								// Skip leading whitespaces and comments (if no contain user strings)
								if (_EXTCSS3_TYPE_EMPTY_EX(*token)) {
									*token = (*token)->next;
									continue;
								}

								if (last == NULL) {
									if ((rule->block->decls = decl = last = extcss3_create_decl()) == NULL) {
										return _extcss3_set_error_code(error, EXTCSS3_ERR_MEMORY, tree);
									}
								} else {
									if ((*error = _extcss3_tree_fork_decl(&last)) != 0) {
										return _extcss3_set_error_code(error, *error, tree);
									}

									decl = last;
								}

								decl->base = decl->last = *token;
							}

							// Update the last declaration token
							decl->last = *token;

							if ((*token)->type == EXTCSS3_TYPE_SEMICOLON) {
								decl = NULL;
							}

							*token = (*token)->next;
						}
					}

					// Fork the next rule
					if ((*error = _extcss3_tree_fork_rule(&rule)) != 0) {
						return _extcss3_set_error_code(error, *error, tree);
					}
				}

				// Simulated rule for the <eof> token
				if ((*token)->type == EXTCSS3_TYPE_EOF) {
					rule->base_selector = rule->last_selector = *token;
					break;
				}
			} else {
				// Simulated rule for the <eof> token
				if ((*token)->type == EXTCSS3_TYPE_EOF) {
					// Fork the next rule
					if ((*error = _extcss3_tree_fork_rule(&rule)) != 0) {
						return _extcss3_set_error_code(error, *error, tree);
					}

					rule->base_selector = rule->last_selector = *token;
					break;
				}

				// Update the last selector token
				rule->last_selector = *token;

				// Close the current <at> rule and fork the next rule
				if (
					rule->base_selector->type == EXTCSS3_TYPE_AT_KEYWORD &&
					(*token)->type == EXTCSS3_TYPE_SEMICOLON
				) {
					if ((*error = _extcss3_tree_fork_rule(&rule)) != 0) {
						return _extcss3_set_error_code(error, *error, tree);
					}
				}
			}
		}

		*token = (*token)->next;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	return tree;
}

/* ==================================================================================================== */

static inline void *_extcss3_set_error_code(int *error, int code, extcss3_rule *tree)
{
	*error = code;

	if (tree != NULL) {
		extcss3_release_rules_list(tree);
	}

	return NULL;
}

static inline int _extcss3_tree_fork_rule(extcss3_rule **rule)
{
	extcss3_rule *fork;

	if ((fork = extcss3_create_rule()) == NULL) {
		return EXTCSS3_ERR_MEMORY;
	}

	fork->level		= (*rule)->level;
	fork->prev		= *rule;
	(*rule)->next	= fork;
	*rule			= fork;

	return 0;
}

static inline int _extcss3_tree_fork_decl(extcss3_decl **decl)
{
	extcss3_decl *fork;

	if ((fork = extcss3_create_decl()) == NULL) {
		return EXTCSS3_ERR_MEMORY;
	}

	fork->prev		= *decl;
	(*decl)->next	= fork;
	*decl			= fork;

	return 0;
}
