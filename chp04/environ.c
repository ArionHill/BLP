#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char *var, *value;

    if (argc == 1 || argc > 3) {
        fprintf(stderr, "usage: environ var [value] \n");
        exit(1);
    }

    /**
     * #include <stdlib.h>
     * char *getenv(const char *name);
     * 环境由一组格式为"名字=值"的字符串组成. getenv函数以给定的名字搜索环境变量中的一个字符串,
     * 并返回与该名字相关的值. 如果变量不存在, 就返回null. 如果存在但无关联值,它将运行成功并返回
     * 一个空字符串, 即该字符串的第一个字节是null. 
     * 
****** getenv返回的字符串存储在getenv提供的静态空间中, 如果想进一步使用它, 就必须将它复制到另一
****** 个字符串中, 以免被后续的getenv调用所覆盖.
     */ 
    var = argv[1];
    value = getenv(var);
    char *path;
    path = getenv("PATH");
    printf("%s\n", path);
    printf("%ld %ld\n", path, value);
    if (value)
        printf("Variable %s has value %s\n", var, value);
    else
        printf("Variable %s has no value\n", var);

    if (argc == 3) {
        char *string;
        value = argv[2];
        string = malloc(strlen(var) + strlen(value) + 2);
        if (!string) {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }

        /**
         * #include <stdlib.h>
         * int putenv(const char *string);
         * string 是格式为"名字=值"的字符串. putenv将该字符串加到当前环境中. 可用内存不足而不能
         * 扩展环境时, 它会失败并返回-1并设置变量errno为ENOMEN.
         */ 
        strcpy(string, var);
        strcat(string, "=");
        strcat(string, value);
        printf("Calling putenv with: %s\n", string);
        if (putenv(string) != 0) {
            fprintf(stderr, "putenv failed\n");
            free(string);
            exit(1);
        }
        value = getenv(var);
        if (value)
            printf("New value of %s is %s\n", var, value);
        else
            printf("New value of %s is null??\n", var);
    }
    return 0;
}
