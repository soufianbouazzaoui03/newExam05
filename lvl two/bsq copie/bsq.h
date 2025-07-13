#ifndef BSQ_H
#define BSQ_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct {
	char *raw_data;
	char **rows;
	int **obstacle_counts;
	char obstacle;
	char empty;
	char full;
	int n_rows;
	int n_cols;
} t_map;

int ft_strlen(const char s[static 1]);
int is_printable(char c);
int all_unique(char a, char b, char c);
void ft_bzero(void *dest, size_t n);
int parse_uint_strict(const char s[static 1]);

ssize_t read_stream_wrapper(FILE stream[static 1], char *buf, size_t n);
char *slurp(FILE stream[static 1]);
void map_error(void);

int alloc_obstacles(t_map *map);
int alloc_obstacle_row(t_map *map, int row_idx);
void free_int_array(int **arr);

int is_valid_row(t_map map[static 1], const char *s);
int handle_row(int row_idx, t_map map[static 1], char row[static 1]);
int parse_map(t_map map[static 1], FILE stream[static 1]);

int get_int_or_zero(t_map *map, int row_idx, int col_idx);
int preprocess_map(t_map map[static 1]);

int count_obstacles(t_map map[static 1], int row_idx, int col_idx, int square_size);
int inside_square(int row_idx, int col_idx, int best_row, int best_col, int square_size);
void solve(t_map map[static 1]);

void print_filled_map(t_map map[static 1], int best_row, int best_col, int square_size);
int handle_map(FILE stream[static 1]);
int main(int ac, char **av);

#endif