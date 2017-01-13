#include "hash.h"
#include "../../utils.h"

#include <string.h>
#include <strings.h>

/* ==================================================================================================== */

const char *extcss3_hash_colors[29][2] = {
    {"f0ffff", "azure"},
	{"f5f5dc", "beige"},
	{"ffe4c4", "bisque"},
	{"a52a2a", "brown"},
	{"ff7f50", "coral"},
	{"ffd700", "gold"},
	{"008000", "green"},
	{"4b0082", "indigo"},
	{"fffff0", "ivory"},
	{"f0e68c", "khaki"},
	{"faf0e6", "linen"},
	{"800000", "maroon"},
	{"000080", "navy"},
	{"808000", "olive"},
	{"ffa500", "orange"},
	{"da70d6", "orchid"},
	{"cd853f", "peru"},
	{"ffc0cb", "pink"},
	{"dda0dd", "plum"},
	{"800080", "purple"},
	{"fa8072", "salmon"},
	{"a0522d", "sienna"},
	{"c0c0c0", "silver"},
	{"fffafa", "snow"},
	{"d2b48c", "tan"},
	{"008080", "teal"},
	{"ff6347", "tomato"},
	{"ee82ee", "violet"},
	{"f5deb3", "wheat"}
};

const char *extcss3_color_hashes_08[11][2] = {
	{"cornsilk", "fff8dc"},
	{"darkblue", "00008b"},
	{"darkcyan", "008b8b"},
	{"darkgray", "a9a9a9"},
	{"darkgrey", "a9a9a9"},
	{"deeppink", "ff1493"},
	{"honeydew", "f0fff0"},
	{"lavender", "e6e6fa"},
	{"moccasin", "ffe4b5"},
	{"seagreen", "2e8b57"},
	{"seashell", "fff5ee"}
};

const char *extcss3_color_hashes_09[29][2] = {
	{"aliceblue", "f0f8ff"},
	{"burlywood", "deb887"},
	{"cadetblue", "5f9ea0"},
	{"chocolate", "d2691e"},
	{"darkkhaki", "bdb76b"},
	{"firebrick", "b22222"},
	{"gainsboro", "dcdcdc"},
	{"goldenrod", "daa520"},
	{"indianred", "cd5c5c"},
	{"lawngreen", "7cfc00"},
	{"lightblue", "add8e6"},
	{"lightcyan", "e0ffff"},
	{"lightgray", "d3d3d3"},
	{"lightgrey", "d3d3d3"},
	{"lightpink", "ffb6c1"},
	{"limegreen", "32cd32"},
	{"mintcream", "f5fffa"},
	{"mistyrose", "ffe4e1"},
	{"olivedrab", "6b8e23"},
	{"orangered", "ff4500"},
	{"palegreen", "98fb98"},
	{"peachpuff", "ffdab9"},
    {"rosybrown", "bc8f8f"},
    {"royalblue", "4169e1"},
	{"slateblue", "6a5acd"},
	{"slategray", "708090"},
	{"slategrey", "708090"},
	{"steelblue", "4682b4"},
	{"turquoise", "40e0d0"}
};

const char *extcss3_color_hashes_10[16][2] = {
	{"aquamarine", "7fffd4"},
	{"blueviolet", "8a2be2"},
	{"chartreuse", "7fff00"},
	{"darkorange", "ff8c00"},
	{"darkorchid", "9932cc"},
	{"darksalmon", "e9967a"},
	{"darkviolet", "9400d3"},
	{"dodgerblue", "1e90ff"},
	{"ghostwhite", "f8f8ff"},
	{"lightcoral", "f08080"},
	{"lightgreen", "90ee90"},
	{"mediumblue", "0000cd"},
	{"papayawhip", "ffefd5"},
	{"powderblue", "b0e0e6"},
	{"sandybrown", "f4a460"},
	{"whitesmoke", "f5f5f5"}
};

