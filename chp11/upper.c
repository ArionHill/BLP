#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
    int ch;
    while ((ch = getchar()) != EOF) {
        putchar(toupper(ch));
    }
    exit(0);
}
