#include <stdio.h>
#include <ctype.h>
#include <main.h>
#include <lexer.h>

double lexval;
int linenum = 1;
int i = 0;
char lexeme[MAXIDLEN+1];

// a op b; = aopb;
void skipspaces(FILE *tape) {
    int head;

    while (isspace(head = getc(tape))) {  // Ignora caracteres de espaço, "consumindo-o"
        if (head == '\n')
            linenum++;  // Contabiliza o número de linhas
    };

    //printf("linenum: %d\n", linenum);

    ungetc(head, tape);
}

// ID = [A-Za-z][A-Za-z0-9]*
int isID(FILE *tape) {
    i = 0;
    lexeme[i] = getc(tape);

    if (isalpha(lexeme[i])) {  // É ID
        i++;
        
        while (isalnum(lexeme[i] = getc(tape))) {  // Itera enquanto caractere pertencer ao formato ID
            if (i < MAXIDLEN) {  // Verifica se comprimento de lexeme não é maior que MAXIDLEN
                i++;
            }
        }

        // Transição epsilon para estado ID
        ungetc(lexeme[i], tape);  // Caractere não pertence ao formato ID, logo caractere é devolvido ao buffer
        lexeme[i] = 0;
        //printf("%s\n", lexeme);
        return ID;
    }

    // Transição epsilon para estado zero
    ungetc(lexeme[i], tape);  // Não é ID, logo caractere é devolvido ao buffer
    return 0;
}

// NUM = [0-9]+
int isNUM(FILE *tape) {
    i = 0;
    int aux = getc(tape);

    if (isdigit(aux)) {  // É NUM
        ungetc(aux, tape);
        fscanf(tape, "%lg", &lexval);
        sprintf(lexeme, "%lg", lexval);

        // Transição epsilon para estado NUM
        return NUM;
    }

    // Transição epsilon para estado zero
    ungetc(aux, tape);  // Não é NUM, logo caractere é devolvido ao buffer
    return 0;
}

// ASGN = [:=]
int isASGN(FILE *tape) {
    i = 0;
    if ((lexeme[0] = getc(tape)) == ':') {
        if ((lexeme[1] = getc(tape)) == '=') {  // É ASGN
            lexeme[2] = 0;

            // Transição epsilon para estado ASGN
            return ASGN;
        }

        ungetc(lexeme[1], tape);  // Não é ASGN, logo caractere é devolvido ao buffer
    } 

    // Transição epsilon para estado zero
    ungetc(lexeme[0], tape);  // Não é ASGN, logo caractere é devolvido ao buffer
    lexeme[0] = 0;
    return 0;
}

/*
// DEC = [1-9][0-9]* | 0
int isDEC(FILE *tape) {
    int i = 0;
    lexeme[i] = getc(tape);

    if (lexeme[i] == '\n') {  // É DEC, vindo de OCT ou HEX, zero
        // Transição epsilon para estado DEC
        ungetc(lexeme[i], tape);  // Caractere não pertence ao formato DEC, logo caractere é devolvido ao buffer
        lexeme[i] = 0;
        //printf("%s\n", lexeme);
        return DEC;
    }

    if (isdigit(lexeme[i])) {  // É DEC
        i++;

        while (isdigit(lexeme[i] = getc(tape))) {  // Itera enquanto caractere pertencer ao formato DEC
            i++;
        }
        
        // Transição epsilon para estado DEC
        ungetc(lexeme[i], tape);  // Caractere não pertence ao formato DEC, logo caractere é devolvido ao buffer
        lexeme[i] = 0;
        //printf("%s\n", lexeme);
        return DEC;
    }

    // Transição epsilon para estado zero
    ungetc(lexeme[i], tape);  // Não é DEC, logo caractere é devolvido ao buffer
    return 0;
}

// OCT = 0[0-7]+
int isOCT(FILE *tape) {
    int i = 0;
    lexeme[i] = getc(tape);

    if (lexeme[i] == '0') {
        i++;
        lexeme[i] = getc(tape);

        if(lexeme[i] == '\n') {
            // Transição epsilon para estado zero
            ungetc(lexeme[i], tape);  // Não é OCT, logo caractere é devolvido ao buffer
            return 0;
        }

        do {
            if (lexeme[i] < '0' || lexeme[i] > '7') {  // Caractere não pertence ao formato OCT         
                // Transição epsilon para estado zero
                ungetc(lexeme[i], tape);  // Não é OCT, logo caractere é devolvido ao buffer
                return 0;
            }

            i++;
        } while (isdigit(lexeme[i] = getc(tape)));  // Itera enquanto caractere pertencer ao formato OCT

        // É OCT
        // Transição epsilon para estado OCT
        ungetc(lexeme[i], tape);  // Caractere não pertence ao formato OCT, logo caractere é devolvido ao buffer
        lexeme[i] = 0;
        //printf("%s\n", lexeme);
        return OCT;
    }
    
    // Transição epsilon para estado zero
    ungetc(lexeme[i], tape);  // Não é OCT, logo caractere é devolvido ao buffer
    return 0;
}

// HEX = 0[xX][0-9a-fA-F]+
int isHEX(FILE *tape) {
    int i = 1;
    lexeme[i] = getc(tape);

    if (lexeme[i] == '\n') {
        // Transição epsilon para estado zero
        ungetc(lexeme[i], tape);  // Não é HEX, logo caractere é devolvido ao buffer
        return 0;
    }

    if (toupper(lexeme[i]) == 'X') {
        i++;

        while (isalnum(lexeme[i] = getc(tape))) {  // Itera enquanto caractere pertencer ao formato HEX
            if ((lexeme[i] < '0' || lexeme[i] > '9') && (toupper(lexeme[i]) < 'A' || toupper(lexeme[i]) > 'F')) {  // Caractere não pertence ao formato HEX
                // Transição epsilon para estado zero
                ungetc(lexeme[i], tape);  // Não é HEX, logo caractere é devolvido ao buffer
                return 0;
            }

            i++;
        }

        // É HEX
        // Transição epsilon para estado HEX
        ungetc(lexeme[i], tape);  // Caractere não pertence ao formato HEX, logo caractere é devolvido ao buffer
        lexeme[i] = 0;
        //printf("%s\n", lexeme);
        return HEX;
    }
    
    // Transição epsilon para estado zero
    ungetc(lexeme[i], tape);  // Não é HEX, logo caractere é devolvido ao buffer
    return 0;
}
*/

int gettoken(FILE *source) {  // Front-end do analisador léxico
    int token;
    i = 0;

    skipspaces(source);  // Ignora os caracteres de espaço da entrada
    
    if ((token = isID(source))) {
        //printf("ID\n");
        return ID;
    }
    
    if ((token = isNUM(source))){
        //printf("NUM\n");
        return NUM;
    }
    
    if ((token = isASGN(source))) {
        //printf("ASGN\n");
        return ASGN;
    }

    token = getc(source);

    return token;
}
