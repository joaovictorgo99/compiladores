#include <stdio.h>

#define MAXIDLEN 512
#define ID 1024
#define UINT32 1025  // integer
#define UINT64 1026  // long
#define FLOAT32 1027  // real
#define FLOAT64 1028  // double
#define ASGN 1029  // :=
#define LEQ 1030  // <=
#define GEQ 1031  // >=
#define NEQ 1032  // <>

extern char lexeme[];
extern double lexval;
extern int linenum;
