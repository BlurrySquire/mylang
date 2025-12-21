#ifndef _H_MYLANG_TOKEN
#define _H_MYLANG_TOKEN

#include <types/dynamic_array.h>

typedef enum token_type {
	TOKEN_NONE,

	TOKEN_STRING_LIT,
	TOKEN_INT_LIT,

	TOKEN_IDENTIFIER,

	TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
	TOKEN_COMMA,
	TOKEN_SEMICOLON,
} token_type;

typedef struct token {
	token_type type;
	darray value; // NULL if no value
} token;

const char* token_to_string(token tok);

#endif