const char *extcss3_color_hashes_11[11][2] = {
	{"darkmagenta", "8b008b"},
	{"deepskyblue", "00bfff"},
	{"floralwhite", "fffaf0"},
	{"forestgreen", "228b22"},
	{"greenyellow", "adff2f"},
	{"lightsalmon", "ffa07a"},
	{"lightyellow", "ffffe0"},
	{"navajowhite", "ffdead"},
	{"saddlebrown", "8b4513"},
	{"springgreen", "00ff7f"},
	{"yellowgreen", "9acd32"}
};

const char *extcss3_color_hashes_12[7][2] = {
	{"antiquewhite", "faebd7"},
	{"darkseagreen", "8fbc8f"},
	{"lemonchiffon", "fffacd"},
    {"lightskyblue", "87cefa"},
	{"mediumorchid", "ba55d3"},
	{"mediumpurple", "9370db"},
	{"midnightblue", "191970"}
};

const char *extcss3_color_hashes_13[11][2] = {
	{"darkgoldenrod", "b8860b"},
	{"darkslateblue", "483d8b"},
	{"darkslategray", "2f4f4f"},
    {"darkslategrey", "2f4f4f"},
	{"darkturquoise", "00ced1"},
	{"lavenderblush", "fff0f5"},
    {"lightseagreen", "20b2aa"},
	{"palegoldenrod", "eee8aa"},
	{"paleturquoise", "afeeee"},
	{"palevioletred", "db7093"},
	{"rebeccapurple", "639"}
};

const char *extcss3_color_hashes_14[7][2] = {
	{"blanchedalmond", "ffebcd"},
	{"cornflowerblue", "6495ed"},
	{"darkolivegreen", "556b2f"},
	{"lightslategray", "778899"},
	{"lightslategrey", "778899"},
	{"lightsteelblue", "b0c4de"},
	{"mediumseagreen", "3cb371"}
};

const char *extcss3_color_hashes_xx[11][2] = {
    {"black", "000"},
	{"white", "fff"},
	{"fuchsia", "f0f"},
	{"yellow", "ff0"},
	{"hotpink", "ff69b4"},
	{"lightgoldenrodyellow", "fafad2"},
	{"mediumaquamarine", "66cdaa"},
	{"mediumslateblue", "7b68ee"},
	{"mediumspringgreen", "00fa9a"},
	{"mediumturquoise", "48d1cc"},
	{"mediumvioletred", "c71585"},
};

/* ==================================================================================================== */

