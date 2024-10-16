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
    /*0*/int ominus = 0, oplus = 0, otimes = 0, op;/*0*/

    if (lookahead == '+' || lookahead == '-') {
        /*1*/
        if (lookahead == '-') {  // Token é símbolo de negação
            ominus = '-';  // Variável recebe símbolo de negação
        }
        /*1*/

        match(lookahead);
    }

// T -> F { otimes F }
_T:  // Termo
// F -> ( E ) | ID | OCT | HEX | DEC 
_F:  // Fator
    switch(lookahead) {
        case '(':
            match('(');
            E();
            match(')');
            break;
        case ID:
            /*2*/printf(" %s ", lexeme);/*2*/
            match(ID);
            break;
        case OCT:
            /*3*/printf(" %s ", lexeme);/*3*/
            match(OCT);
            break;
        case HEX:
            /*4*/printf(" %s ", lexeme);/*4*/
            match(HEX);
            break;
        default:
            /*5*/printf(" %s ", lexeme);/*5*/
            match(DEC);
    }

    /*6*/
    if (otimes) {  // Imprimi o símbolo de multiplicação ou divisão
        printf(" %c ", otimes);
        otimes = 0;  // Reseta a variável
    }
    /*6*/

    if (lookahead == '*' || lookahead == '/') {
        /*7*/
        if (lookahead == '*') {  // Token é símbolo de multiplicação
            otimes = '*';  // Variável recebe símbolo de multiplicação
        }
        else if (lookahead == '/') {  // Token é símbolo de divisão
            otimes = '/';  // Variável recebe símbolo de divisão
        }
        /*7*/

        /*8*/op = lookahead;/*8*/
        match(lookahead);
        goto _F;
    }

    /*9*/
    if (ominus) {  // Imprimi o símbolo de negação
        printf(" negate ");
        ominus = 0;  // Reseta a variável
    }
    /*9*/

    /*10*/
    if (oplus) {  // Imprimi o símbolo de adição ou subtração
        printf(" %c ", oplus);
        oplus = 0;  // Reseta a variável
    }
    /*10*/

    if (lookahead == '+' || lookahead == '-') {
        /*11*/
        if (lookahead == '+') {  // Token é símbolo de adição
            oplus = '+';  // Variável recebe símbolo de adição
        }
        else if (lookahead == '-') {  // Token é símbolo de subtração
            oplus = '-';  // Variável recebe símbolo de subtração
        }
        /*11*/

        /*12*/op = lookahead;/*12*/
        match(lookahead);
        goto _T;
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
