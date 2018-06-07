#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    struct tm *tm_ptr;
    time_t the_time;

    /**
     * #include <time.h>
     * time_t time(time_t *tloc);
     * time返回的是从纪元开始至今的秒数. 如果tloc不是一个空指针,
     * time函数还会把返回值写入tloc指针指向的位置.
     *  
     * #include <time.h>
     * struct tm *gmtime(const time_t timeval)
     */
    (void) time(&the_time);
    tm_ptr = gmtime(&the_time);
    printf("Raw time is %ld\n", the_time);
    printf("gmtime gives:\n");
    printf("date: %02d:%02d:%02d\n",
        tm_ptr->tm_year, tm_ptr->tm_mon + 1, tm_ptr->tm_mday);
    printf("time: %02d:%02d:%02d\n",
        tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
    printf("asctime: %s\n\n", asctime(tm_ptr));
    printf("ctime: %s\n\n", ctime(&the_time));
    /**
     * #include <time.h>
     * time_t mktime(struct tm *timeptr);
     * 把已分解的tm结构再转换为原始的time_t时间值, 使用mktime函数.
     * 如果tm结构不能被表示为time_t值, mktime返回-1.
****** 注意: mktime在转换为time_t时间时会将timeptr转换为本地时间的time_t(local_time)
     */
    printf("gmt_to_tm: %ld\n\n", mktime(tm_ptr));

    /**
     * #include <time.h>
     * struct tm *localtime(const time_t *timeval)
     */
    tm_ptr = localtime(&the_time);
    printf("Raw time is %ld\n", the_time);
    printf("gmtime gives:\n");
    printf("date: %02d:%02d:%02d\n",
        tm_ptr->tm_year, tm_ptr->tm_mon + 1, tm_ptr->tm_mday);
    printf("time: %02d:%02d:%02d\n",
        tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
    printf("asctime: %s\n\n", asctime(tm_ptr));
    printf("ctime: %s\n\n", ctime(&the_time));
    printf("gmt_to_tm: %ld\n\n", mktime(tm_ptr));

    return 0;
}
