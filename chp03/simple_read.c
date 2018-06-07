#include <unistd.h>
#include <stdlib.h>

int main()
{
    char buf[255];
    int nread;
    nread = read(0, buf, 255);
    if (nread == -1)
        write(2, "read error\n", 5);
    
    if (write(1, buf, nread) != nread) {
        write(2, "write error\n", 5);
    }
    exit(0);
}
