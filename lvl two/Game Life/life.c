#include "life.h"

void print_grid(char **grid, int we, int he)
{
	for (int i = 0; i < he; i++) {
		for (int j = 0; j < we; j++)
			putchar(grid[i][j]);
		putchar('\n'); 
	}
}

void creat_grid(char **grid, int we, int he) {
	int x = 0, y = 0, pen_down = 0;
	char cmd;

	for (int i = 0; i < he; i++) {
		for (int j = 0; j < we; j++)
			grid[i][j] = DEAD_CELL;
	}

	while (read(0, &cmd, 1) >0) {
		if (cmd == 'w' && y > 0) y--;
		else if (cmd == 'a' && x > 0) x--;
		else if (cmd == 's' && y < he - 1) y++;
		else if (cmd == 'd' && x < we - 1) x++;
		else if (cmd == 'x') pen_down = !pen_down;

		if (pen_down)
			grid[y][x] = LIVE_CELL;
	}
}

int count_nb(char **grid, int x, int y, int we, int he)
{
	int count = 0;

	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++){
			if (dx == 0 && dy == 0) continue;
			int ny = y + dy;
			int nx = x + dx;
			if (nx >= 0 && nx < we && ny >= 0 && ny < he && grid[ny][nx] == LIVE_CELL)
				count++;
		}
	}
	return count;
}

void simulate(char **grid, char **new_grid, int we, int he)
{
	for (int y = 0; y < he; y++) {
		for (int x = 0; x < we; x++) {
			int nb = count_nb(grid, x, y, we, he);
			if (grid[y][x] == LIVE_CELL)
				new_grid[y][x] = (nb == 2 || nb == 3) ? LIVE_CELL : DEAD_CELL;
			else 
				new_grid[y][x] = (nb == 3) ? LIVE_CELL : DEAD_CELL;
		}
	}

	for (int i = 0; i < he; i++) {
		for (int j = 0; j < we; j++)
			grid[i][j] = new_grid[i][j];
	}
	
}

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;
	
	int we = atoi(av[1]);
	int he = atoi(av[2]);
	int iter = atoi(av[3]);

	if (we <= 0 || he <= 0 || iter < 0)
		return 1;

	char **grid = malloc(he * sizeof(char *));
	char **new_grid = malloc(he * sizeof(char *));
	for (int i = 0; i < he; i++) {
		grid[i] = malloc(we * sizeof(char));
		new_grid[i] = malloc(we * sizeof(char));
	}

	creat_grid(grid, we, he);
	for (int i = 0; i < iter; i++)
	{
		simulate(grid, new_grid, we, he);
	}
	print_grid(grid, we, he);

	for (int i = 0; i < he; i++){
		free(grid[i]);
		free(new_grid[i]);
	}
	free(grid);
	free(new_grid);
	return 0;
}