bool extcss3_minify_hash(char *str, unsigned short int len, extcss3_token *token, int *error)
{
	unsigned short int i;

	if ((token == NULL) || (str == NULL)) {
		*error = EXTCSS3_ERR_NULL_PTR;
		return EXTCSS3_FAILURE;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	if (len == 4) {
		if (EXTCSS3_CHARS_EQ(str[3], 'f')) {
			return extcss3_minify_hash(str, 3, token, error);
		}
	} else if (len == 8) {
		if (EXTCSS3_CHARS_EQ(str[6], 'f') && EXTCSS3_CHARS_EQ(str[7], 'f')) {
			return extcss3_minify_hash(str, 6, token, error);
		} else if (
			EXTCSS3_CHARS_EQ(str[0], str[1]) &&
			EXTCSS3_CHARS_EQ(str[2], str[3]) &&
			EXTCSS3_CHARS_EQ(str[4], str[5]) &&
			EXTCSS3_CHARS_EQ(str[6], str[7])
		) {
			token->user.len = 5;
			if ((token->user.str = (char *)calloc(1, sizeof(char) * (token->user.len))) == NULL) {
				*error = EXTCSS3_ERR_MEMORY;
				return EXTCSS3_FAILURE;
			}

			token->user.str[0] = '#';
			token->user.str[1] = str[1];
			token->user.str[2] = str[3];
			token->user.str[3] = str[5];
			token->user.str[4] = str[7];
		}
	} else if (len == 6) {
		if (
			EXTCSS3_CHARS_EQ(str[0], str[1]) &&
			EXTCSS3_CHARS_EQ(str[2], str[3]) &&
			EXTCSS3_CHARS_EQ(str[4], str[5])
		) {
			if (EXTCSS3_CHARS_EQ(str[0], 'f') && (str[2] == '0') && (str[4] == '0')) {
				token->user.len = 3;
				if ((token->user.str = (char *)calloc(1, sizeof(char) * (token->user.len))) == NULL) {
					*error = EXTCSS3_ERR_MEMORY;
					return EXTCSS3_FAILURE;
				}

				memcpy(token->user.str, "red", 3);
			} else {
				token->user.len = 4;
				if ((token->user.str = (char *)calloc(1, sizeof(char) * (token->user.len))) == NULL) {
					*error = EXTCSS3_ERR_MEMORY;
					return EXTCSS3_FAILURE;
				}

				token->user.str[0] = '#';
				token->user.str[1] = str[1];
				token->user.str[2] = str[3];
				token->user.str[3] = str[5];
			}
		} else {
			for (i = 0; i < (sizeof(extcss3_hash_colors) / sizeof(extcss3_hash_colors[0])); i++) {
				if (strncasecmp(str, extcss3_hash_colors[i][0], 6) == 0) {
					token->user.len = strlen(extcss3_hash_colors[i][1]);
					if ((token->user.str = (char *)calloc(1, sizeof(char) * (token->user.len))) == NULL) {
						*error = EXTCSS3_ERR_MEMORY;
						return EXTCSS3_FAILURE;
					}

					memcpy(token->user.str, extcss3_hash_colors[i][1], token->user.len);
				}
			}
		}
	} else if (len == 3) {
		if (EXTCSS3_CHARS_EQ(str[0], 'f') && (str[1] == '0') && (str[2] == '0')) {
			token->user.len = 3;
			if ((token->user.str = (char *)calloc(1, sizeof(char) * (token->user.len))) == NULL) {
				*error = EXTCSS3_ERR_MEMORY;
				return EXTCSS3_FAILURE;
			}

			memcpy(token->user.str, "red", 3);
		}
	}

	return EXTCSS3_SUCCESS;
}

bool extcss3_minify_color(extcss3_token *token, int *error)
{
	unsigned short int i, elements;
	const char *(*map)[2];

	if (token == NULL) {
		*error = EXTCSS3_ERR_NULL_PTR;
		return EXTCSS3_FAILURE;
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	switch (token->data.len) {
		case 8:
			map = extcss3_color_hashes_08;
			elements = sizeof(extcss3_color_hashes_08) / sizeof(extcss3_color_hashes_08[0]);
			break;

		case 9:
			map = extcss3_color_hashes_09;
			elements = sizeof(extcss3_color_hashes_09) / sizeof(extcss3_color_hashes_09[0]);
			break;

		case 10:
			map = extcss3_color_hashes_10;
			elements = sizeof(extcss3_color_hashes_10) / sizeof(extcss3_color_hashes_10[0]);
			break;

		case 11:
			map = extcss3_color_hashes_11;
			elements = sizeof(extcss3_color_hashes_11) / sizeof(extcss3_color_hashes_11[0]);
			break;

		case 12:
			map = extcss3_color_hashes_12;
			elements = sizeof(extcss3_color_hashes_12) / sizeof(extcss3_color_hashes_12[0]);
			break;

		case 13:
			map = extcss3_color_hashes_13;
			elements = sizeof(extcss3_color_hashes_13) / sizeof(extcss3_color_hashes_13[0]);
			break;

		case 14:
			map = extcss3_color_hashes_14;
			elements = sizeof(extcss3_color_hashes_14) / sizeof(extcss3_color_hashes_14[0]);
			break;

		default:
			map = extcss3_color_hashes_xx;
			elements = sizeof(extcss3_color_hashes_xx) / sizeof(extcss3_color_hashes_xx[0]);

	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	for (i = 0; i < elements; i++) {
		if (
			(strlen(map[i][0]) == token->data.len) &&
			(strncasecmp(token->data.str, map[i][0], token->data.len) == 0)
		) {
			token->user.len = strlen(map[i][1]) + 1;

			if ((token->user.str = (char *)calloc(1, sizeof(char) * (token->user.len))) == NULL) {
				*error = EXTCSS3_ERR_MEMORY;
				return EXTCSS3_FAILURE;
			}

			*token->user.str = '#';
			memcpy(token->user.str + 1, map[i][1], token->user.len - 1);

			break;
		}
	}

	return EXTCSS3_SUCCESS;
}
