#include "bsq.h"

int ft_strlen(const char s[static 1]) {
	int len = 0;
	while (*s++)
		++len;
	return len;
}

int is_printable(char c) {
	if (c >= 32 && c <= 126)
		return 1;
	return 0;
}

int all_unique(char a, char b, char c) {
	if (a == b || a == c || b == c)
		return 0;
	return 1;
}

void ft_bzero(void *dest, size_t n) {
	unsigned char *d = dest;

	for (size_t idx = 0; idx < n; ++idx)
		d[idx] = 0;
}

int parse_uint_strict(const char s[static 1]) {
	int res = 0;
	int prev_res = -1;

	while (*s) {
		if (*s >= '0' && *s <= '9') {
			res = res * 10 + *s - '0';
			if (res < prev_res)
				return -1;
			prev_res = res;
		} else {
			return -1;
		}
		++s;
	}
	return res;
}


ssize_t read_stream_wrapper(FILE stream[static 1], char *buf, size_t n) {
	ssize_t offset;

	for (offset = 0; offset < (ssize_t)n; ++offset) {
		int ret = fscanf(stream, "%c", buf + offset);
		if (ret == EOF)
			break;
	}
	return offset;
}

char *slurp(FILE stream[static 1]) {
	ssize_t bytes_read;
	ssize_t total_bytes_read = 0;
	char *buffer;
	char *previous_buffer = NULL;

	while (1) {
		if ((buffer = malloc(sizeof(*buffer) * (total_bytes_read + BUFSIZ + 1))) == NULL)
			return NULL;

		for (int idx = 0; previous_buffer && (buffer[idx] = previous_buffer[idx]); ++idx);
		free(previous_buffer);

		if ((bytes_read = read_stream_wrapper(stream, buffer + total_bytes_read, BUFSIZ)) < 0)
			return (free(buffer), NULL);
		else if (bytes_read == 0)
			break;

		total_bytes_read += bytes_read;
		buffer[total_bytes_read] = 0;
		previous_buffer = buffer;
	}

	buffer[total_bytes_read] = 0;
	return buffer;
}

void map_error() {
	fprintf(stderr, "map error\n");
}


int alloc_obstacles(t_map *map) {
	size_t size = sizeof(*map->obstacle_counts) * ((size_t)map->n_rows + 1);

	map->obstacle_counts = malloc(size);
	if (map->obstacle_counts == NULL)
		return EXIT_FAILURE;

	ft_bzero(map->obstacle_counts, size);

	return EXIT_SUCCESS;
}

int alloc_obstacle_row(t_map *map, int row_idx) {
	size_t size = sizeof(*map->obstacle_counts[row_idx]) * (size_t)map->n_cols;

	map->obstacle_counts[row_idx] = malloc(size);
	if (map->obstacle_counts[row_idx] == NULL)
		return EXIT_FAILURE;

	ft_bzero(map->obstacle_counts[row_idx], size);

	return EXIT_SUCCESS;
}

void free_int_array(int **arr) {
	int i = 0;
	while (arr && arr[i])
		free(arr[i++]);
	free(arr);
}


int is_valid_row(t_map map[static 1], const char *s) {
	while (*s) {
		if (*s != map->obstacle && *s != map->empty)
			return 0;
		++s;
	}
	return 1;
}

