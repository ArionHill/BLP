/**
 * 利用mmap和数组凡是的存取操作来修改一个结构化数据文件
 */

#include <unistd.h>                              // 系统调用. 必须出现在首行, 因为它定义的与POSIX规范有关的标志可能影响其它头文件.
#include <stdio.h>
#include <sys/mman.h>                            //
#include <fcntl.h>                               // 系统调用.
#include <stdlib.h>                              //

typedef struct {
    int integer;
    char string[24];
} RECORD;

#define NRECORDS 100

int main()
{
    RECORD record, *mapped;
    int i, f;
    /**
     * "r" "rb": 只读.
     * "w" "wb": 只写, 文件长度截短为零.
     * "a" "ab": 追加.
     * "r+" "rb+" "r+b": 读写.
     * "w+" "wb+" "w+b": 读写. 文件长度截短为零.
     * "a+" "ab+" "a+b": 读写. 追加.
     */
    FILE *fp;                                    // FILE文件流, 文件流指针.
    fp = fopen("records.dat", "w+");             // FILE *fopen(const char *filename, const char *mode) 
    for (i = 0; i < NRECORDS; ++ i) {
        record.integer = i;
        sprintf(record.string, "RECORD-%d", i);  // sprint(char *s, const char *format, ...)
        fwrite(&record, sizeof(record), 1, fp);  // size_t fwrite(const void *ptr, size_t size, size_t nitems, FILE *stream)
    }
    fclose(fp);

    /**
     * int fseek(FILE *stream, long int offset, int whence)
     * whence取值:
     *     SEEK_SET: offset是绝对位置.
     *     SEEK_CUR: offset是相对于当前位置的一个相对位置.
     *     SEEK_END: offset是相对于文件尾的一个相对位置.
     */
    fp = fopen("records.dat", "r+");
    fseek(fp, 43 * sizeof(record), SEEK_SET);    
    fread(&record, sizeof(record), 1, fp);

    record.integer = 143;
    sprintf(record.string, "RECORD-%d", record.integer);

    fseek(fp, 43 * sizeof(record), SEEK_SET);
    fwrite(&record, sizeof(record), 1, fp);
    fclose(fp);

    /**
     * int open(const char *path, int oflags)
     * oflags必选参数: O_RDONLY 只读, O_WRONLY 只写, O_RDWR 读写
     *       可选参数: O_APPEND 追加, O_TRUNC 文件长度设为0, O_CREAT 如果需要就新建, 
     *           O_EXCL: 与 O_CREAT一起使用, 确保调用者创建出文件. open是一个原子操作,
     *                   即它只执行一个函数调用. 使用这个可选模式可以防止两个程序同时创建
     *                   一个文件. 如果文件已存在, open调用失败.
     * 
     * void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
     * 创建一个指向一段内存区域的指针, 该内存区域与可以通过一个打开的文件描述符访问的文件内容相关联.
     * 这通过一个使用带特殊权限的虚拟内存段来实现. 对这类虚拟内存段的读写会使操作系统去读写磁盘文件中
     * 与之对应的部分.
     * 参数:
     *     off: 共享内存段访问的文件中数据的起始偏移位置.
     *     fildes: 文件描述符.
     *     addr: 请求使用某个特定内存地址. 推荐取0, 这样结果指针将自动分配, 可以在不同系统, 可用地
     *           址范围不一样时, 保持程序的可移植性.
     *     prot: 内存段访问权限. 下列常熟按位OR的结果.
     *         PROT_READ: 读
     *         PROT_WRITE: 写
     *         PROT_EXEC: 执行
     *         PROT_NONE: 禁止访问
     *     flags: 对内存段的改变产生的影响.
     *         MAP_PRIVATE: 私有内存段, 修改只对本进程有效.
     *         MAP_SHARED: 内存段修改保存到磁盘文件中.
     *         MAP_FIXED: 该内存段必须位于addr指定的地址处
     */ 
    f = open("records.dat", O_RDWR);
    mapped = (RECORD *)mmap(0, NRECORDS * sizeof(record),
                          PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    
    mapped[43].integer = 243;
    sprintf(mapped[43].string, "RECORD-%d", mapped[43].integer);

    /**
     * int msync(void *addr, size_t len, int flags)
     * 把在该内存段的某个部分或整段中的修改写回到被映射的文件中(或者从被映射文件中读出).
     * addr, len确定内存段中需要修改的部分. flags控制执行修改的具体方式.
     * flags可选项:
     *     MS_ASYNC: 异步写方式
     *     MS_SYNC: 同步写方式
     *     MS_INVALIDATE: 从文件中读回数据
     * 
     * int munmap(void *addr, size_t len)
     * 释放内存段.
     */ 
    msync((void *)mapped, NRECORDS * sizeof(record), MS_ASYNC);
    for (i = 0; i < NRECORDS; ++ i) {
        printf("%d\n", mapped[i].integer);
        printf("%s\n", mapped[i].string);
        fprintf(stdout, "%d\n", mapped[i].integer);
        fprintf(stdout, "%s\n", mapped[i].string);
    }
    munmap((void *)mapped, NRECORDS * sizeof(record));
    close(f);

    exit(0);
}