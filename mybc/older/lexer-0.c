#include <stdio.h>
#include <main.h>
#include <ctype.h>
#include <lexer.h>

int linenum = 1;

// abc123 := a + b
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
    int head = getc(tape);

    if (isalpha(head)) {  // É ID
        while (isalnum(head = getc(tape)));  // Itera enquanto caractere pertencer ao formato ID

        // Transição epsilon para estado ID
        ungetc(head, tape);  // Caractere não pertence ao formato ID, logo caractere é devolvido ao buffer
        return ID;
    }

    // Transição epsilon para estado zero
    ungetc(head, tape);  // Não é ID, logo caractere é devolvido ao buffer
    return 0;
}

// DEC = [1-9][0-9]* | 0
int isDEC(FILE *tape) {
    int head = getc(tape);

    if (head == '\n') {  // É DEC, vindo de OCT ou HEX, zero
        // Transição epsilon para estado DEC
        ungetc(head, tape);  // Caractere não pertence ao formato DEC, logo caractere é devolvido ao buffer
        return DEC;
    }

    if (isdigit(head)) {  // É DEC
        if (head == '0') {  // É DEC, zero
            return DEC;
        }

        while (isdigit(head = getc(tape)));  // Itera enquanto caractere pertencer ao formato DEC
        
        // Transição epsilon para estado DEC
        ungetc(head, tape);  // Caractere não pertence ao formato DEC, logo caractere é devolvido ao buffer
        return DEC;
    }

    // Transição epsilon para estado zero
    ungetc(head, tape);  // Não é DEC, logo caractere é devolvido ao buffer
    return 0;
}

// OCT = 0[0-7]+
int isOCT(FILE *tape) {
    int head = getc(tape);

    if (head == '0') {
        if((head = getc(tape)) == '\n') {
            // Transição epsilon para estado zero
            ungetc(head, tape);  // Não é OCT, logo caractere é devolvido ao buffer
            return 0;
        }

        do {
            if (head < '0' || head > '7') {  // Caractere não pertence ao formato OCT
                // Transição epsilon para estado zero
                ungetc(head, tape);  // Não é OCT, logo caractere é devolvido ao buffer
                return 0;
            }
        } while (isdigit(head = getc(tape)));  // Itera enquanto caractere pertencer ao formato OCT

        // É OCT
        // Transição epsilon para estado OCT
        ungetc(head, tape);  // Caractere não pertence ao formato OCT, logo caractere é devolvido ao buffer
        return OCT;
    }
    
    // Transição epsilon para estado zero
    ungetc(head, tape);  // Não é OCT, logo caractere é devolvido ao buffer
    return 0;
}

// HEX = 0[xX][0-9a-fA-F]+
int isHEX(FILE *tape) {
    int head = getc(tape);

    if (head == '\n') {
        // Transição epsilon para estado zero
        ungetc(head, tape);  // Não é HEX, logo caractere é devolvido ao buffer
        return 0;
    }

    if (toupper(head) == 'X') {  
        while (isalnum(head = getc(tape))) {  // Itera enquanto caractere pertencer ao formato HEX
            if ((head < '0' || head > '9') && (toupper(head) < 'A' || toupper(head) > 'F')) {  // Caractere não pertence ao formato HEX
                // Transição epsilon para estado zero
                ungetc(head, tape);  // Não é HEX, logo caractere é devolvido ao buffer
                return 0;
            }
        }

        // É HEX
        // Transição epsilon para estado HEX
        ungetc(head, tape);  // Caractere não pertence ao formato HEX, logo caractere é devolvido ao buffer
        return HEX;
    }
    
    // Transição epsilon para estado zero
    ungetc(head, tape);  // Não é HEX, logo caractere é devolvido ao buffer
    return 0;
}

int gettoken(FILE *source) {  // Front-end do analisador léxico
    int token;

    skipspaces(source);  // Ignora os caracteres de espaço da entrada
    
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
