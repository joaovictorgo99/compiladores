/*
Pascal Syntax Diagrams
Source: Peter Grogono's "Programming in PASCAL" Addison Wesley 1980 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>
#include <keywords.h>
#include <main.h>
#include <symtab.h>

int lookahead;
int lexlevel = 1;  // Variável para nível léxico
int lexlevel_counter = 0;  // Variável para contador de nível léxico
int error_counter = 0;  // Variável para contador de erros

void mypas(void) {  // Programa
	match(PROGRAM);
	match(ID);
	match('(');
	idlist();
	match(')');
	match(';');
	block();
	match('.');
}

int idlist(void) {  // Lista de identficadores
	int first_index = symtab_next_entry;
	int error_status;

_idlist:
	if ((error_status = symtab_append(lexeme, lexlevel))) {
		error_counter++;

		if (error_status == -2) {
			fprintf(stderr, "symtab overflow\n");
		}

		if (error_status == -3) {
			fprintf(stderr, "symbol already existing in the same lexlevel\n");
			lexlevel++;
			lexlevel_counter++;
			goto _idlist;
		}
	}

	match(ID);

	if (lookahead == ',') {
		match(',');
		goto _idlist;
	}

	return first_index;
}

void block(void) {  // Bloco
	vardef();
	subprogs();
	beginstmt();
}

int type(void) {  // Tipo
	int t = 0;

	switch (lookahead) {
        case INTEGER:
        case LONG:
        case REAL:
        case DOUBLE:
			t = lookahead;
            match(lookahead);
            break;
        default:
            match(BOOLEAN);
			return BOOLEAN;
    }

	return t;
}

void vardef(void) {  // Variável
	int i, j, t;

	if (lookahead == VAR) {
		match(VAR);

_idlist:
		i = idlist();
		j = symtab_next_entry;
		match(':');
		t = type();

		for (i; i < j; i++) {
			symtab[i].objtype = 0;
			symtab[i].type = t;
			strcpy(symtab[i].offset, "");
		}

		match(';');

		if (lookahead == ID) {
			goto _idlist;
		}
	}
}

void subprogs(void) {  // Subprograma
	while (lookahead == PROCEDURE || lookahead == FUNCTION) {
		int hasfunc = 0, i, j, t;

		switch (lookahead) {
			case PROCEDURE:
				match(PROCEDURE);
				break;
			default:
				match(FUNCTION);
				hasfunc = 1;
		}

		lexlevel++;
		i = idlist();
		j = symtab_next_entry;
		
		parmlst();

		if (hasfunc) {
			match(':');
			t = type();

			for (i; i < j; i++) {
				symtab[i].objtype = 2;
				symtab[i].type = t;
				strcpy(symtab[i].offset, "");
			}
		} else {
			for (i; i < j; i++) {
				symtab[i].objtype = 3;
				symtab[i].type = t;
				strcpy(symtab[i].offset, "");
			}
		}

		match(';');
		block();
		match(';');

		if (lexlevel_counter > 0) {
			lexlevel = lexlevel - lexlevel_counter;  // Retorna nível léxico para o nível correto (antes dos erros)
			lexlevel_counter = 0;  // Reseta contador
		}

		lexlevel--;
	}
}

void beginstmt(void) {  // Início de declarações
	match(BEGIN);
	stmtlst();
	match(END);
}

void stmtlst(void) {  // Lista de declarações
_stmtlst:
	stmt();

	if (lookahead == ';') {
		match(';');
		goto _stmtlst;
	}
}

void parmlst(void) {  // Lista de parâmetros
    int i, j, t;

	if (lookahead == '(') {
		match('(');

_parmlst:
		if (lookahead == VAR) {
			match(VAR);
		}

		i = idlist();
        j = symtab_next_entry;
		match(':');
		t = type();

		for (i; i < j; i++) {
			symtab[i].objtype = 1;
			symtab[i].type = t;
			strcpy(symtab[i].offset, "");
		}

		if (lookahead == ';') {
			match(';');
			goto _parmlst;
		}

		match(')');
	} else {
		;
	}
}

void stmt(void) {  // Declaração
	switch (lookahead) {
		case BEGIN:
			beginstmt();
			break;
		case ID:
			match(ID);

			if (lookahead == ASGN) {
				match(ASGN);
				expr();
			} else {
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

void ifstmt(void) {  // Declaração "se"
	match(IF);
	expr();
	match(THEN);
	stmt();

	if (lookahead == ELSE) {
		match(ELSE);
		stmt();
	}
}

void repstmt(void) {  // Declaração "repetir"
	match(REPEAT);
	stmtlst();
	match(UNTIL);
	expr();
}

void whilestmt(void) {  // Declaração "enquanto"
	match(WHILE);
	expr();
	match(DO);
	stmt();
}

void exprlst(void) {  // Lista de expressões
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

int isrelop(void) {  // Verifica se token é símbolo de menor, menor ou igual, não igual, maior ou maior ou igual
	switch (lookahead) {
		case '<':
		case LEQ:
		case NEQ:
		case '>':
		case GEQ:
			return lookahead;
			break;
		default:
			return 0;
	}
}

void expr(void) {  // Expressão
	smpexpr();

	if (isrelop()) {
		match(lookahead);
		smpexpr();
	}
}

int isoplus(void) {  // Verifica se token é símbolo de adição, subtração ou OU
	switch (lookahead) {
		case '+':
		case '-':
		case OR:
			return lookahead;
			break;
		default:
			return 0;
	}
}

void smpexpr(void) {  // Expressão simples
	if (lookahead == '+' || lookahead == '-') {  // Token é símbolo de adição ou subtração
		match(lookahead);
	}

	term();
	
	while (isoplus()) {
		match(lookahead);
		term();
	}
}

int isotimes(void) {  // Verifica se token é símbolo de multiplicação, divisão, divisão inteira, módulo ou E
	switch (lookahead) {
		case '*':
		case '/':
		case DIV:
		case MOD:
		case AND:
			return lookahead;
			break;
		default:
			return 0;
	}
}

void term(void) {  // Termo
	factor();

	while (isotimes()) {
		match(lookahead);
		factor();
	}
}

void factor(void) {  // Fator
	switch (lookahead) {
		case ID:
			match(ID);
			exprlst();
			break;
		case UINT32:
			match(UINT32);
			break;
		case UINT64:
			match(UINT64);
			break;
		case FLOAT32:
			match(FLOAT32);
			break;
		case FLOAT64:
			match(FLOAT64);
			break;
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
	//printf("lookahead %d\n", lookahead);
	//printf("expected %d\n", expected);
	if (lookahead == expected) {  // Token reconhecido
		lookahead = gettoken(source);
	} else if (lookahead == EOF) {  // Erro de fim de arquivo
		fprintf(stderr, "premature EOF found at line %d\n", linenum);
		exit(-1);
	} else {  // Erro de sintaxe
		fprintf(stderr, "syntax error at line %d\n", linenum);
		error_counter++;
		exit(-4);
	}
}
