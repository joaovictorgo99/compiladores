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

_T:
    T();
    if (lookahead == '+' || lookahead == '-') {
        match(lookahead);
        goto _T;
    }
}

// T -> F { otimes F }
void T(void) {
_F:
    F();
    if (lookahead == '*' || lookahead == '/') {
        match(lookahead);
        goto _F;
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
