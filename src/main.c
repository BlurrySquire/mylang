#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <string.h>

#include <types/dynamic_array.h>

#include <token.h>
#include <lexer.h>

const char* token_to_string(token tok) {
	switch (tok.type) {
		case TOKEN_NONE: return "NONE";

		case TOKEN_STRING_LIT: return "STRING_LIT";
		case TOKEN_INT_LIT: return "INT_LIT";

		case TOKEN_IDENTIFIER: return "IDENTIFIER";

		case TOKEN_LEFT_PAREN: return "LEFT_PAREN";
		case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";
		case TOKEN_COMMA: return "COMMA";
		case TOKEN_SEMICOLON: return "SEMICOLON";

		default: return "UNKNOWN";
	}
}

static const char* input_file = "";
static const char* output_file = "out.elf";

bool parse_args(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Error: No input file specified.\n");
		printf("./mylang <input_file> <output_file>\n");
		return false;
	}

	input_file = argv[1];

	if (argc >= 3) {
		output_file = argv[2];
	}
	else {
		printf ("Warning: No output file specified. '%s' is being used instead.\n", output_file);
	}

	if (argc > 3) {
		printf("Warning: Extra argmuents specified are being ignored.\n");
	}

	return true;
}

int main(int argc, char* argv[]) {
	if (!parse_args(argc, argv)) {
		return 1;
	}

	FILE* file = fopen(input_file, "r");
	
	fseek(file, 0, SEEK_END);
	uint64_t size = ftell(file);
	rewind(file);

	char* buffer = malloc(size + 1);
	if (!buffer) {
		fclose(file);
		return 1;
	}

	fread(buffer, size, 1, file);

	lexer lex;
	lexer_create(&lex, buffer, size);

	if (!lexer_tokenise(&lex)) {
		return 1;
	}

	for (size_t i = 0; i < lex.tokens.length; i++) {
		token tok;
		darray_at(&lex.tokens, i, &tok);

		if (tok.value.elements != NULL) {
			printf("%s(%s)\n", token_to_string(tok), (const char*)tok.value.elements);
		}
		else {
			printf("%s\n", token_to_string(tok));
		}
	}

	for (size_t i = 0; i < lex.tokens.length; i++) {
		token tok;
		darray_at(&lex.tokens, i, &tok);

		if (tok.value.length < 1) {
			darray_destroy(&tok.value);
		}
	}

	lexer_destroy(&lex);

	return 0;
}