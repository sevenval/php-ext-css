#ifndef EXTCSS3_TYPES_H
#define EXTCSS3_TYPES_H

#include <stdlib.h>
#include <stdbool.h>

/* ==================================================================================================== */

#define EXTCSS3_SUCCESS					(true)
#define EXTCSS3_FAILURE					(false)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EXTCSS3_ERR_MEMORY				((unsigned short int)1)
#define EXTCSS3_ERR_BYTES_CORRUPTION	((unsigned short int)2)
#define EXTCSS3_ERR_NULL_PTR			((unsigned short int)3)
#define EXTCSS3_ERR_INV_PARAM			((unsigned short int)4)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EXTCSS3_REPLACEMENT_CHR			("\xEF\xBF\xBD")
#define EXTCSS3_REPLACEMENT_LEN			((unsigned short int)3)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EXTCSS3_FLAG_ID					((unsigned short int)1)
#define EXTCSS3_FLAG_UNRESTRICTED		((unsigned short int)2)
#define EXTCSS3_FLAG_INTEGER			((unsigned short int)3)
#define EXTCSS3_FLAG_NUMBER				((unsigned short int)4)
#define EXTCSS3_FLAG_STRING				((unsigned short int)5)
#define EXTCSS3_FLAG_AT_URL_STRING		((unsigned short int)6)

#define EXTCSS3_FLAG_ID_STR				("id")
#define EXTCSS3_FLAG_UNRESTRICTED_STR	("unrestricted")
#define EXTCSS3_FLAG_INTEGER_STR		("int")
#define EXTCSS3_FLAG_NUMBER_STR			("num")
#define EXTCSS3_FLAG_STRING_STR			("string")
#define EXTCSS3_FLAG_AT_URL_STRING_STR	("at_url_string")

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EXTCSS3_UNDEFINED				((unsigned short int)0)
#define EXTCSS3_UNDEFINED_STR			("undefined")

#define EXTCSS3_TYPE_IDENT				((unsigned short int)1)
#define EXTCSS3_TYPE_FUNCTION			((unsigned short int)2)
#define EXTCSS3_TYPE_AT_KEYWORD			((unsigned short int)3)
#define EXTCSS3_TYPE_HASH				((unsigned short int)4)
#define EXTCSS3_TYPE_STRING				((unsigned short int)5)
#define EXTCSS3_TYPE_BAD_STRING			((unsigned short int)6)
#define EXTCSS3_TYPE_URL				((unsigned short int)7)
#define EXTCSS3_TYPE_BAD_URL			((unsigned short int)8)
#define EXTCSS3_TYPE_DELIM				((unsigned short int)9)
#define EXTCSS3_TYPE_NUMBER				((unsigned short int)10)
#define EXTCSS3_TYPE_PERCENTAGE			((unsigned short int)11)
#define EXTCSS3_TYPE_DIMENSION			((unsigned short int)12)
#define EXTCSS3_TYPE_UNICODE_RANGE		((unsigned short int)13)
#define EXTCSS3_TYPE_INCLUDE_MATCH		((unsigned short int)14)
#define EXTCSS3_TYPE_DASH_MATCH			((unsigned short int)15)
#define EXTCSS3_TYPE_PREFIX_MATCH		((unsigned short int)16)
#define EXTCSS3_TYPE_SUFFIX_MATCH		((unsigned short int)17)
#define EXTCSS3_TYPE_SUBSTR_MATCH		((unsigned short int)18)
#define EXTCSS3_TYPE_COLUMN				((unsigned short int)19)
#define EXTCSS3_TYPE_WS					((unsigned short int)20)
#define EXTCSS3_TYPE_CDO				((unsigned short int)21)
#define EXTCSS3_TYPE_CDC				((unsigned short int)22)
#define EXTCSS3_TYPE_COLON				((unsigned short int)23)
#define EXTCSS3_TYPE_SEMICOLON			((unsigned short int)24)
#define EXTCSS3_TYPE_COMMA				((unsigned short int)25)
#define EXTCSS3_TYPE_BR_RO				((unsigned short int)26)
#define EXTCSS3_TYPE_BR_RC				((unsigned short int)27)
#define EXTCSS3_TYPE_BR_SO				((unsigned short int)28)
#define EXTCSS3_TYPE_BR_SC				((unsigned short int)29)
#define EXTCSS3_TYPE_BR_CO				((unsigned short int)30)
#define EXTCSS3_TYPE_BR_CC				((unsigned short int)31)
#define EXTCSS3_TYPE_COMMENT			((unsigned short int)32)
#define EXTCSS3_TYPE_EOF				((unsigned short int)33)

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
	size_t				rest;

	char				*cursor;
	char				*reader;
	char				*writer;
};

struct _extcss3_token
{
	unsigned short int	type;
	unsigned short int	flag;

	extcss3_str			data;
	extcss3_str			info;
	extcss3_str			user;

	extcss3_token		*prev;
	extcss3_token		*next;
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

struct _extcss3_mod
{
	void				*string;
	void				*bad_string;
	void				*url;
	void				*bad_url;
	void				*comment;

	void				(*callback)(extcss3_intern *intern);
	void				(*destructor)(void *modifier);
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
	extcss3_str			orig;
	extcss3_str			copy;

	extcss3_state		state;

	extcss3_vendor		*base_vendor;
	extcss3_vendor		*last_vendor;

	extcss3_token		*base_token;
	extcss3_token		*last_token;

	extcss3_ctxt		*base_ctxt;
	extcss3_ctxt		*last_ctxt;

	extcss3_mod			modifier;
};

#pragma pack(pop)

/* ==================================================================================================== */

#endif /* EXTCSS3_TYPES_H */
