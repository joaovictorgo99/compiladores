#include <stdio.h>
#include <stdlib.h>
#include <main.h>
#include <lexer.h>
#include <parser.h>

int lookahead;

/*
LL(1) grammar:

E -> T R
T -> F Q
R -> '+' T R | '-' T R | <>
Q -> '*' F Q | '/' F Q | <>
F -> ( E ) | ID | OCT | HEX | DEC
*/

// E -> T R
void E(void) {  // Expressão
    /*0*/int ominus = 0;/*0*/

    if (lookahead == '+' || lookahead == '-') {
        /*1*/
        if (lookahead == '-') {  // Token é símbolo de negação
            ominus = '-';  // Variável recebe símbolo de negação
        }
        /*1*/

        match(lookahead);
    }

    T();

    /*2*/
    if (ominus) {  // Imprimi o símbolo de negação
        printf(" negate ");
        ominus = 0;  // Reseta a variável
    }
    /*2*/

    R();
}

//T -> F Q
void T(void) {  // Termo
    F();
    Q();
}

// R -> '+' T R | '-' T R | <>
void R(void) {
    while (lookahead == '+' || lookahead == '-') {
        /*0*/int op = lookahead;/*0*/
        match(lookahead);
        T();
        /*1*/printf(" %c ", op);/*1*/
    }
}

// Q -> '*' F Q | '/' F Q | <>
void Q(void) {
    while (lookahead == '*' || lookahead == '/') {
        /*0*/int op = lookahead;/*0*/
        match(lookahead);
        F();
        /*1*/printf(" %c ", op);/*1*/
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
