#include <stdio.h>
#include <parser.h>
#include <lexer.h>

FILE *source;

int main()
{
    do {  // Loop para entrada
        source = stdin;
        lookahead = gettoken(source);
        E();
    } while (lookahead != EOF);

    return 0;
}