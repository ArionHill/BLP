#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_function(void *arg);
sem_t bin_sem;

#define WORK_SIZE 1024
char work_area[WORK_SIZE];

int main(int argc, char const *argv[])
{
    int res;
    pthread_t a_thread;
    void *thread_result;

    /**
     * int sem_init(sem_t *sem, int pshared, unsigned in value);
     * 初始化sem指向的信号量对象,设置它的共享项,并给它一个初始的整数值.
     * pshared控制参数类型,如果值为0,则该信号量是当前进程的局部信号量,
     * 否则,该信号量可以在多个进程间共享.
     */
    res = sem_init(&bin_sem, 0, 0);                              // 设置信号量初始值为0
    if (res != 0) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    
    /**
     * int pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
     * 第一个参数为指向pthread_t类型数据的指针.线程被创建时,这个指针指向的变量中被写入一个标识符,我们用该标识符来引用新线程.
     * 下一个参数设置线程的属性.不需要特殊属性时设为NULL.
     * 最后两个参数分别告诉线程将要启动执行的函数和传递给该函数的参数.
     * void *(*start_routine)(void *)启动执行的函数以一个指向void的指针为参数,返回的也是一个指向void的指针.
     * 
     * 用fork调用后,父子进程将在同一位置继续执行下去,只是fork调用的返回值是不同的;
     * 但对新线程来说,必须明确地给它提供一个函数指针,新线程将在这个新位置开始执行.
     * 
     * 该函数调用成功时返回0,失败时返回错误代码.
     */
    res = pthread_create(&a_thread, NULL, thread_function, NULL); // 启动新线程
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Input some text. Enter 'end' to finish\n");
    while (strncmp("end", work_area, 3) != 0) {
        fgets(work_area, WORK_SIZE, stdin);                      // 从键盘读取一些文本
        /**
         * int sem_post(sem_t *sem);
         * 以原子操作的方式给信号量的值加1
         */
        sem_post(&bin_sem);                                      // 增加信号量的值.以原子操作的方式给信号量的值加1
    }

    printf("\nWaiting for thread to finish...\n");
    /**
     * int pthread_join(pthread th, void **thread_return);
     * 第一个参数指定要等待的线程,线程通过pthread_create返回的标识符来指定.
     * 第二个参数是一个指针,它指向另一个指针,而后者指向指向线程的返回值.
     * 与pthread_create类似,该函数成功时返回0,失败时返回错误代码.
     * 作用等价与进程中用来收集子进程信息的wait函数.
     */
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined, it returned %s\n", (char *)thread_result);
    /**
     * int sem_destroy(sem_t *sem);
     * 清理信号量所拥有的所有资源.如果清理的信号量正在被一些线程等待,就会收到一个错误.
     */
    sem_destroy(&bin_sem);
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
    /**
     * int sem_wait(sem_t *sem);
     * 以原子操作的方式将信号量减1,但它会等待直到信号量有个非零值才会开始减法操作.
     */
    sem_wait(&bin_sem);
    while (strncmp("end", work_area, 3) != 0) {
        printf("You input %lu characters\n", strlen(work_area) -1);
        sem_wait(&bin_sem);
    }
    /**
     * void pthread_exit(void *retval);
     * 终止调用它的线程并返回一个指向某个对象的指针.
     * 注意,绝不能用它来返回一个指向局部变量的指针,因为线程调用该函数后,这个局部变量就不再存在了,
     * 这将引起严重的程序漏洞.
     * 该函数类似于进程结束时调用exit函数.
     */
    // pthread_exit(NULL);
    pthread_exit("Thank you for the CPU time");
}
