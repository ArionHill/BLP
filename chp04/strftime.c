#define _XOPEN_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    struct tm *tm_ptr, timestruct;
    time_t the_time;
    char buf[256];
    char *result;

    /**
     * #include <time.h>
     * size_t strftime(char *s, size_t maxsize, const char *format, struct tm *timeptr);
     * strftime函数格式化timeptr指针指向的tm结构所表示的时间和日期, 并将结果放在字符串s中.
     * 字符串被指定(至少)maxsize个字符长. format字符串用于控制写入字符串s的字符. 转换控制
     * 符如下:
     * %a %A: 星期几缩写与全称 
     * %b %B: 月份缩写与全称
     * %c: 日期和时间
     * %d: 月份中的日期, 01~31
     * %H: 小时, 00~23
     * %I: 12小时制中的小时, 01~12
     * %j: 年份中的日期, 001~366
     * %m: 年份中的月份, 01~12
     * %M: 分钟, 00~59
     * %p: a.m. 或 p.m.
     * %S: 秒, 00~61
     * %u: 星期几, 1~7(周一为1)
     * %U: 一年中的第几周, 01~53(周日是一周的第一天)
     * %V: 一年中的第几周, 01~53(周一是一周的第一天)
     * %w: 星期几, 0~6(周日为0)
     * %x: 本地格式的日期
     * %X: 本地格式的时间
     * %y: 年份中的后两位
     * %Y: 年份
     * %Z: 时区名
     * %%: 字符%
     */
    (void) time(&the_time);
    tm_ptr = localtime(&the_time);
    strftime(buf, 256, "%A %d %B, %I:%S %p", tm_ptr);

    printf("strftime gives: %s\n", buf);



    char p[10], u[10], U[10], V[10], w[10], x[10], X[10], y[10], Y[10], Z[10], j[10];
    strftime(p, 10, "%p", tm_ptr);
    strftime(u, 10, "%u", tm_ptr);
    strftime(U, 10, "%U", tm_ptr);
    strftime(V, 10, "%V", tm_ptr);
    strftime(w, 10, "%w", tm_ptr);
    strftime(x, 10, "%x", tm_ptr);
    strftime(X, 10, "%X", tm_ptr);
    strftime(y, 10, "%y", tm_ptr);
    strftime(Y, 10, "%Y", tm_ptr);
    strftime(Z, 10, "%Z", tm_ptr);
    strftime(j, 10, "%j", tm_ptr);   
    printf("%%p: %s\n", p);
    printf("%%u: %s\n", u);
    printf("%%U: %s\n", U);
    printf("%%V: %s\n", V);
    printf("%%w: %s\n", w);
    printf("%%x: %s\n", x);
    printf("%%X: %s\n", X);
    printf("%%y: %s\n", y);
    printf("%%Y: %s\n", Y);
    printf("%%Z: %s\n", Z);
    printf("%%j: %s\n", j);


    strcpy(buf, "Thu 26 July 2007, 17:53 will do fine");

    printf("calling strtime with: %s\n", buf);
    tm_ptr = &timestruct;

    /**
     * #include <time.h>
     * char *strptime(const char *buf, const char *format, struct tm *timeptr);
     * strptime返回一个指针, 指向转换过程处理的最后一个字符后面的那个字符. 如果遇到不能转换的
     * 字符, 转换过程就在该处停下来.
     * 调用程序需要检查是否已从传递的字符串中读入了足够多的数据, 以确保tm结构中写入了有意义的值.
     */
    result = strptime(buf, "%a %d %b %Y, %R", tm_ptr);    
    printf("strptime consumed up to: %s\n", result);

    printf("strptime gives:\n");
    printf("date: %02d/%02d/%02d\n",
        tm_ptr->tm_year % 100, tm_ptr->tm_mon + 1, tm_ptr->tm_mday);
    printf("time: %02d:%02d\n",
        tm_ptr->tm_hour, tm_ptr->tm_min);

        

    struct tm tm;   
        
    strptime("24/Aug/2011:09:42:35", "%d/%b/%Y:%H:%M:%S" , &tm);  
    printf("asctime:%s\n",asctime(&tm));  
  
    memset(buf,0,sizeof(buf));  
    strftime(buf, sizeof(buf), "%d %b %Y %H:%M", &tm);  
    puts(buf);  

    return 0;
}
