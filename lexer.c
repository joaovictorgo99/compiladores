#include <stdio.h>
#include <main.h>
#include <ctype.h>
#include <lexer.h>

int linenum = 1;
int i = 0;
char lexeme[MAXIDLEN+1];

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
    //int i = 0;
    lexeme[i] = getc(tape);

    if (isalpha(lexeme[i])) {
        i++;
        
        while (isalnum(lexeme[i] = getc(tape))) {
            i++;
        }

        ungetc(lexeme[i], tape);
        lexeme[i] = 0;

        //printf("%s\n", lexeme);
        return ID;
    }

    ungetc(lexeme[i], tape);

    return 0;
}

// DEC = [1-9][0-9]* | 0
int isDEC(FILE *tape) {
    //int i = 0;
    lexeme[i] = getc(tape);

    if (lexeme[i] == '\n') {
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;

        //printf("%s\n", lexeme);
        return DEC;
    }

    if (isdigit(lexeme[i])) {
        i++;

        while (isdigit(lexeme[i] = getc(tape))) {
            i++;
        }
        
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        
        //printf("%s\n", lexeme);
        return DEC;
    }

    ungetc(lexeme[i], tape);

    return 0;
}

// OCT = 0[0-7]+
int isOCT(FILE *tape) {
    //int i = 0;
    lexeme[i] = getc(tape);

    if (lexeme[i] == '0') {
        i++;
        lexeme[i] = getc(tape);

        if(lexeme[i] == '\n') {
            ungetc(lexeme[i], tape);

            return 0;
        }

        do {
            if (lexeme[i] < '0' || lexeme[i] > '7') {            
                ungetc(lexeme[i], tape);

                return 0;
            }

            i++;
        } while (isdigit(lexeme[i] = getc(tape)));

        ungetc(lexeme[i], tape);
        lexeme[i] = 0;

        //printf("%s\n", lexeme);
        return OCT;
    }
    
    ungetc(lexeme[i], tape);

    return 0;
}

// HEX = 0[xX][0-9a-fA-F]+
int isHEX(FILE *tape) {
    //int i = 1;
    lexeme[i] = getc(tape);

    if (lexeme[i] == '\n') {
        ungetc(lexeme[i], tape);

        return 0;
    }

    if (toupper(lexeme[i]) == 'X') {
        i++;

        while (isalnum(lexeme[i] = getc(tape))) {
            if ((lexeme[i] < '0' || lexeme[i] > '9') && (toupper(lexeme[i]) < 'A' || toupper(lexeme[i]) > 'F')) {
                ungetc(lexeme[i], tape);

                return 0;
            }

            i++;
        }

        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        
        //printf("%s\n", lexeme);
        return HEX;
    }
    
    ungetc(lexeme[i], tape);

    return 0;
}

int gettoken(FILE *source) {
    int token;

    i = 0;
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
