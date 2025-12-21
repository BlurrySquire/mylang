#ifndef _H_MYLANG_LEXER
#define _H_MYLANG_LEXER

#include <stdbool.h>

#include <types/dynamic_array.h>
#include <token.h>

typedef struct lexer {
	const char* source;
	uint64_t length;
	uint64_t position;
	darray tokens;
} lexer;

void lexer_create(lexer* lex, const char* source, uint64_t length);
void lexer_destroy(lexer* lex);

bool lexer_tokenise(lexer* lex);

#endif