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
F -> ( E ) | ID | DEC
*/

// E -> T R
void E(void) {
    T();
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
            R();
            break;
        case '-':
            match('-');
            T();
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
            Q();
            break;
        case '/':
            match('/');
            F();
            Q();
            break;
        default:
            ;
    }
}

// F -> ( E ) | ID | DEC
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