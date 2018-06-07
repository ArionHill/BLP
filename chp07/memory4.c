#include <stdlib.h>
#define ONE_K (1024)


int main(int argc, char const *argv[])
{
    char *some_memory;
    char *scan_ptr;

    some_memory = (char *)malloc(ONE_K);
    if (some_memory == NULL) exit(EXIT_FAILURE);
    // some_memory = "abc";

    scan_ptr = some_memory;
    while (1) {
        *scan_ptr = '\0';
        scan_ptr ++;
    }
    exit(EXIT_SUCCESS);
}
