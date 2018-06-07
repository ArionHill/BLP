#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    time_t timeval;
    /**
     * #include <time.h>
     * time_t time(time_t *tloc);
     * time返回的是从纪元开始至今的秒数. 如果tloc不是一个空指针,
     * time函数还会把返回值写入tloc指针指向的位置.
     */
    (void) time(&timeval);
    printf("The date is: %s", ctime(&timeval));

    /**
     * #include <time.h>
     * struct tm *localtime(const time_t *timeval)
     */

    return 0;
}
