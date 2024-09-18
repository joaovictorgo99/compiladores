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
void E(void) {
    int ominus = 0;

    if (lookahead == '+' || lookahead == '-') {        
        if (lookahead == '-') {
            ominus = '-';
        }

        match(lookahead);
    }

    T();

    if (ominus) {
        printf(" negate ");
        ominus = 0;
    }

    R();
}

//T -> F Q
void T(void) {
    F();
    Q();
}

// R -> '+' T R | '-' T R | <>
void R(void) {
    while (lookahead == '+' || lookahead == '-') {
        int op = lookahead;
        match(lookahead);
        T();
        printf(" %c ", op);
    }
}

// Q -> '*' F Q | '/' F Q | <>
void Q(void) {
    while (lookahead == '*' || lookahead == '/') {
        int op = lookahead;
        match(lookahead);
        F();
        printf(" %c ", op);
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
