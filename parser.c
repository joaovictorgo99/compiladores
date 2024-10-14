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
    /*0*/int ominus = 0, oplus = 0, otimes = 0, op;/*0*/

    if (lookahead == '+' || lookahead == '-') {
        /*1*/
        if (lookahead == '-') {
            ominus = '-';
        }
        /*1*/

        match(lookahead);
    }

// T -> F { otimes F }
_T:
// F -> ( E ) | ID | OCT | HEX | DEC 
_F:
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
    if (otimes) {
        printf(" %c ", otimes);
        otimes = 0;
    }
    /*6*/

    if (lookahead == '*' || lookahead == '/') {
        /*7*/
        if (lookahead == '*') {
            otimes = '*';
        }
        else if (lookahead == '/') {
            otimes = '/';
        }
        /*7*/

        /*8*/op = lookahead;/*8*/
        match(lookahead);
        goto _F;
    }

    /*9*/
    if (ominus) {
        printf(" negate ");
        ominus = 0;
    }
    /*9*/

    /*10*/
    if (oplus) {
        printf(" %c ", oplus);
        oplus = 0;
    }
    /*10*/

    if (lookahead == '+' || lookahead == '-') {
        /*11*/
        if (lookahead == '+') {
            oplus = '+';
        }
        else if (lookahead == '-') {
            oplus = '-';
        }
        /*11*/

        /*12*/op = lookahead;/*12*/
        match(lookahead);
        goto _T;
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
