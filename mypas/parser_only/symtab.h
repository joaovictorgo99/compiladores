#include <stdio.h>
#include <lexer.h>

#pragma once

#define SYMTAB_SIZE 0x100000
#define OFFSET_SIZE 80

typedef struct _symtab_ {
    char name[MAXIDLEN];
    int objtype; // == 0 se variable; == 1 se parameter; == 2 se function; == 3 se procedure
    int type; /*
                BYTE, // boolean
                UINT32,  // integer
                UINT64,  // long
                FLOAT32,  // real
                FLOAT64,  // double
              */
    int lexlevel;
    char offset[OFFSET_SIZE];
} SYMTAB;

extern SYMTAB symtab[];
extern int symtab_next_entry;
int symtab_lookup(char const *name, int lexlevel);
int symtab_append(char const *name, int lexlevel);

