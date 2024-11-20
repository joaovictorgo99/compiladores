#include <stdio.h>
#include <mybc.h>
#include <parser.h>

FILE *source;

void cmd(void) {
    source = stdin;  // Entrada do usuário
    lookahead = gettoken(source);

    switch (lookahead) {
        case ';':
        case '\n':
        case EOF:
            break;  // Não faz nada
        default:
            E();
            printf("\t%lg\n", result);
    }
}

void mybc(void) {
    cmd();

    while (lookahead == ';' || lookahead == '\n') {
        cmd();
    }
}