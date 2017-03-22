#ifndef EXTCSS3_TYPES_H
#define EXTCSS3_TYPES_H

#include <stdlib.h>
#include <stdbool.h>

/* ==================================================================================================== */

#define EXTCSS3_SUCCESS					(true)
#define EXTCSS3_FAILURE					(false)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EXTCSS3_ERR_MEMORY				((unsigned int)1)
#define EXTCSS3_ERR_BYTES_CORRUPTION	((unsigned int)2)
#define EXTCSS3_ERR_NULL_PTR			((unsigned int)3)
#define EXTCSS3_ERR_INV_PARAM			((unsigned int)4)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EXTCSS3_REPLACEMENT_CHR			("\xEF\xBF\xBD")
#define EXTCSS3_REPLACEMENT_LEN			((unsigned int)3)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EXTCSS3_FLAG_ID					((unsigned int)1)
#define EXTCSS3_FLAG_UNRESTRICTED		((unsigned int)2)
#define EXTCSS3_FLAG_INTEGER			((unsigned int)3)
#define EXTCSS3_FLAG_NUMBER				((unsigned int)4)
#define EXTCSS3_FLAG_STRING				((unsigned int)5)
#define EXTCSS3_FLAG_AT_URL_STRING		((unsigned int)6)

#define EXTCSS3_FLAG_ID_STR				("id")
#define EXTCSS3_FLAG_UNRESTRICTED_STR	("unrestricted")
#define EXTCSS3_FLAG_INTEGER_STR		("int")
#define EXTCSS3_FLAG_NUMBER_STR			("num")
#define EXTCSS3_FLAG_STRING_STR			("string")
#define EXTCSS3_FLAG_AT_URL_STRING_STR	("at_url_string")

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EXTCSS3_UNDEFINED				((unsigned int)0)
#define EXTCSS3_UNDEFINED_STR			("undefined")

#define EXTCSS3_TYPE_IDENT				((unsigned int)1)
#define EXTCSS3_TYPE_FUNCTION			((unsigned int)2)
#define EXTCSS3_TYPE_AT_KEYWORD			((unsigned int)3)
#define EXTCSS3_TYPE_HASH				((unsigned int)4)
#define EXTCSS3_TYPE_STRING				((unsigned int)5)
#define EXTCSS3_TYPE_BAD_STRING			((unsigned int)6)
#define EXTCSS3_TYPE_URL				((unsigned int)7)
#define EXTCSS3_TYPE_BAD_URL			((unsigned int)8)
#define EXTCSS3_TYPE_DELIM				((unsigned int)9)
#define EXTCSS3_TYPE_NUMBER				((unsigned int)10)
#define EXTCSS3_TYPE_PERCENTAGE			((unsigned int)11)
#define EXTCSS3_TYPE_DIMENSION			((unsigned int)12)
#define EXTCSS3_TYPE_UNICODE_RANGE		((unsigned int)13)
#define EXTCSS3_TYPE_INCLUDE_MATCH		((unsigned int)14)
#define EXTCSS3_TYPE_DASH_MATCH			((unsigned int)15)
#define EXTCSS3_TYPE_PREFIX_MATCH		((unsigned int)16)
#define EXTCSS3_TYPE_SUFFIX_MATCH		((unsigned int)17)
#define EXTCSS3_TYPE_SUBSTR_MATCH		((unsigned int)18)
#define EXTCSS3_TYPE_COLUMN				((unsigned int)19)
#define EXTCSS3_TYPE_WS					((unsigned int)20)
#define EXTCSS3_TYPE_CDO				((unsigned int)21)
#define EXTCSS3_TYPE_CDC				((unsigned int)22)
#define EXTCSS3_TYPE_COLON				((unsigned int)23)
#define EXTCSS3_TYPE_SEMICOLON			((unsigned int)24)
#define EXTCSS3_TYPE_COMMA				((unsigned int)25)
#define EXTCSS3_TYPE_BR_RO				((unsigned int)26)
#define EXTCSS3_TYPE_BR_RC				((unsigned int)27)
#define EXTCSS3_TYPE_BR_SO				((unsigned int)28)
#define EXTCSS3_TYPE_BR_SC				((unsigned int)29)
#define EXTCSS3_TYPE_BR_CO				((unsigned int)30)
#define EXTCSS3_TYPE_BR_CC				((unsigned int)31)
#define EXTCSS3_TYPE_COMMENT			((unsigned int)32)
#define EXTCSS3_TYPE_EOF				((unsigned int)33)

