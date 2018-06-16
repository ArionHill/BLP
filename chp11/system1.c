#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    printf("Running ps with system\n");
    system("ps ax &");
    printf("Done.\n");
    return 0;
}
