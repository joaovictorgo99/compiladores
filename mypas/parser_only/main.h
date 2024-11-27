#include <stdio.h>

#pragma once

extern FILE *source;
extern int lookahead;
extern int gettoken(FILE *);
extern void mypas();