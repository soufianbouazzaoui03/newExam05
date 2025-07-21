#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int count_nb(char **board, int x, int y, int w, int h) {
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0)
                continue;
            int nx = dx + x;
            int ny = dy + y;

            if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
                if (board[ny][nx] == '0')
                    count++;
            }
        }
    }
    return count;
}

void game(char **board, int w, int h) {
    char **cpy = malloc(sizeof(char *) * h);
    for (int i = 0; i < h; i++) {
        cpy[i] = malloc(w);
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int nb = count_nb(board, x, y, w, h);
            char c = board[y][x];
            if (c == '0') {
                if (nb == 2 || nb == 3) {
                    cpy[y][x] = '0';
                }
                else {
                    cpy[y][x] = ' ';
                }
            }else {
                if (nb == 3) {
                    cpy[y][x] = '0';
                }
                else {
                    cpy[y][x] = ' ';
                }

            }
        }
    }
    for (int i = 0 ; i < h; i++) {
        for (int j = 0; j < w; j++) {
            board[i][j] = cpy[i][j];
        }
    }
    for (int i = 0; i < h; i++)
        free(cpy[i]);
    free(cpy);
}

// void f(){system("leaks a.out");}

int main(int argc, char **argv) {
    if (argc != 4)
        return 1;
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int iter = atoi(argv[3]);

    char **board = malloc(sizeof(char *) * height);
    for (int i = 0; i < height; i++) {
        board[i] = malloc(sizeof(char) * width);
        for (int j = 0; j < width; j++) {
            board[i][j] = ' ';
        }
    }

    int pen_x = 0, pen_y = 0, pen_down = 0;

    char cmd;
    while (read(0, &cmd, 1) == 1) {
        if (cmd == 'w' && pen_y > 0)
            pen_y--;
        if (cmd == 's' && pen_y < height - 1)
            pen_y++;
        if (cmd == 'a' && pen_x > 0)
            pen_x--;
        if (cmd == 'd' && pen_x < width - 1)
            pen_x++;
        if (cmd == 'x')
            pen_down = !pen_down;

        if (pen_down && pen_x >= 0 && pen_y >= 0 && pen_x < width && pen_y < height)
            board[pen_y][pen_x] = '0';
    }

    for (int i = 0; i < iter; i++) {
        game(board, width, height);
    }
    for (int i = 0; i < height; i++) {
        for (int j =0 ; j < width; j++) {
            putchar(board[i][j]);
        }
        putchar('\n');
    }
    
    for (int i = 0; i < height; i++)
        free(board[i]);
    free(board);

}