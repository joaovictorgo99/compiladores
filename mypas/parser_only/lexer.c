#include <stdio.h>
#include <ctype.h>
#include <mypas/parser_only/main.h>
#include <mypas/parser_only/lexer.h>
#include <mypas/parser_only/keywords.h>

double lexval;
int linenum = 1;
int i = 0;
char lexeme[MAXIDLEN+1];

// abc123 := a + b
void skipspaces_and_comments(FILE *tape) {
    int head;

    if ((head = getc(tape)) == '{') {
        while ((head = getc(tape)) != '}') {
            if (head == '\n')
                linenum++;  // Contabiliza o número de linhas
        }
    }
    else {
        ungetc(head, tape);
    }

    if (isspace(head = getc(tape))) {
        while (isspace(head = getc(tape))) {  // Ignora caracteres de espaço, "consumindo-o"
            if (head == '\n')
                linenum++;  // Contabiliza o número de linhas
        }
    }

    //printf("linenum: %d\n", linenum);

    ungetc(head, tape);
}

// ID = [A-Za-z][A-Za-z0-9]*
int isID(FILE *tape) {
    int keyword;
    lexeme[i] = getc(tape);

    if (isalpha(lexeme[i])) {  // É ID
        i++;
        
        while (isalnum(lexeme[i] = getc(tape))) {  // Itera enquanto caractere pertencer ao formato ID
            i++;

            if (i > MAXIDLEN) {
                printf("MAXIDLEN");
                break;
            }
        }

        // Transição epsilon para estado ID
        ungetc(lexeme[i], tape);  // Caractere não pertence ao formato ID, logo caractere é devolvido ao buffer
        lexeme[i] = 0;
        //printf("%s\n", lexeme);

        if ((keyword = iskeyword(lexeme))) {
            return keyword;
        }

        return ID;
    }

    // Transição epsilon para estado zero
    ungetc(lexeme[i], tape);  // Não é ID, logo caractere é devolvido ao buffer
    return 0;
}

// NUM = [0-9]+
int isNUM(FILE *tape) {
    char tape_copy[MAXIDLEN+1];
    int index = 0;
    int flag = 0;

    fscanf(tape, "%s", &tape_copy);

    if (isdigit(tape_copy[index])) {  // É NUM
        while (index < MAXIDLEN) {
            if (tape_copy[index] == '.') {
                flag = 1;
            }
            
            index++;
        }

        fscanf(tape, "%ld", &lexval);
        sprintf(lexeme, "%ld", lexval);

        if (flag == 0) {
            if (atoi(tape_copy) <= 65535) {
                // Transição epsilon para estado INTEGER
                return UINT32;
            }
            else {
                // Transição epsilon para estado LONG
                return UINT64;
            }
        }
        else {
            if (atof(tape_copy) <= 3.4E+38) {
                // Transição epsilon para estado REAL
                return FLOAT32;
            }
            else {
                // Transição epsilon para estado DOUBLE
                return FLOAT64;
            }
        }
    }

    // Transição epsilon para estado zero
    return 0;
}

int isRELOP(FILE *tape) {
	lexeme[2] = 0;

	switch (lexeme[0] = getc(tape)) {
		case '<':
			if ((lexeme[1] = getc(tape)) == '=') {
				return LEQ;
			}

			if (lexeme[1] == '>') {
				return NEQ;
			}

			ungetc(lexeme[1], tape);
			lexeme[1] = 0;
			return lexeme[0];
		case '>':
			if ((lexeme[1] = getc(tape)) == '=') {
				return GEQ;
			}

			ungetc(lexeme[1], tape);
			lexeme[1] = 0;
			return lexeme[0];
		default:  // Não faz nada
			;
	}

	ungetc(lexeme[0], tape);
	lexeme[1] = 0;
	return 0;
}

// ASGN = [:=]
int isASGN(FILE *tape) {
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
    //int i = 0;
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
    //int i = 0;
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
    //int i = 1;
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

    skipspaces(source);  // Ignora os caracteres de espaço da entrada
    
    if ((token = isID(source))) {
        //printf("ID\n");
        return ID;
    }
    
    if ((token = isNUM(source))){
        //printf("NUM\n");
        return token;
    }
    
    if ((token = isASGN(source))) {
        //printf("ASGN\n");
        return ASGN;
    }

    if ((token = isRELOP(source))) {
        //printf("RELOP\n");
        return token;
    }

    token = getc(source);

    return token;
}