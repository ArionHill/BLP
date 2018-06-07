#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char tmpname[L_tmpnam];
    char *filename;
    FILE *tmpfp;
    
    /**
     * 当编译一个使用tmpnam函数的程序时, GNU C编译器会对它的使用给出警告信息. 如下
     * warning: the use of `tmpnam' is dangerous, better use `mkstemp'
     * 
     * UNIX 另外两种生成临时文件名的方式:
     * #include <stdlib.h>
     * char *mktemp(char *template);
     * int mkstemp(char *template);
     * template必须是一个以6个X结尾的字符串.
     * mktemp成功返回一个指向生成的字符串的指针.
     * 失败返回一个空指针.
     * 
     * mkstemp类似与tmpfile,创建的同时打开一个临时文件.返回一个打开的底层文件描述符.
     */
    filename = tmpnam(tmpname);

    printf("Temporary file name is: %s\n", filename);
    tmpfp = tmpfile();
    if (tmpfp)
        printf("Open a temporary file OK\n");
    else
        perror("tmpfile");
    return 0;
}
