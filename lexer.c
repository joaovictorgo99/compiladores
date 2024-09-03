#include <stdio.h>
#include <main.h>
#include <ctype.h>
#include <lexer.h>

int linenum = 1;

// abc123 := a + b
void skipspaces(FILE *tape) {
    int head;

    while (isspace(head = getc(tape))) {
        if (head == '\n')
            linenum++;
    };

    //printf("linenum: %d\n", linenum);

    ungetc(head, tape);
}

// ID = [A-Za-z][A-Za-z0-9]*
int isID(FILE *tape) {
    int head = getc(tape);

    if (isalpha(head)) {
        while (isalnum(head = getc(tape)));
        ungetc(head, tape);

        return ID;
    }

    ungetc(head, tape);
    return 0;
}

// DEC = [1-9][0-9]* | 0
int isDEC(FILE *tape) {
    int head = getc(tape);

    if (head == '\n') {
        ungetc(head, tape);

        return DEC;
    }

    if (isdigit(head)) {
        if (head == '0') {
            return DEC;
        }

        while (isdigit(head = getc(tape)));
        
        ungetc(head, tape);
        
        return DEC;
    }

    ungetc(head, tape);

    return 0;
}

// OCT = 0[0-7]+
int isOCT(FILE *tape) {
    int head = getc(tape);

    if (head == '0') {
        if((head = getc(tape)) == '\n') {
            ungetc(head, tape);

            return 0;
        }

        do {
            if (head < '0' || head > '7') {            
                ungetc(head, tape);

                return 0;
            }
        } while (isdigit(head = getc(tape)));

        ungetc(head, tape);

        return OCT;
    }
    
    ungetc(head, tape);

    return 0;
}

// HEX = 0[xX][0-9a-fA-F]+
int isHEX(FILE *tape) {
    int head = getc(tape);

    if (head == '\n') {
        ungetc(head, tape);

        return 0;
    }

    if (toupper(head) == 'X') {
        while (isalnum(head = getc(tape))) {
            if ((head < '0' || head > '9') && (toupper(head) < 'A' || toupper(head) > 'F')) {
                ungetc(head, tape);

                return 0;
            }
        }

        ungetc(head, tape);

        return HEX;
    }
    
    ungetc(head, tape);

    return 0;
}

int gettoken(FILE *source) {
    int token;

    skipspaces(source);
    
    if ((token = isID(source))) {
        //printf("ID\n");
        return ID;
    }
    else if ((token = isOCT(source))){
        //printf("OCT\n");
        return OCT;
    }
    else if ((token = isHEX(source))) {
        //printf("HEX\n");
        return HEX;
    }
    else if ((token = isDEC(source))) {
        //printf("DEC\n");
        return DEC;
    }

    token = getc(source);

    return token;
}
