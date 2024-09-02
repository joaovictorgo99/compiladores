#include <stdio.h>
#include <main.h>
#include <ctype.h>
#include <lexer.h>

// abc123 := a + b
void skipspaces(FILE *tape) {
    int head;
    while(isspace(head = getc(tape)));
    ungetc(head, tape);
}

// DEC = [1-9][0-9]* | 0
int isDEC(FILE *tape) {
    int head = getc(tape);

    if(isdigit(head)) {
        if(head == '0') {
            return DEC;
        }
        while(isdigit(head = getc(tape)));
        ungetc(head, tape);

        return DEC;
    }

    ungetc(head, tape);

    return 0;
}

// ID = [A-Za-z][A-Za-z0-9]*
int isID(FILE *tape) {
    int head = getc(tape);

    if(isalpha(head)) {
        // is ID
        while(isalnum(head = getc(tape)));
        // epsilon transition into ID-state
        ungetc(head, tape);
        return ID;
    }

    // epsilon transition into zero-state
    ungetc(head, tape);
    return 0;
}

int gettoken(FILE *source) {
    int token;

    skipspaces(source);

    if((token = isID(source))) 
        return ID;
    else if((token = isDEC(source))) 
        return DEC;

    token = getc(source);

    return token;
}