#define EXTCSS3_TYPE_IDENT_STR			("ident")
#define EXTCSS3_TYPE_FUNCTION_STR		("function")
#define EXTCSS3_TYPE_AT_KEYWORD_STR		("at_keyword")
#define EXTCSS3_TYPE_HASH_STR			("hash")
#define EXTCSS3_TYPE_STRING_STR			("string")
#define EXTCSS3_TYPE_BAD_STRING_STR		("bad_string")
#define EXTCSS3_TYPE_URL_STR			("url")
#define EXTCSS3_TYPE_BAD_URL_STR		("bad_url")
#define EXTCSS3_TYPE_DELIM_STR			("delim")
#define EXTCSS3_TYPE_NUMBER_STR			("number")
#define EXTCSS3_TYPE_PERCENTAGE_STR		("percentage")
#define EXTCSS3_TYPE_DIMENSION_STR		("dimension")
#define EXTCSS3_TYPE_UNICODE_RANGE_STR	("unicode_range")
#define EXTCSS3_TYPE_INCLUDE_MATCH_STR	("include_match")
#define EXTCSS3_TYPE_DASH_MATCH_STR		("dash")
#define EXTCSS3_TYPE_PREFIX_MATCH_STR	("prefix_match")
#define EXTCSS3_TYPE_SUFFIX_MATCH_STR	("suffix_match")
#define EXTCSS3_TYPE_SUBSTR_MATCH_STR	("substring_match")
#define EXTCSS3_TYPE_COLUMN_STR			("column")
#define EXTCSS3_TYPE_WS_STR				("whitespace")
#define EXTCSS3_TYPE_CDO_STR			("cdo")
#define EXTCSS3_TYPE_CDC_STR			("cdc")
#define EXTCSS3_TYPE_COLON_STR			("colon")
#define EXTCSS3_TYPE_SEMICOLON_STR		("semicolon")
#define EXTCSS3_TYPE_COMMA_STR			("comma")
#define EXTCSS3_TYPE_BR_RO_STR			("br_ro")
#define EXTCSS3_TYPE_BR_RC_STR			("br_rc")
#define EXTCSS3_TYPE_BR_SO_STR			("br_so")
#define EXTCSS3_TYPE_BR_SC_STR			("br_sc")
#define EXTCSS3_TYPE_BR_CO_STR			("br_co")
#define EXTCSS3_TYPE_BR_CC_STR			("br_cc")
#define EXTCSS3_TYPE_COMMENT_STR		("comment")
#define EXTCSS3_TYPE_EOF_STR			("eof")

/* ==================================================================================================== */

#pragma pack(push)
#pragma pack()

typedef struct _extcss3_str		extcss3_str;

typedef struct _extcss3_state	extcss3_state;

typedef struct _extcss3_token	extcss3_token;

typedef struct _extcss3_ctxt	extcss3_ctxt;

typedef struct _extcss3_vendor	extcss3_vendor;

typedef struct _extcss3_sig		extcss3_sig;

typedef struct _extcss3_not		extcss3_not;

typedef struct _extcss3_mod		extcss3_mod;

typedef struct _extcss3_decl	extcss3_decl;

typedef struct _extcss3_block	extcss3_block;

typedef struct _extcss3_rule	extcss3_rule;

typedef struct _extcss3_intern	extcss3_intern;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

struct _extcss3_str
{
	char				*str;
	size_t				len;
};

struct _extcss3_state
{
	char				*reader;
	char				*cursor;
	char				*writer;

	size_t				rest;
};

struct _extcss3_token
{
	unsigned int		type;

	extcss3_str			data;
	extcss3_str			info;
	extcss3_str			user;

	extcss3_token		*prev;
	extcss3_token		*next;

	unsigned int		flag;
};

struct _extcss3_ctxt
{
	size_t				level;
	extcss3_token		*token;

	extcss3_ctxt		*prev;
	extcss3_ctxt		*next;
};

struct _extcss3_vendor
{
	extcss3_str			name;
	extcss3_vendor		*next;
};

struct _extcss3_sig
{
	unsigned int		type;
	extcss3_sig			*next;

	void				*callable;
};

struct _extcss3_not
{
	extcss3_sig			*base;
	extcss3_sig			*last;

	void				(*callback)(extcss3_intern *intern, extcss3_sig *signal);
	void				(*destructor)(void *callable);
};

struct _extcss3_mod
{
	void				*string;
	void				*bad_string;
	void				*url;
	void				*bad_url;
	void				*comment;

	void				(*callback)(extcss3_intern *intern);
	void				(*destructor)(void *modifier);

	long long			user_strlen_diff;
};

struct _extcss3_decl
{
	extcss3_token		*base;
	extcss3_token		*last;

	extcss3_decl		*prev;
	extcss3_decl		*next;
};

struct _extcss3_block
{
	extcss3_token		*base;
	extcss3_token		*last;

	extcss3_rule		*rules;
	extcss3_decl		*decls;
};

struct _extcss3_rule
{
	unsigned int		level;

	extcss3_token		*base_selector;
	extcss3_token		*last_selector;

	extcss3_block		*block;

	extcss3_rule		*prev;
	extcss3_rule		*next;
};

struct _extcss3_intern
{
	extcss3_state		state;

	extcss3_str			orig;
	extcss3_str			copy;

	extcss3_token		*base_token;
	extcss3_token		*last_token;

	extcss3_ctxt		*base_ctxt;
	extcss3_ctxt		*last_ctxt;

	extcss3_vendor		*base_vendor;
	extcss3_vendor		*last_vendor;

	extcss3_not			notifier;
	extcss3_mod			modifier;
};

#pragma pack(pop)

/* ==================================================================================================== */

#endif /* EXTCSS3_TYPES_H */
