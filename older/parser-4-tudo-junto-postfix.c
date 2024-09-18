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
    int ominus = 0, oplus = 0, otimes = 0, op;

    if (lookahead == '+' || lookahead == '-') {        
        if (lookahead == '-') {
            ominus = '-';
        }

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
            printf(" %s ", lexeme);
            match(ID);
            break;
        case OCT:
            printf(" %s ", lexeme);
            match(OCT);
            break;
        case HEX:
            printf(" %s ", lexeme);
            match(HEX);
            break;
        default:
            printf(" %s ", lexeme);
            match(DEC);
    }

    if (otimes) {
        printf(" %c ", otimes);
        otimes = 0;
    }

    if (lookahead == '*' || lookahead == '/') {
        if (lookahead == '*') {
            otimes = '*';
        }
        else if (lookahead == '/') {
            otimes = '/';
        }

        op = lookahead;
        match(lookahead);
        goto _F;
    }

    if (ominus) {
        printf(" negate ");
        ominus = 0;
    }

    if (oplus) {
        printf(" %c ", oplus);
        oplus = 0;
    }

    if (lookahead == '+' || lookahead == '-') {
        if (lookahead == '+') {
            oplus = '+';
        }
        else if (lookahead == '-') {
            oplus = '-';
        }

        op = lookahead;
        match(lookahead);
        goto _T;
    }
}

void match(int expected) {
    if (lookahead == expected) {
        lookahead = gettoken(source);
    }
    else {
        fprintf(stderr, "token mismatch\n");
        exit(-3);
    }
}
