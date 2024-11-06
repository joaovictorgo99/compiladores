#include <stdio.h>
#include <string.h>
#include <mypas/parser_only/keywords.h>

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
	"not"
	"true",
	"false",
	"integer",
	"long",
	"real",
	"double",
	"boolean"
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
