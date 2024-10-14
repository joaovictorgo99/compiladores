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
void E(void) {
    /*0*/int ominus = 0;/*0*/

    if (lookahead == '+' || lookahead == '-') {
        /*1*/
        if (lookahead == '-') {
            ominus = '-';
        }
        /*1*/

        match(lookahead);
    }

    T();

    /*2*/
    if (ominus) {
        printf(" negate ");
        ominus = 0;
    }
    /*2*/

    while (lookahead == '+' || lookahead == '-') {
        /*3*/int op = lookahead;/*3*/
        match(lookahead);
        T();
        /*4*/printf(" %c ", op);/*4*/
    }
}

// T -> F { otimes F }
void T(void) {
    F();

    while (lookahead == '*' || lookahead == '/') {
        /*0*/int op = lookahead;/*0*/
        match(lookahead);
        F();
        /*1*/printf(" %c ", op);/*1*/
    }
}

// F -> ( E ) | ID | OCT | HEX | DEC 
void F(void) {
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
    if (lookahead == expected) {
        lookahead = gettoken(source);
    }
    else {
        /*0*/fprintf(stderr, "token mismatch\n");/*0*/
        exit(-3);
    }
}
