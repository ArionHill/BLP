#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

int main(int argc, char *argv[])
{
    int opt;
    /**
     * 长选项数组由一些类型为struct option的结构组成, 每个结构描述了一个长选项的行为. 该数组必须以一个包含全0的结构结尾.
     * 长选项结构在头文件getopt.h中定义, 该头文件必须与常量_GNU_SOURCE一同包含进来, 该常量启用getopt_long功能.
     * struct option {
     *     const char *name;                     // 长选项的名字. 缩写也可以接受, 只要不与其它选项混淆.
     *     int has_arg;                          // 该选项是否带参数. 0不带, 1带, 2有一个可选参数
     *     int *flag;                            // 为NULL时, getopt_long返回在成员val里给出的值. 否则,
     *                                           // getopt_long返回0, 并将val的值写入flag指向的变量.
     *     int val;                              // getopt_long为该选项返回的值.
     * };
     */ 
    struct option longopts[] = {
        {"initialize", 0, NULL, 'i'},
        {"file", 1, NULL, 'f'},
        {"list", 0, NULL, 'l'},
        {"restart", 0, NULL, 'r'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, ":if:lr", longopts, NULL)) != -1) {
        switch(opt) {
            case 'i':
            case 'l':
            case 'r':
                printf("option: %c\n", opt);
                printf("optind: %d\n", optind);
                break;
            case 'f':
                printf("filename: %s\n", optarg);
                printf("optind: %d\n", optind);
                break;
            case ':':
                printf("option needs a value\n");
                printf("optind: %d\n", optind);
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                printf("optind: %d\n", optind);
                break;
            default: printf("option0: %c\n", opt);
        }
    }
    for(; optind < argc; optind ++)
        printf("argument: %s\n", argv[optind]);
    return 0;
}
