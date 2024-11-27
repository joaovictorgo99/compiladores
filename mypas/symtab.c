#include <stdio.h>
#include <string.h>
#include <symtab.h>

SYMTAB symtab[SYMTAB_SIZE];

int symtab_next_entry = 0;

int symtab_lookup(char const *name, int lexlevel) {  // Busca e retorna a posição do símbolo da tabela
    int i;

    //printf("\tlook %d\n", lexlevel);

    for (i = 0; i < symtab_next_entry; i++) {
        if (strcmp(name, symtab[i].name) == 0 && symtab[i].lexlevel == lexlevel) {  // Encontrou, retorna posição
            return i;
        }
    }

    // Não encontrou, retorna posição livre
    return i;
}

int symtab_append(char const *name, int lexlevel) {  // Salva símbolo na tabela de símbolos
    int i = symtab_lookup(name, lexlevel);

    //printf("\tappend %d\n", lexlevel);

    if (i > SYMTAB_SIZE) {  // Verifica se tabela de símbolos está cheia
        return -2;
    }

    if (i < symtab_next_entry) {  // Verifica se símbolo já existe no mesmo nível léxico
        return -3;
    }

    // Salva símbolo na posição livre
    strcpy(symtab[i].name, name);
    symtab[i].lexlevel = lexlevel;
    symtab_next_entry++;

    return 0;
}