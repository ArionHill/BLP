/**
 * 使用environ变量打印环境变量.
 */ 

#include <stdlib.h>
#include <stdio.h>

/**
 * 可以通过environ变量直接访问程序的环境变量.
 */ 
extern char **environ;

int main()
{
    char **env = environ;
    printf("%ld\n", environ);
    while(*env) {
        printf("%s\n", *env);
        env ++;
    }
    exit(0);
}