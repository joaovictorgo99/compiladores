#include <stdio.h>

#pragma once

extern int lookahead;
extern char lexeme[];
extern void match(int expected);
extern int gettoken(FILE *);
extern FILE *source;
void mypas(void);
int idlist(void);
int type(void);
void block(void);
void vardef(void);
void subprogs(void);
void beginstmt(void);
void stmtlst(void);
void parmlst(void);
void stmt(void);
void ifstmt(void);
void repstmt(void);
void whilestmt(void);
void exprlst(void);
void expr(void);
void smpexpr(void);
void term(void);
void factor(void);
