#ifndef BSQ_H
#define BSQ_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct {
	char *raw_data;
	char **grid;
	int **obs_grid;
	char obs;
	char empty;
	char full;
	int he;
	int we;
} t_map;

int ft_strlen(const char *s);
int is_print(char c);
int all_unique(char a, char b, char c);
void ft_bzero(void *dest, size_t n);
int atoi_safe(const char *s);

ssize_t read_stream_wrapper(FILE *stream, char *buf, size_t n);
char *slurp(FILE *stream);
void map_error(void);

int alloc_obs(t_map *m);
int alloc_obs_row(t_map *m, int row);
void free_grid(int **grid);

int is_valide_row(t_map *m, const char *row);
int handle_row(int idx, t_map *m, char *row);
int parse_map(t_map *m, FILE *stream);

int get_iz(t_map *m, int r, int c);
int p_map(t_map *m);

int count_obs(t_map *m, int r, int c, int size);
int inside_square(int r, int c, int br, int bc, int size);
void solve(t_map *m);

int handle_map(FILE *stream);
int main(int ac, char **av);

#endif