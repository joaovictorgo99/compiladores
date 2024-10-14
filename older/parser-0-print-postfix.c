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
            /*0*/printf(" + ");/*0*/
            R();
            break;
        case '-':
            match('-');
            T();
            /*1*/printf(" - ");/*1*/
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
            /*0*/printf(" * ");/*0*/
            Q();
            break;
        case '/':
            match('/');
            F();
            /*1*/printf(" / ");/*1*/
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
            /*0*/printf(" id ");/*0*/
            match(ID);
            break;
        case OCT:
            /*1*/printf(" oct ");/*1*/
            match(OCT);
            break;
        case HEX:
            /*2*/printf(" hex ");/*2*/
            match(HEX);
            break;
        default:
            match(DEC);
            /*3*/printf(" dec ");/*3*/
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