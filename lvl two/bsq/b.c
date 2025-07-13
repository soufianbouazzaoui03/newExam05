#include "bsq.h"

int ft_strlen(const char *s) {
	int len = 0;
	while (*s++) ++len;
	return len;
}

int is_print(char c) { return c >= 32 && c <= 126; }
int all_unique(char a, char b, char c) { return !(a == b || a == c || b == c); }

void ft_bzero(void *dest, size_t n) {
	unsigned char *d = dest;
	for (size_t i = 0; i < n; ++i) d[i] = 0;
}

int atoi_safe(const char *s) {
	int res = 0, prev = -1;
	while (*s) {
		if (*s >= '0' && *s <= '9') {
			res = res * 10 + *s - '0';
			if (res < prev) return -1;
			prev = res;
		} else return -1;
		++s;
	}
	return res;
}

ssize_t read_stream_wrapper(FILE *stream, char *buf, size_t n){
	ssize_t i;
	for (i = 0; i < (ssize_t)n; ++i)
		if(fscanf(stream, "%c", buf + i) == EOF) break;
	return i;
}
char *slurp(FILE *stream){
	ssize_t total = 0;
	char *buf = NULL, *prev = NULL;
	while(1){
		if(!(buf = malloc(totla + BUFSIZE + 1))) return NULL;
		for(int i = 0; prev && (buf[i] = prev[i]); ++i);
		free(prev);
		ssize_t read = read_stream_wrapper(stream, buf + total, BUFSIZ);
		if(read <= 0) break;
		total += read;
		buf[total] = 0;
		prev = buf;
	}
	return buf;
}
void map_error(void);

int alloc_obs(t_map *m);
int alloc_obs_row(t_map *m, int row);
void free_grid(int **grid);

int is_valide_row(t_map *m, const char *row);
int handle_row(int idx, t_map *m, char *row){
	int len = ft_strlen(row);
	if(idx == -1){
		if(len < 7) return EXIT_FAILURE;

	}
}
int parse_map(t_map *m, FILE *stream){
	char *data = slurp(stream);
	if(!data) return EXIT_FAILURE;
	m->raw_grid = data;

}

int get_iz(t_map *m, int r, int c){
	return (r < 0 || c < 0) ? 0 : m->obs_grid[r][c];
}
int p_map(t_map *m){
	for (int r = 0; r < he; ++r)
		for (int c = 0; c < m->we; ++c)
			m->obs_grid[r][c] = (m->grid[r][c] == m->obs) + get_iz(m, r-1, c) + get_iz(m, r, c-1) - get_iz(m, r-1, c-1);
	return EXIT_SUCCESS
}

int count_obs(t_map *m, int r, int c, int size){
	if(r-1+size >= m)
}
int inside_square(int r, int c, int br, int bc, int size);
void solve(t_map *m){}

int handle_map(FILE *stream);
int main(int ac, char **av);