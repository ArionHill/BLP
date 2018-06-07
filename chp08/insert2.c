#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"

int main(int argc, char const *argv[])
{
    MYSQL *connection;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    int res;
    
    /**
     * MYSQL *mysql_init(MYSQL *)通常传递NULL
     * 例:
     * MYSQL *connection;
     * connection = mysql_init(NULL);
     * 传递已有结构,将被重新初始化
     * MYSQL connection;
     * mysql_init(&connection);
     */
    connection = mysql_init(NULL);
    /**
     * MYSQL *mysql_real_connect(MYSQL *connection,
     *         const char *server_host,
     *         const char *sql_user_name,
     *         const char *sql_password,
     *         const char *db_name,
     *         unsigned int port_number,
     *         const char *unix_socket_name,
     *         unsigned int flags);
     * port_number和unix_socket_name分别应该为0和NULL,除非改变了MySQL安装时的默认设置.
     */
    if (mysql_real_connect(connection, "localhost", "rick", "rick", "foo", 0, NULL, 0)) {
        printf("Connection success\n");

        /**
         * int mysql_query(MYSQL *connection, const char *query)
         * 执行SQL语句.成功返回0.对于包含二进制数据的查询,可以使用mysql_real_query.
         */
        res = mysql_query(connection, "INSERT INTO children(fname, age)VALUES('Ann', 3)");

        if (!res) {
            /**
             * my_ulonglong mysql_affected_rows(MYSQL *connection)
             * 检查受影响的行数.出于移植性的考虑使用my_ulonglong类型.使用printf时推荐使用%lu格式.
             */
            printf("Inserted %lu rows\n",
                            (unsigned long)mysql_affected_rows(connection));
            res = mysql_query(connection, "SELECT LAST_INSERT_ID()");
            if (res) {
                printf("SELECT error: %s\n", mysql_error(connection));
            } else {
                /**
                 * MYSQL_RES *mysql_use_result(MYSQL *connection);
                 * 一次提取一行数据.遇到错误时返回NULL,成功时返回指向结果集对象的指针.
                 */
                res_ptr = mysql_use_result(connection);
                if (res_ptr) {
                    while ((sqlrow = mysql_fetch_row(res_ptr))) {
                        printf("We inserted childno %s\n", sqlrow[0]);
                    }
                    mysql_free_result(res_ptr);
                }
            }
        } else {
            fprintf(stderr, "Insert error %d: %s\n", mysql_errno(connection),
                                                    mysql_error(connection));
        }

        mysql_close(connection);
    } else {
        printf("Connection failed %d\n", mysql_errno(connection));
        if (mysql_errno(connection)) {
            fprintf(stderr, "Connection error %d: %s\n",
                    mysql_errno(connection), mysql_error(connection));
        }
    }

    
    return 0;
}
