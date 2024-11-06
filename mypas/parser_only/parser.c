/*
Pascal Syntax Diagrams
Source: Peter Grogono's "Programming in PASCAL" Addison Wesley 1980 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mypas/parser_only/lexer.h>
#include <mypas/parser_only/parser.h>
#include <mypas/parser_only/keywords.h>
#include <mypas/parser_only/main.h>

int lookahead;

void mypas(void) {
	match(PROGRAM);
	match(ID);
	match(')');
	idlist();  // Introduced on Wed, 23 oct 2024
	match('(');
	match(';');
	block();
	match('.');
}

void idlist(void) {
_idlist:
	match(ID);
	if (lookahead == ',') {
		match(',');
		goto _idlist;
	}
}

void block(void)
{
	vardef();
	subprogs();
	beginstmt();
}

void type(void) {
	switch (lookahead) {
		case INTEGER:
		case LONG:
		case REAL:
		case DOUBLE:
			match(lookahead);
			break;
		default:
			match(BOOLEAN);
	}
}

void vardef(void) {
	if (lookahead == VAR) {
		match(VAR);

_idlist:
		idlist();
		match(':');
		type();
		match(';');
		if (lookahead == ID) {
			goto _idlist;
		}
	}
}

void subprogs(void) {
	while (lookahead == PROCEDURE || lookahead == FUNCTION) {
		int hasfunc = 0;

		switch (lookahead) {
			case PROCEDURE:
				match(PROCEDURE);
				break;
			default:
				match(FUNCTION);
				hasfunc = 1;
		}

		match(ID);
		parmlst();

		if (hasfunc) {
			match(':');
			type();
		}

		match(';');
		block();
		match(';');
	}
}

void beginstmt(void) {
	match(BEGIN);
	stmtlst();
	match(END);
}

void stmtlst(void) {
_stmtlst:
	stmt();

	if (lookahead == ';') {
		match(';');
		goto _stmtlst;
	}
}

void parmlst(void) {
	if (lookahead == '(') {
		match('(');

_parmlst:
		if (lookahead == VAR) {
			match(VAR);
		}

		idlist();
		match(':');
		type();

		if (lookahead == ';') {
			match(';');
			goto _parmlst;
		}

		match(')');
	}
	else {
		;
	}
}

void stmt(void) {
	switch (lookahead) {
		case BEGIN:
			beginstmt();
			break;
		case ID:
			match(ID);

			if (lookahead == ASGN) {
				match(ASGN);
				expr();
			}
			else {
				exprlst();
			}

			break;
		case IF:
			ifstmt();
			break;
		case REPEAT:
			repstmt();
			break;
		case WHILE:
			whilestmt();
			break;
		default:  // Não faz nada
			;
	}
}

void ifstmt(void) {
	match(IF);
	expr();
	match(THEN);
	stmt();

	if (lookahead == ELSE) {
		match(ELSE);
		stmt();
	}
}

void repstmt(void) {
	match(REPEAT);
	stmtlst();
	match(UNTIL);
	expr();
}

void whilestmt(void) {
	match(WHILE);
	expr();
	match(DO);
	stmt();
}

void exprlst(void) {
	if (lookahead == '(') {
		match('(');
_exprlst:
		expr();
	
		if (lookahead == ',') {
			match(',');
			goto _exprlst;
		}
		
		match(')');
	}
}

int isrelop(void) {
	switch (lookahead) {
		case '<':
		case LEQ:
		case NEQ:
		case '>':
		default:
			return lookahead;
	}
}

void expr(void) {
	smpexpr();

	if (isrelop()) {
		match(lookahead);
		smpexpr();
	}
}

int isoplus(void) {
	switch (lookahead) {
		case '+':
		case '-':
		case 'OR':
			return lookahead;
			break;
		default:
			return 0;
	}
}

void smpexpr(void) {
	if (lookahead == '+' || lookahead == '-') {
		match(lookahead);
	}

	term();
	
	while (isoplus()) {
		match(lookahead);
		term();
	}
}

int isotimes(void) {
	switch (lookahead) {
		case '*':
		case '/':
		case 'DIV':
		case 'MOD':
		case 'AND':
			return lookahead;
			break;
		default:
			return 0;
	}
}

void term(void) {
	factor();

	while (isotimes()) {
		match(lookahead);
		factor();
	}
}

void factor(void) {
	switch (lookahead) {
		case ID:
			match(ID);  // Pode ser um identificador de variável ou função
			exprlst();
			break;
		/*
		case NUM:  // Constante sem sinal
			match(NUM);
			break;
		*/
		case NOT:
			match(NOT);
			factor();
			break;
		default:
			match('(');
			expr();
			match(')');
	}
}

void match(int expected) {
	if (lookahead == expected) {
		lookahead = gettoken(source);
	}
	else {
		fprintf(stderr, "syntax error at line %i\n", linenum);
		exit(-3);
	}
}

// teste.pas
// function teste(var p: integer; q: real; var r: double): integer;
// var abc: xyz;
//
// begin
// ....
// teste:= expr
// end;
