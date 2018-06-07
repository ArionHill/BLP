#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int opt;

    /**
     * int getopt(int argc, char *const argv[], const char *optstring)
     * extern char *optarg;
     * extern int optind, opterr, optopt;
     * 1. 如果选项有一个关联值, 则外部变量optarg指向这个值.
     * 2. 如果选项处理完毕, getopt返回-1, 特殊参数--将使getopt停止扫描.
     * 3. 如果遇到一个无法识别的选项, getopt返回一个问号(?), 并把它保存到外部变量optopt中.
     * 4. 如果一个选项要求有一个关联值(如下-f), 但用户并未提供这个值, getopt通常将返回一个问
     *    号(?). 如果我们将选项字符串的第一个设置为冒号(:), 那么getopt将在用户未提供值的情况
     *    下返回冒号(:)而不是问号(?)
     * 
     * optind被设置为下一个待处理参数的索引. getopt利用它来记录自己的进度. 程序很少需要对这个
     * 变量进行设置. 当所有选项参数都处理完毕后, optind将指向argv数组尾部可以找到其余参数的位置.
     */ 
    while((opt = getopt(argc, argv, ":if:lr")) != -1) {
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
    for (; optind < argc; optind ++) {        
        printf("argument: %s\n", argv[optind]);
        printf("optind: %d\n", optind);
    }
       
    exit(0);
}