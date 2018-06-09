#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"

MYSQL my_connection
MYSQL_RES *res_ptr;
MYSQL_ROW sqlrow;

int main(int argc, char const *argv[])
{
    int res;

    mysql_init(&my_connection);
    if (mysql_real_connection(&my_connection, "localhost", "rick",
                                                    "secret", "foo", 0, NULL, 0)) {
        printf("Connection success\n");
        res = mysql_query(&my_connection, "SELECT childno, fname,
                                                    age FROM children WHERE age > 5");
        if (res) {
            printf("SELECT error: %s\n", mysql_error(&my_connection));
        } else {
            /**
             * MYSQL_RES *mysql_store_result(MYSQL *connection);
             * 立刻保存在客户端返回的所有数据.返回一个指向结果集的结构指针,如果失败返回NULL.
             */
            res_ptr = mysql_store_result(&my_connection);
            if (res_ptr) {
                /**
                 * my_ulonglong mysql_num_rows(MYSQL_RES *result)
                 * mysql_store_result调用成功后,调用mysql_num_rows得到返回记录的数目,没有返回记录时值为0.
                 */
                printf("Retrieved %lu rows\n", (unsigned long)mysql_num_rows(res_ptr));
                /**
                 * MYSQL_ROW mysql_fetch_row(MYSQL_RES *result);
                 * 从mysql_store_result得到的结果结构中提取一行,并将其放入一个结构中.数据用完或发生错误时返回NULL.
                 */
                while ((sqlrow = mysql_fetch_row(res_ptr))) {
                    printf("Fetched data ...\n");
                }
                if (mysql_errno(&my_connection)) {
                    fprintf(stderr, "Retrive error: %s\n", mysql_error(&my_connection));
                }
                /**
                 * 完成对结果集的操作后,必须调用此函数让MySQL库清理它分配的对象.
                 */
                mysql_free_result(res_ptr);
            }
        }
        mysql_close(&my_connection);
    } else {
        fprintf(stderr, "Connection failed\n");
        if (mysql_errno(&my_connection)) {
            fprintf(stderr, "Connection error %d: %s\n",
                        mysql_errno(&my_connection), mysql_error(&my_connection));
        }
    }
    return EXIT_SUCCESS;
}
