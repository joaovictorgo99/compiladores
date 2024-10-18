#include <stdio.h>
#include <stdlib.h>
#include <main.h>
#include <lexer.h>
#include <parser.h>
#include <string.h>

/* PILHA */
// Variáveis
#define STACKSIZ 1024
double stack[STACKSIZ];
int sp = -1;

// Funções
void push(double x) {  // Empilha valor na pilha
    sp++;
    stack[sp] = x;
}

double pop(void) {  // Desempilha valor na pilha
    double x = stack[sp];
    sp--;

    return x;
}
/* PILHA */

/* CALCULADORA */
// Variáveis
double acc;

// Funções
double compute(int op, double x, double y) {  // Realiza a computação entre os valores
    double result;

    switch (op) {
        case '+':
            result = x+y;
            break;
        case '-':
            result = x-y;
            break;
        case '*':
            result = x*y;
            break;
        case '/':
            result = x/y;
            break;
        default:  // Não faz nada
            ;
    }

    return result;
}
/* CALCULADORA */

/* SÍMBOLOS */
// Variáveis
#define MAXSYMTABENTRIES 1024
char symtab[MAXSYMTABENTRIES][MAXIDLEN+1];
double virtmem[MAXSYMTABENTRIES];
int symtab_next_entry = 0;

// Funções
int symtab_lookup(char *varname) {  // Busca e retorna a posição do símbolo da tabela
    int i;

    for (i = 0; i < symtab_next_entry; i++) {
        if (strcmp(varname, symtab[i]) == 0) {
            return i;
        }
    }

    strcpy(symtab[i], varname);
    
    return symtab_next_entry++;
}

void store(char *varname, double val) {  // Busca na memória e salva o valor
    int i = symtab_lookup(varname);

    virtmem[i] = val;
}

double recall(char *varname) {  // Retorna o valor na memória
    int i = symtab_lookup(varname);

    return virtmem[i];
}
/* SÍMBOLOS */

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
void E(void) {  // Expressão
    /*0*/int ominus = 0, oplus = 0;/*0*/

    if (lookahead == '+' || lookahead == '-') {
        /*1*/
        if (lookahead == '-') {  // Token é símbolo de negação
            ominus = '-';  // Variável recebe símbolo de negação
        }
        /*1*/

        match(lookahead);
    }

_T:
    T();

    /*2*/
    if (ominus) {  // Imprimi o símbolo de negação
        printf("\tneg acc\n");
        // Ideia: acc = -acc;
        ominus = 0;  // Reseta a variável
    }
    /*2*/

    /*3*/
    if (oplus) {  // Imprimi o símbolo de adição ou subtração
        switch(oplus) {
            case '+':
                printf("\tadd stack[sp], acc\n");
                // Ideia: acc = compute('+', stack[sp], acc);
                break;
            case '-':
                printf("\tsub acc, stack[sp]\n");
                printf("\tpop acc\n");
                break;
            default:  // Não faz nada
                ;
        }
        
        oplus = 0;  // Reseta a variável
    }
    /*3*/

    if (lookahead == '+' || lookahead == '-') {
        /*4*/
        oplus = lookahead;
        printf("\tpush acc\n");
        // Ideia: push(acc);
        /*4*/

        /*5*/int op = lookahead;/*5*/
        match(lookahead);
        goto _T;
        /*6*/printf(" %c ", op);/*6*/
    }
}

// T -> F { otimes F }
void T(void) {  // Termo
    /*0*/int otimes = 0;/*0*/
    
_F:
    F();

    /*1*/
    if (otimes) {  // Imprimi o símbolo de multiplicação ou divisão
        printf(" %c ", otimes);
        otimes = 0;  // Reseta a variável
    }
    /*1*/

    if (lookahead == '*' || lookahead == '/') {
        /*2*/
        if (lookahead == '*') {  // Token é símbolo de multiplicação
            otimes = '*';  // Variável recebe símbolo de multiplicação
        }
        /*2*/

        /*3*/
        if (lookahead == '/') {  // Token é símbolo de divisão
            otimes = '/';  // Variável recebe símbolo de divisão
        }
        /*3*/

        /*4*/int op = lookahead;/*4*/
        match(lookahead);
        goto _F;
        /*5*/printf(" %c ", op);/*5*/
    }
}

// F -> ( E ) | ID | OCT | HEX | DEC 
void F(void) {  // Fator
    /**/char varname[MAXIDLEN+1];/**/
    switch(lookahead) {
        case '(':
            match('(');
            E();
            match(')');
            break;
        case ID:
            /*0*/printf(" %s ", lexeme);/*0*/
            /**/strcpy(varname, lexeme);/**/
            match(ID);
            
            if (lookahead == ASGN) {
                // L-value
                match(ASGN);
                E();
                printf("\tstore acc, %s\n", varname);
                // Ideia: store(varname, acc);
            }
            else {
                // certo?
                // R-value (usa recall)
                match(ASGN);
                E();
                printf("\recall acc, %s\n", varname);
                // Ideia: recall(varname, acc);
                // certo?
            }
            
            break;
        default:
            /**/printf("\tmov %s, acc\n", lexeme);/**/
            // Ideia: acc = atof(lexeme);
            match(NUM);
    }
}

void match(int expected) {
    if (lookahead == expected) {  // Token reconhecido
        lookahead = gettoken(source);
    }
    else {  // Token não reconhecido
        /*0*/fprintf(stderr, "token mismatch\n");/*0*/
        exit(-3);
    }
}