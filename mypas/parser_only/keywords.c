#include <stdio.h>
#include <string.h>
#include <keywords.h>

char *keyword[] = {
	"begin",
	"program",
	"var",
	"procedure",
	"function",
	"if",
	"then",
	"else",
	"repeat",
	"until",
	"while",
	"do",
	"in",
	"div",
	"mod",
	"and",
	"or",
	"not",
	"true",
	"false",
	"integer",  // 32 bits
	"long",  // 64 bits
	"real",  // 32 bits
	"double",  // 64 bits
	"boolean",  // 1 byte ou 4 bytes
	"end"
};

int iskeyword(char *identifier) {
	int i;

	for (i = BEGIN; i <= END; i++) {
		if (strcmp(identifier, keyword[i-BEGIN]) == 0) {
			return i;
		}
	}

	return 0;
}

