#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"

MYSQL my_connection
MYSQL_RES *res_ptr;
MYSQL_ROW sqlrow;

void display_header();
void display_row();

int main(int argc, char const *argv[])
{
    int res;
    int first_row = 1;

    mysql_init(&my_connection);
    if (mysql_real_connection(&my_connection, "localhost", "rick",
                                                    "secret", "foo", 0, NULL, 0)) {
        printf("Connection success\n");
        res = mysql_query(&my_connection, "SELECT childno, fname,
                                                    age FROM children WHERE age > 5");
        if (res) {
            fprintf(stderr, "SELECT error: %s\n", mysql_error(&my_connection));
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
                    display_header();
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

void display_header() {
    MYSQL_FIELD *field_ptr;

    printf("Column detail:\n");
    /**
     * MYSQL_FIELD *mysql_fetch_field(MYSQL_RES *result)
     * 同时将元数据与数据提取到一个新的结构中.
     * 需要重复使用此函数,直到返回表示数据结束的NULL值为止.
     * 然后使用指向字段结构数据的指针来得到关于列的信息.
     * MYSQL_FIELD结构中的成员
     *     char *name;                   列名
     *     char *table;                  列所属表名
     *     char *def;                    调用mysql_list_fields时,它将包含该列的默认值
     *     enum enum_field_type type;    列类型
     *     unsigned int length;          列宽,定义表时指定
     *     unsigned int max_length;      使用mysql_store_result时,包含以字节为单位的提取提取的最长列的长度.使用mysql_use_result时不会被设置.
     *     unsigned int flags;           列定义标志,与得到的数据无关.常见标志: NOT_NULL_FLAG, PRI_KEY_FLAG, UNSIGNED_FLAG, AUTO_INCREMENT_FLAG, BINARY_FLAG
     *     unsigned int decimals;        小数点后的数字个数.仅对数字字段有效.
     *
     * enum enum_field_type type中常见列类型:
     *     FIELD_TYPE_DECIMAL
     *     FIELD_TYPE_LONG
     *     FIELD_TYPE_STRING
     *     FIELD_TYPE_VAR_STRING
     */
    while ((field_ptr = mysql_fetch_field(res_ptr)) != NULL) {
        printf("\t Name: %s\n", field_ptr->name);
        printf("\t Type: ");
        /**
         * 宏定义IS_SUM当字段类型为数字时返回true
         */
        if (IS_NUM(field_ptr->type)) {
            printf("Numeric field\\n");
        } else {
            switch (field_ptr->type) {
                case FIELD_TYPE_VAR_STRING:
                    printf("VARCHAR\n");
                    break;
                case FIELD_TYPE_LONG:
                    printf("LONG\n");
                    break;
                default:
                    printf("Type is %d, check in mysql.h\n", field_ptr->type);
            }
        }
        printf("\t Max width %ld\n", field_ptr->length);
        if (field_ptr->flags & AUTO_INCREMENT_FLAG)
            printf("\t Auto increments\n");
        printf("\n");
    }
}

void display_row() {
    unsigned int field_count;

    field_count = 0;
    /**
     * unsigned int mysql_field_count(MYSQL *connection);
     * 接受连接对象,并返回结果集中的字段数目.
     */
    while (field_count < mysql_field_count(&my_connection)) {
        if (sqlrow[field_count])
            printf("%s ", sqlrow[field_count]);
        else 
            printf("NULL");
        field_count ++;
    }
    printf("\n");
}