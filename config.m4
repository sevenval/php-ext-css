
PHP_ARG_WITH(extcss3, whether to enable the extcss3 extension,
	[--with-extcss3                   Enable extcss3 support])

if test "$PHP_EXTCSS3" != "no"; then
	PHP_SUBST(EXTCSS3_SHARED_LIBADD)
	PHP_NEW_EXTENSION(extcss3,
		extcss3/intern.c					\
		extcss3/utils.c						\
		extcss3/dumper/dumper.c				\
		extcss3/minifier/minifier.c			\
		extcss3/minifier/tree.c				\
		extcss3/minifier/types/numeric.c	\
		extcss3/minifier/types/hash.c		\
		extcss3/minifier/types/function.c	\
		extcss3/tokenizer/preprocessor.c	\
		extcss3/tokenizer/context.c			\
		extcss3/tokenizer/tokenizer.c		\
		php_extcss3.c,
	$ext_shared, , "-Wall")
fi
