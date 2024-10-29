#include <stdio.h>

extern int lookahead;

void E(void);
void T(void);
void R(void);
void Q(void);
void F(void);

extern int gettoken(FILE *);

extern void match(int);