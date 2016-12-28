#ifndef PHP_EXTCSS3_H
#define PHP_EXTCSS3_H

#include <php.h>

#include "extcss3/types.h"

#define PHP_EXTCSS3_EXTNAME	"extcss3"
#define PHP_EXTCSS3_EXTVER	"1.0.0"

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif /* HAVE_CONFIG_H */

extern zend_module_entry extcss3_module_entry;
#define phpext_extcss3_ptr &extcss3_module_entry;

typedef struct _extcss3_object
{
	extcss3_intern	*intern;
	zend_object		std;
} extcss3_object;

#endif /* PHP_EXTCSS3_H */
