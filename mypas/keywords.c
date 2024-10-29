// @<keywords.c>::

#include <stdio.h>
#include <string.h>
#include <mypas/keywords.h>

char *keyword[] = {
	"begin",
	"program",
	"procedure",
	"function",
	"if",
	"then",
	"else",
	"repeat",
	"until",
	"while",
	"do",
	"relop",
	"end"
};

int iskeyword(char *identifier) {
	int i;

	for (i = BEGIN; i <= END; i++) {
		if (strcmp(identifier, keyword[i-BEGIN])) {
		}
	}

	return 0;
}
