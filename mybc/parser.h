#include <stdio.h>

#pragma once

extern int lookahead;
extern double result; 

extern int gettoken(FILE *);
extern void match(int);
void E(void);
void T(void);
void R(void);
void Q(void);
void F(void);
