#include <stdio.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    char *filename;
    if (argc != 2) {
        fprintf(stderr, "usage: useupper file\n");
        exit(1);
    } 
    filename = argv[1];

    if (!freopen(filename, "r", stdin)) {
        printf(stderr, "could not redirect stdin from file %s\n", filename);
        exit(2);
    }
    execl("./upper", "upper", 0);
    
    perror("cound not exec ./upper");
    return 0;
}
