#include <unistd.h>
#include <stdlib.h>
#include <curses.h>


int main(int argc, char const *argv[])
{
    WINDOW *pad_ptr;
    int x, y;
    int pad_lines;
    int pad_cols;
    char disp_char;

    initscr();
    pad_lines = LINES + 50;
    pad_cols = COLS + 50;
    /**
     * WINDOW *newpad(int number_of_lines, int number_of_columns);
     * 创建pad
     */
    pad_ptr = newpad(pad_lines, pad_cols);
    disp_char = 'a';

    for (x = 0; x < pad_lines; x ++) {
        for (y = 0; y < pad_cols; y ++) {
            mvwaddch(pad_ptr, x, y, disp_char);
            if (disp_char == 'z') disp_char = 'a';
            else disp_char ++;
        }
    }

    /**
     * int prefresh(WINDOW *pad_ptr, int pad_row, int pad_column,
     *              int screen_row_min, int screen_col_min,
     *              int screen_row_max, int screen_col_max);
     * 将pad从坐标(pad_row, pad_column)开始的区域写到屏幕上指定的显示区域,
     * 该显示区域的范围从坐标(screen_row_min, screen_col_min)到(screen_row_max,
     * screen_col_max).
     */
    prefresh(pad_ptr, 5, 7, 2, 2, 9, 9);
    sleep(1);
    prefresh(pad_ptr, LINES + 5, COLS + 7, 5, 5, 21, 19);
    sleep(1);
    delwin(pad_ptr);
    endwin();
    exit(EXIT_SUCCESS);
}
