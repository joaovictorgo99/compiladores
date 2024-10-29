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
    if (lookahead == '+' || lookahead == '-') {
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
            match(ID);
            break;
        case OCT:
            match(OCT);
            break;
        case HEX:
            match(HEX);
            break;
        default:
            match(DEC);
    }

    if (lookahead == '*' || lookahead == '/') {
        match(lookahead);
        goto _F;
    }

    if (lookahead == '+' || lookahead == '-') {
        match(lookahead);
        goto _T;
    }
}

void match(int expected) {
    if(lookahead == expected) {
        lookahead = gettoken(source);
    }
    else {
        fprintf(stderr, "token mismatch\n");
        exit(-3);
    }
}