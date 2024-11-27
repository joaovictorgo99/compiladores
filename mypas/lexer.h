#include <stdio.h>

#pragma once

#define MAXIDLEN 512

enum {
    ID = 1024,
    NUM,
    BYTE, // boolean
    UINT32,  // integer
    UINT64,  // long
    FLOAT32,  // real
    FLOAT64,  // double
    ASGN,  // :=
    LEQ,  // <=
    GEQ,  // >=
    NEQ  // <>
};

extern char lexeme[];
extern double lexval;
extern int linenum;
