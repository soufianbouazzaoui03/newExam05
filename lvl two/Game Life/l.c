#include "life.h"

void print_grid(char **grid, int we, int he){
	for (int i = 0; i < he; i++){
		for(int j = 0; j < we; j++)
			putchar(grid[i][j]);
		pucthar('\n');
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
int count_nb(char **grid, int x, int y, int we, int he){
	int count = 0;
	for(dy = -1; dy <= 1; dy++){
		for(dx = -1; dx <= 1; dx++){
			if (dy == 0 && dx == 0) continue;
			int ny = y + dy;
			int nx = x + dx;
			if(ny >= 0 && ny < he && nx >= 0 && nx < we &&grid[ny][nx] == LIVE_CELL)
				count++;
		}
	}
	return count;
}
void simulate(char **grid, char **new_grid, int we, int he){
	for(int y = 0; y < he; y++){
		for (int x = 0; x < we; x++){
			int nb = count_nb()
		}
	}
}

int main(int ac; char **av){
	if(ac != 4)
		return 1;
	int we 
}