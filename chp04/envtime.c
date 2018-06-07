#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int i;
    time_t the_time;

    for (i = 1; i <= 10; ++ i) {
        /**
         * #include <time.h>
         * time_t time(time_t *tloc);
         * time返回的是从纪元开始至今的秒数. 如果tloc不是一个空指针,
         * time函数还会把返回值写入tloc指针指向的位置.
         */
        the_time = time((time_t *)0);
        printf("The time is %ld\n", the_time);
        sleep(2);
        /**
         * #include <time.h>
         * double difftime(time_t time1, time_t time2);
         * 将time1 - time2的值作浮点数返回.
         */
        printf("%lf\n", difftime(time((time_t *)0), the_time));
    }
    exit(0);
}