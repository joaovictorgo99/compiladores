#include <stdio.h>
#include <mybc/parser.h>
#include <mybc/lexer.h>

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