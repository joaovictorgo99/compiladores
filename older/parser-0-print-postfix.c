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
    switch(lookahead) {
        case '+':
            match('+');
            T();
            printf(" + ");
            R();
            break;
        case '-':
            match('-');
            T();
            printf(" - ");
            R();
            break;
        default:
            ;
    }
}

// Q -> '*' F Q | '/' F Q | <>
void Q(void) {
    switch(lookahead) {
        case '*':
            match('*');
            F();
            printf(" * ");
            Q();
            break;
        case '/':
            match('/');
            F();
            printf(" / ");
            Q();
            break;
        default:
            ;
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
            printf(" id ");
            match(ID);
            break;
        case OCT:
            printf(" oct ");
            match(OCT);
            break;
        case HEX:
            printf(" hex ");
            match(HEX);
            break;
        default:
            match(DEC);
            printf(" dec ");
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