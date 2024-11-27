#include <stdio.h>

#pragma once

enum {
	BEGIN = 0X1000000,
	PROGRAM,
	VAR,
	PROCEDURE,
	FUNCTION,
	IF,
	THEN,
	ELSE,
	REPEAT,
	UNTIL,
	WHILE,
	DO,
	IN,
	DIV,
	MOD,
	AND,
	OR,
	NOT,
	TRUE,
	FALSE,
	INTEGER,
	LONG,
	REAL,
	DOUBLE,
	BOOLEAN,
	END
};

int iskeyword(char *);

