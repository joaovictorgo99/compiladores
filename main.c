#include <stdio.h>
#include <parser.h>
#include <lexer.h>

FILE *source;

int main()
{
    source = stdin;
    lookahead = gettoken(source);
    E();

    return 0;
}