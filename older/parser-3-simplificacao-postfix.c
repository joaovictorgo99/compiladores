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
    /*0*/int ominus = 0, oplus = 0;/*0*/

    if (lookahead == '+' || lookahead == '-') {
        /*1*/
        if (lookahead == '-') {
            ominus = '-';
        }
        /*1*/

        match(lookahead);
    }

_T:
    T();

    /*2*/
    if (ominus) {
        printf(" negate ");
        ominus = 0;
    }
    /*2*/

    /*3*/
    if (oplus) {
        printf(" %c ", oplus);
        oplus = 0;
    }
    /*3*/

    if (lookahead == '+' || lookahead == '-') {
        /*4*/
        if (lookahead == '+') {
            oplus = '+';
        }
        /*4*/

        /*5*/
        if (lookahead == '-') {
            oplus = '-';
        }
        /*5*/

        /*6*/int op = lookahead;/*6*/
        match(lookahead);
        goto _T;
        /*7*/printf(" %c ", op);/*7*/
    }
}

// T -> F { otimes F }
void T(void) {
    /*0*/int otimes = 0;/*0*/
    
_F:
    F();

    /*1*/
    if (otimes) {
        printf(" %c ", otimes);
        otimes = 0;
    }
    /*1*/

    if (lookahead == '*' || lookahead == '/') {
        /*2*/
        if (lookahead == '*') {
            otimes = '*';
        }
        /*2*/

        /*3*/
        if (lookahead == '/') {
            otimes = '/';
        }
        /*3*/

        /*4*/int op = lookahead;/*4*/
        match(lookahead);
        goto _F;
        /*5*/printf(" %c ", op);/*5*/
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
