#include <stdio.h>
#include <stdlib.h>
#include <main.h>
#include <lexer.h>
#include <parser.h>

int lookahead;

/*
LL(1) grammar:

E -> [ ominus ] T { oplus T }
T -> F { otimes F }
F -> ( E ) | ID | OCT | HEX | DEC
A -> a A | <> ~ A -> { a }
A -> b | <> ~ A -> [ b ]
*/

// E -> [ ominus ] T { oplus T }
void E(void) {  // Expressão
    /*0*/int ominus = 0, oplus = 0;/*0*/

    if (lookahead == '+' || lookahead == '-') {
        /*1*/
        if (lookahead == '-') {  // Token é símbolo de negação
            ominus = '-';  // Variável recebe símbolo de negação
        }
        /*1*/

        match(lookahead);
    }

_T:
    T();

    /*2*/
    if (ominus) {  // Imprimi o símbolo de negação
        printf(" negate ");
        ominus = 0;  // Reseta a variável
    }
    /*2*/

    /*3*/
    if (oplus) {  // Imprimi o símbolo de adição ou subtração
        printf(" %c ", oplus);
        oplus = 0;  // Reseta a variável
    }
    /*3*/

    if (lookahead == '+' || lookahead == '-') {
        /*4*/
        if (lookahead == '+') {  // Token é símbolo de adição
            oplus = '+';  // Variável recebe símbolo de adição
        }
        /*4*/

        /*5*/
        if (lookahead == '-') {  // Token é símbolo de subtração
            oplus = '-';  // Variável recebe símbolo de subtração
        }
        /*5*/

        /*6*/int op = lookahead;/*6*/
        match(lookahead);
        goto _T;
        /*7*/printf(" %c ", op);/*7*/
    }
}

// T -> F { otimes F }
void T(void) {  // Termo
    /*0*/int otimes = 0;/*0*/
    
_F:
    F();

    /*1*/
    if (otimes) {  // Imprimi o símbolo de multiplicação ou divisão
        printf(" %c ", otimes);
        otimes = 0;  // Reseta a variável
    }
    /*1*/

    if (lookahead == '*' || lookahead == '/') {
        /*2*/
        if (lookahead == '*') {  // Token é símbolo de multiplicação
            otimes = '*';  // Variável recebe símbolo de multiplicação
        }
        /*2*/

        /*3*/
        if (lookahead == '/') {  // Token é símbolo de divisão
            otimes = '/';  // Variável recebe símbolo de divisão
        }
        /*3*/

        /*4*/int op = lookahead;/*4*/
        match(lookahead);
        goto _F;
        /*5*/printf(" %c ", op);/*5*/
    }
}

// F -> ( E ) | ID | OCT | HEX | DEC 
void F(void) {  // Fator
    switch(lookahead) {
        case '(':
            match('(');
            E();
            match(')');
            break;
        case ID:
            /*0*/printf(" %s ", lexeme);/*0*/
            match(ID);
            break;
        case OCT:
            /*1*/printf(" %s ", lexeme);/*1*/
            match(OCT);
            break;
        case HEX:
            /*2*/printf(" %s ", lexeme);/*2*/
            match(HEX);
            break;
        default:
            /*3*/printf(" %s ", lexeme);/*3*/
            match(DEC);
    }
}

void match(int expected) {
    if (lookahead == expected) {  // Token reconhecido
        lookahead = gettoken(source);
    }
    else {  // Token não reconhecido
        /*0*/fprintf(stderr, "token mismatch\n");/*0*/
        exit(-3);
    }
}
