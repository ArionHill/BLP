#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_function(void *arg);
pthread_mutex_t work_mutex;                                                          // 声明互斥量

#define WORK_SIZE 1024
char work_area[WORK_SIZE];                                                           // 工作区
int time_to_exit = 0;

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
    res = pthread_mutex_init(&work_mutex, NULL);
    if (res != 0) {
        perror("Mutex initialization failed");
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
    
    pthread_mutex_lock(&work_mutex);
    printf("Input some text. Enter 'end' to finish\n");
    while (!time_to_exit) {
        fgets(work_area, WORK_SIZE, stdin);
        pthread_mutex_unlock(&work_mutex);
        while(1) {
            pthread_mutex_lock(&work_mutex);
            if (work_area[0] != '\0') {
                pthread_mutex_unlock(&work_mutex);
                sleep(1);
            }
            else {
                break;
            }
        }        
    }
    pthread_mutex_unlock(&work_mutex);
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
    pthread_mutex_destroy(&work_mutex);
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
    sleep(1);
    pthread_mutex_lock(&work_mutex);
    while (strncmp("end", work_area, 3) != 0) {
        printf("You input %lu characters\n", strlen(work_area) -1);
        work_area[0] = '\0';
        pthread_mutex_unlock(&work_mutex);
        sleep(1);
        pthread_mutex_lock(&work_mutex);
        while (work_area[0] == '\0') {
            pthread_mutex_unlock(&work_mutex);
            sleep(1);
            pthread_mutex_lock(&work_mutex);
        }
    }
    time_to_exit = 1;
    work_area[0] = '\0';
    pthread_mutex_unlock(&work_mutex);
    pthread_exit(0);
}