int handle_row(int row_idx, t_map map[static 1], char row[static 1]) {
	int row_len = ft_strlen(row);

	if (row_idx == -1) {
		if (row_len < 7)
			return EXIT_FAILURE;

		map->full = row[row_len - 1];
		map->obstacle = row[row_len - 3];
		map->empty = row[row_len - 5];

		if (!is_printable(map->full) || !is_printable(map->obstacle) || !is_printable(map->empty))
			return EXIT_FAILURE;

		if (!all_unique(map->full, map->obstacle, map->empty))
			return EXIT_FAILURE;

		row[row_len - 6] = 0;
		map->n_rows = parse_uint_strict(row);

		if (map->n_rows <= 0)
			return EXIT_FAILURE;

		map->rows = malloc(sizeof(*map->rows) * (size_t)map->n_rows);
		if (map->rows == NULL)
			return EXIT_FAILURE;

		if (alloc_obstacles(map) == EXIT_FAILURE)
			return EXIT_FAILURE;
	} else if (row_idx < map->n_rows) {
		if (row_idx == 0) {
			map->n_cols = row_len;
			if (map->n_cols == 0)
				return EXIT_FAILURE;
		} else if (row_len != map->n_cols)
			return EXIT_FAILURE;

		if (!is_valid_row(map, row))
			return EXIT_FAILURE;

		map->rows[row_idx] = row;

		if (alloc_obstacle_row(map, row_idx) == EXIT_FAILURE)
			return EXIT_FAILURE;
	} else
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int parse_map(t_map map[static 1], FILE stream[static 1]) {
	char *data = slurp(stream);

	if (data == NULL)
		return EXIT_FAILURE;
	map->raw_data = data;
	
	int offset = 0;
	int row_idx = -2;
	int offset_to_start_of_row = 0;
	int ends_with_nl = 0;
	while (data[offset]) {
		ends_with_nl = 0;

		if (data[offset] == '\n') {
			data[offset] = 0;
			++row_idx;
			ends_with_nl = 1;

			if (handle_row(row_idx, map, data + offset_to_start_of_row) == EXIT_FAILURE)
				return (free(data), free(map->rows), free_int_array(map->obstacle_counts), EXIT_FAILURE);

			offset_to_start_of_row = offset + 1;
		}
		++offset;
	}

	if (!ends_with_nl || row_idx != map->n_rows - 1)
		return (free(data), free(map->rows), free_int_array(map->obstacle_counts), EXIT_FAILURE);

	return EXIT_SUCCESS;
}


int get_int_or_zero(t_map *map, int row_idx, int col_idx) {
	if (row_idx < 0 || col_idx < 0)
		return 0;
	return map->obstacle_counts[row_idx][col_idx];
}

int preprocess_map(t_map map[static 1]) {
	for (int row_idx = 0; row_idx < map->n_rows; ++row_idx) {
		for (int col_idx = 0; col_idx < map->n_cols; ++col_idx) {
			int is_obstacle = map->rows[row_idx][col_idx] == map->obstacle;

			int top_obstacle_count = get_int_or_zero(map, row_idx - 1, col_idx);
			int left_obstacle_count = get_int_or_zero(map, row_idx, col_idx - 1);
			int top_left_obstacle_count = get_int_or_zero(map, row_idx - 1, col_idx - 1);

			int obstacle_count = is_obstacle + top_obstacle_count + left_obstacle_count - top_left_obstacle_count;

			map->obstacle_counts[row_idx][col_idx] = obstacle_count;
		}
	}
	return EXIT_SUCCESS;
}


int count_obstacles(t_map map[static 1], int row_idx, int col_idx, int square_size) {
	if (row_idx - 1 + square_size >= map->n_rows || col_idx - 1 + square_size >= map->n_cols)
		return -1; // out of bounds, don't need to check this and bigger squares

	int obstacles_top_left = get_int_or_zero(map, row_idx - 1, col_idx - 1);
	int obstacles_top_right = get_int_or_zero(map, row_idx - 1, col_idx - 1 + square_size);
	int obstacles_bottom_right = get_int_or_zero(map, row_idx - 1 + square_size, col_idx - 1 + square_size);
	int obstacles_bottom_left = get_int_or_zero(map, row_idx - 1 + square_size, col_idx - 1);

	int square_obstacle_count = obstacles_top_left - obstacles_top_right + obstacles_bottom_right - obstacles_bottom_left;
	return square_obstacle_count;
}

int inside_square(int row_idx, int col_idx, int best_row, int best_col, int square_size) {
	if (row_idx >= best_row && row_idx < best_row + square_size)
		if (col_idx >= best_col && col_idx < best_col + square_size)
			return 1;
	return 0;
}

void solve(t_map map[static 1]) {
	int best_row = -1, best_col = -1, square_size = 0;

	for (int row_idx = 0; row_idx < map->n_rows; ++row_idx) {
		for (int col_idx = 0; col_idx < map->n_cols; ++col_idx) {
			while (count_obstacles(map, row_idx, col_idx, square_size) == 0) {
				++square_size;
				best_row = row_idx;
				best_col = col_idx;
			}
		}
	}
	--square_size;
	print_filled_map(map, best_row, best_col, square_size);
}


void print_filled_map(t_map map[static 1], int best_row, int best_col, int square_size) {
	for (int row_idx = 0; row_idx < map->n_rows; ++row_idx) {
		for (int col_idx = 0; col_idx < map->n_cols; ++col_idx) {
			int c = map->rows[row_idx][col_idx];
			if (inside_square(row_idx, col_idx, best_row, best_col, square_size))
				(void)fprintf(stdout, "%c", map->full);
			else
				(void)fprintf(stdout, "%c", c);
		}
		(void)fprintf(stdout, "\n");
	}
}


int handle_map(FILE stream[static 1]) {
	t_map map;

	ft_bzero(&map, sizeof(map));
	if (parse_map(&map, stream) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (preprocess_map(&map) == EXIT_FAILURE)
		return EXIT_FAILURE;
	solve(&map);
	free(map.raw_data);
	free(map.rows);
	free_int_array(map.obstacle_counts);
	return EXIT_SUCCESS;
}

int main(int ac, char **av) {
	if (ac < 1) {
		map_error();
	} else if (ac < 2) {
		if (handle_map(stdin) == EXIT_FAILURE)
			map_error();
		putchar('\n'); // même stdin doit être suivi d'un saut de ligne
	} else {
		for (int idx = 1; idx < ac; ++idx) {
			FILE *stream = fopen(av[idx], "r");
			if (stream == NULL) {
				map_error();
				putchar('\n');
				continue;
			}
			if (handle_map(stream) == EXIT_FAILURE)
				map_error();
			fclose(stream);
			putchar('\n');
		}
	}
	return EXIT_SUCCESS;
}