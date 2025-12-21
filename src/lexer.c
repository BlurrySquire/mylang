#include <lexer.h>

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

// Don't actually know if c has these functions
// I previously just got them from <string> in c++

bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void lexer_create(lexer* lex, const char* source, uint64_t length) {
	lex->source = source;
	lex->length = length;
	lex->position = 0;
	
	darray_create(&lex->tokens, sizeof(token), 1);
}

void lexer_destroy(lexer* lex) {
	lex->source = NULL;
	lex->length = 0;
	lex->position = 0;
	
	darray_destroy(&lex->tokens);
}

char peek(lexer* lex, int64_t offset) {
	if (lex->position + offset < lex->length) {
		return lex->source[lex->position + offset];
	}
	else {
		return '\0';
	}
}

char consume(lexer* lex) {
	if (lex->position < lex->length) {
		return lex->source[lex->position++];
	}
	else {
		return '\0';
	}
}

bool lexer_tokenise(lexer* lex) {
	while (peek(lex, 0) != '\0') {
		// TODO: \n, \r, etc
		if (peek(lex, 0) == '\"') {
			token tok;
			tok.type = TOKEN_STRING_LIT;
			
			consume(lex);

			darray_create(&tok.value, sizeof(char), 1);

			while (peek(lex, 0) != '\0' && peek(lex, 0) != '\"') {
				char c = consume(lex);
				darray_push(&tok.value, &c);
			}

			if (peek(lex, 0) == '\0') {
				printf("Got to end of file before end of string '\"'.\n");
				return false;
			}

			consume(lex);

			char null = '\0';
			darray_push(&tok.value, &null);

			darray_push(&lex->tokens, &tok);
		}
		else if (is_digit(peek(lex, 0))) {
			token tok;
			tok.type = TOKEN_INT_LIT;

			darray_create(&tok.value, sizeof(char), 1);

			while (peek(lex, 0) != '\0' && is_digit(peek(lex, 0))) {
				char c = consume(lex);
				darray_push(&tok.value, &c);
			}

			char null = '\0';
			darray_push(&tok.value, &null);

			darray_push(&lex->tokens, &tok);
		}
		else if (is_alpha(peek(lex, 0)) || peek(lex, 0) == '_') {
			token tok;
			tok.type = TOKEN_IDENTIFIER;

			darray_create(&tok.value, sizeof(char), 1);

			while (peek(lex, 0) != '\0' && (is_alpha(peek(lex, 0)) || peek(lex, 0) == '_')) {
				char c = consume(lex);
				darray_push(&tok.value, &c);
			}

			char null = '\0';
			darray_push(&tok.value, &null);

			darray_push(&lex->tokens, &tok);
		}
		else if (peek(lex, 0) == '(') {
			consume(lex);
			
			token tok;
			memset(&tok, 0, sizeof(token));
			tok.type = TOKEN_LEFT_PAREN;

			darray_push(&lex->tokens, &tok);
		}
		else if (peek(lex, 0) == ')') {
			consume(lex);
			
			token tok;
			memset(&tok, 0, sizeof(token));
			tok.type = TOKEN_RIGHT_PAREN;

			darray_push(&lex->tokens, &tok);
		}
		else if (peek(lex, 0) == ',') {
			consume(lex);
			
			token tok;
			memset(&tok, 0, sizeof(token));
			tok.type = TOKEN_COMMA;

			darray_push(&lex->tokens, &tok);
		}
		else if (peek(lex, 0) == ';') {
			consume(lex);
			
			token tok;
			memset(&tok, 0, sizeof(token));
			tok.type = TOKEN_SEMICOLON;

			darray_push(&lex->tokens, &tok);
		}
		else {
			char c = peek(lex, 0);
			if (c == '\n' || c == '\r' || c == ' ' || c == '\t') {
				consume(lex);
			}
			else {
				printf("Unexpected character: %c\n", consume(lex));
			}
		}
	}

	return true;
}