#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"

MYSQL my_connection
MYSQL_RES *res_ptr;
MYSQL_ROW sqlrow;

void display_row();

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
            res_ptr = mysql_use_result(&my_connection);
            if (res_ptr) {
                /**
                 * MYSQL_ROW mysql_fetch_row(MYSQL_RES *result);
                 * 调用mysql_use_result后必须反复调用mysql_fetch_row直到提取了所有的数据.
                 * 如果没有从mysql_use_result中得到所有数据,后续的的提取数据操作可能返回遭到破坏的信息.
                 */
                while ((sqlrow = mysql_fetch_row(res_ptr))) {
                    printf("Fetched data ...\n");
                    display_row();
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

void display_row() {
    unsigned int field_count;

    field_count = 0;
    /**
     * unsigned int mysql_field_count(MYSQL *connection);
     * 接受连接对象,并返回结果集中的字段数目.
     */
    while (field_count < mysql_field_count(&my_connection)) {
        printf("%s ", sqlrow[field_count]);
        field_count ++;
    }
    printf("\n");
}