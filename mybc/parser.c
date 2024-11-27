#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <main.h>
#include <lexer.h>
#include <parser.h>
#include <mybc.h>

/* PILHA */
// Variáveis
#define STACKSIZ 1024
double stack[STACKSIZ];
int sp = -1;

// Funções
void push(double x) {  // Empilha valor na pilha
    if (sp < STACKSIZ) {  // Verifica se pilha está cheia
        sp++;
        stack[sp] = x;
    }
}

double pop(void) {  // Desempilha valor na pilha
    double x = stack[sp];
    sp--;

    return x;
}

/* CALCULADORA */
// Variáveis
double acc;
double result;

// Funções
double compute(int op, double x, double y) {  // Realiza a computação entre os valores
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

    //printf("\tresult %lg\n", result);

    return result;
}

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
        if (strcmp(varname, symtab[i]) == 0) {  // Encontrou, retorna posição
            return i;
        }
    }

    // Não encontrou, salva símbolo na tabela e retorna posição
    strcpy(symtab[i], varname);
    return symtab_next_entry++;
}

void store(char *varname, double val) {  // Salva o valor na memória
    int i = symtab_lookup(varname);

    if (symtab_next_entry < MAXSYMTABENTRIES) {  // Verifica se tabela está cheia
        virtmem[i] = val;
    }    
}

double recall(char *varname) {  // Retorna o valor da memória
    int i = symtab_lookup(varname);

    return virtmem[i];
}

/*
LL(1) grammar:

E -> [ ominus ] T { oplus T }
T -> F { otimes F }
F -> ( E ) | ID | OCT | HEX | DEC
A -> a A | <> ~ A -> { a }
A -> b | <> ~ A -> [ b ]
*/

int lookahead;

// E -> [ ominus ] T { oplus T }
void E(void) {  // Expressão
    /*0 notacao semantica*/
    int ominus = 0, oplus = 0;
    /*0*/

    if (lookahead == '+' || lookahead == '-') {
        /*1 notacao semantica*/
        if (lookahead == '-') {  // Token é símbolo de negação
            ominus = '-';  // Variável recebe símbolo de negação
        }
        /*1*/

        match(lookahead); //confirma que a estrutura do E esta correta [E -> E - T]
    }

_T:
    T();

    /*2 notacao semantica*/
    if (ominus) {  // Nega valor do acumulador
        //printf("\tneg acc\n");
        acc = -acc;
        result = acc;
        //printf("\tresult %lg\n", result);
        ominus = 0;  // Reseta a variável
    }
    /*2*/

    /*3 notacao semantica*/
    if (oplus) {  // Realiza a adição ou subtração entre valores da pilha e acumulador
        switch(oplus) {
            case '+':
                //printf("\tadd stack[sp], acc\n");
                //printf("\tpop\n");
                acc = compute('+', stack[sp], acc);  // Computa
                pop();  // Desempilha
                break;
            case '-':
                //printf("\tsub acc, stack[sp]\n");
                //printf("\tpop\n");
                acc = compute('-', stack[sp], acc);  // Computa
                pop();  // Desempilha
                break;
            default:  // Não faz nada
                ;
        }
        
        oplus = 0;  // Reseta a variável
    }
    /*3*/

    //looping responsavel por continuar a verificacao se tem simbolo
    if (lookahead == '+' || lookahead == '-') {  // Token é símbolo de adição ou subtração
        /*4 notacao semantica*/
        oplus = lookahead;  // Variável recebe símbolo de adição ou subtração
        //printf("\tpush acc\n");
        push(acc);  // Empilha
        /*4*/

        match(lookahead); //identifica simbolo
        goto _T; //processa e faz o calculo
    }
}

// T -> F { otimes F }
void T(void) {  // Termo
    /*0 notacao semantica*/int otimes = 0;/*0*/
    
_F:
    F();

    /*1 notacao semantica*/
    if (otimes) {  // Realiza a multiplicação ou divisão entre valores da pilha e acumulador
        switch(otimes) {
            case '*':
                //printf("\tmul stack[sp], acc\n");
                //printf("\tpop\n");
                acc = compute('*', stack[sp], acc);  // Computa
                pop();  // Desempilha
                break;
            case '/':
                //printf("\tdiv stack[sp], acc\n");
                //printf("\tpop\n");
                acc = compute('/', stack[sp], acc);  // Computa
                pop();  // Desempilha
                break;
            default:  // Não faz nada
                ;
        }

        otimes = 0;  // Reseta a variável
    }
    /*1*/

    if (lookahead == '*' || lookahead == '/') {  // Token é símbolo de multiplicação ou divisão
        /*2 notacao semantica*/
        otimes = lookahead;  // Variável recebe símbolo de multiplicação ou divisão
        //printf("\tpush acc\n");
        push(acc);  // Empilha
        /*2*/

        match(lookahead);
        goto _F;
    }
}

// F -> ( E ) | ID | OCT | HEX | DEC 
void F(void) {  // Fator
    /*0 notacao semantica*/
    char varname[MAXIDLEN+1];
    /*0*/

    switch(lookahead) {
        case '(':
            match('(');
            E();
            match(')');
            break;
        case ID:
            /*1 notacao semantica*/
            strcpy(varname, lexeme);
            /*1*/
            match(ID);
            
            if (lookahead == ASGN) {
                // L-value
                match(ASGN);
                E();
                /*2 notacao semantica*/
                result = 0;  // Zera result para impressão de ASGN
                //printf("\tstore acc, %s\n", varname);
                store(varname, acc);  // Armazena
                /*2*/
            }
            else {
                // R-value
                /*3 notacao semantica*/
                //printf("\trecall acc, %s\n", varname);
                acc = recall(varname);  // Recupera
                /*3*/
            }
            break;
        default:
            /*4 notacao semantica*/
            //printf("\tmov %s, acc\n", lexeme);
            acc = atof(lexeme);  // String para float
            /*4*/
            match(NUM);
    }
}

void match(int expected) {
    if (lookahead == expected) {  // Token reconhecido
        lookahead = gettoken(source);
    }
    else {  // Token não reconhecido
        /*0 notacao semantica*/
        fprintf(stderr, "token mismatch\n");
        exit(-3);
        /*0*/
    }
}
