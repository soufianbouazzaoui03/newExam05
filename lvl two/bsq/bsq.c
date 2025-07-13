#include "bsq.h"

// === UTILS ===
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

// === FILE I/O ===
ssize_t read_stream_wrapper(FILE *stream, char *buf, size_t n) {
	ssize_t i;
	for (i = 0; i < (ssize_t)n; ++i)
		if (fscanf(stream, "%c", buf + i) == EOF) break;
	return i;
}

char *slurp(FILE *stream) {
	ssize_t total = 0;
	char *buf = NULL, *prev = NULL;
	while (1) {
		if (!(buf = malloc(total + BUFSIZ + 1))) return NULL;
		for (int i = 0; prev && (buf[i] = prev[i]); ++i);
		free(prev);
		ssize_t read = read_stream_wrapper(stream, buf + total, BUFSIZ);
		if (read <= 0) break;
		total += read;
		buf[total] = 0;
		prev = buf;
	}
	return buf;
}

void map_error() { fprintf(stderr, "map error\n"); }

// === MEMORY ===
int alloc_obs(t_map *m) {
	m->obs_grid = malloc(sizeof(*m->obs_grid) * (m->he + 1));
	if (!m->obs_grid) return EXIT_FAILURE;
	ft_bzero(m->obs_grid, sizeof(*m->obs_grid) * (m->he + 1));
	return EXIT_SUCCESS;
}

int alloc_obs_row(t_map *m, int row) {
	m->obs_grid[row] = malloc(sizeof(int) * m->we);
	if (!m->obs_grid[row]) return EXIT_FAILURE;
	ft_bzero(m->obs_grid[row], sizeof(int) * m->we);
	return EXIT_SUCCESS;
}

void free_grid(int **grid) {
	for (int i = 0; grid && grid[i]; ++i) free(grid[i]);
	free(grid);
}

// === PARSING ===
int is_valide_row(t_map *m, const char *row) {
	while (*row) if (*row != m->obs && *row != m->empty) return 0; else ++row;
	return 1;
}

int handle_row(int idx, t_map *m, char *row) {
	int len = ft_strlen(row);
	if (idx == -1) {
		if (len < 7) return EXIT_FAILURE;
		m->full = row[len-1]; m->obs = row[len-3]; m->empty = row[len-5];
		if (!is_print(m->full) || !is_print(m->obs) || !is_print(m->empty)) return EXIT_FAILURE;
		if (!all_unique(m->full, m->obs, m->empty)) return EXIT_FAILURE;
		row[len-6] = 0;
		if ((m->he = atoi_safe(row)) <= 0) return EXIT_FAILURE;
		if (!(m->grid = malloc(sizeof(*m->grid) * m->he))) return EXIT_FAILURE;
		return alloc_obs(m);
	}
	if (idx >= m->he) return EXIT_FAILURE;
	if (idx == 0) { if ((m->we = len) == 0) return EXIT_FAILURE; }
	else if (len != m->we) return EXIT_FAILURE;
	if (!is_valide_row(m, row)) return EXIT_FAILURE;
	m->grid[idx] = row;
	return alloc_obs_row(m, idx);
}

int parse_map(t_map *m, FILE *stream) {
	char *data = slurp(stream);
	if (!data) return EXIT_FAILURE;
	m->raw_data = data;
	int offset = 0, row_idx = -2, start = 0, e_nl = 0;
	while (data[offset]) {
		if (data[offset] == '\n') {
			data[offset] = 0; ++row_idx; e_nl = 1;
			if (handle_row(row_idx, m, data + start) == EXIT_FAILURE)
				return (free(data), free(m->grid), free_grid(m->obs_grid), EXIT_FAILURE);
			start = offset + 1;
		}
		++offset;
	}
	if (!e_nl || row_idx != m->he - 1)
		return (free(data), free(m->grid), free_grid(m->obs_grid), EXIT_FAILURE);
	return EXIT_SUCCESS;
}

// === ALGORITHM ===
int get_iz(t_map *m, int r, int c) {
	return (r < 0 || c < 0) ? 0 : m->obs_grid[r][c];
}

int p_map(t_map *m) {
	for (int r = 0; r < m->he; ++r)
		for (int c = 0; c < m->we; ++c)
			m->obs_grid[r][c] = (m->grid[r][c] == m->obs) + get_iz(m, r-1, c) + get_iz(m, r, c-1) - get_iz(m, r-1, c-1);
	return EXIT_SUCCESS;
}

int count_obs(t_map *m, int r, int c, int size) {
	if (r-1+size >= m->he || c-1+size >= m->we) return -1;
	return get_iz(m, r-1, c-1) - get_iz(m, r-1, c-1+size) + get_iz(m, r-1+size, c-1+size) - get_iz(m, r-1+size, c-1);
}

int inside_square(int r, int c, int br, int bc, int size) {
	return r >= br && r < br+size && c >= bc && c < bc+size;
}

void solve(t_map *m) {
	int br = -1, bc = -1, size = 0;
	for (int r = 0; r < m->he; ++r)
		for (int c = 0; c < m->we; ++c)
			while (count_obs(m, r, c, size) == 0)
				{ ++size; br = r; bc = c; }
	--size;
	for (int r = 0; r < m->he; ++r) {
		for (int c = 0; c < m->we; ++c)
			fprintf(stdout, "%c", inside_square(r, c, br, bc, size) ? m->full : m->grid[r][c]);
		fprintf(stdout, "\n");
	}
}

// === MAIN ===
int handle_map(FILE *stream) {
	t_map m;
	ft_bzero(&m, sizeof(m));
	if (parse_map(&m, stream) == EXIT_FAILURE) return EXIT_FAILURE;
	if (p_map(&m) == EXIT_FAILURE) return EXIT_FAILURE;
	solve(&m);
	free(m.raw_data); free(m.grid); free_grid(m.obs_grid);
	return EXIT_SUCCESS;
}

int main(int ac, char **av) {
	if (ac < 2) {
		if (handle_map(stdin) == EXIT_FAILURE) map_error();
		putchar('\n');
	} else {
		for (int i = 1; i < ac; ++i) {
			FILE *f = fopen(av[i], "r");
			if (!f) { map_error(); putchar('\n'); continue; }
			if (handle_map(f) == EXIT_FAILURE) map_error();
			fclose(f); putchar('\n');
		}
	}
	return EXIT_SUCCESS;
}