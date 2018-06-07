#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    char c;
    int in, out;
    in = open("bash", O_RDONLY);
    out = open("file.out", O_WRONLY | O_CREAT);
    while (read(in, &c, 1) == 1)
        write(out, &c, 1);
    
    exit(0);
}
