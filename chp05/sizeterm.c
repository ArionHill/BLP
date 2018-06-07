#include <stdio.h>
#include <term.h>
#include <curses.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int nrows, ncolumns;

    setupterm(NULL, fileno(stdout), (int *)0);
    nrows = tigetnum("lines");
    ncolumns = tigetnum("cols");
    printf("This terminal has %d columns and %d rows\n", ncolumns, nrows);
    return 0;
}
