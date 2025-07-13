#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function to count live neighbors of a cell
int count_neighbors(char **board, int width, int height, int x, int y) {
    int count = 0;
    
    // Check all 8 directions around the cell
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // Skip the cell itself
            
            int nx = x + dx;
            int ny = y + dy;
            
            // Check if neighbor is within bounds and alive
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                if (board[ny][nx] == '0') {
                    count++;
                }
            }
            // Cells outside bounds are considered dead (no increment)
        }
    }
    
    return count;
}

// Apply one iteration of Game of Life rules
void apply_game_of_life(char **board, int width, int height) {
    // Create temporary board for next state
    char **next_board = malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        next_board[i] = malloc(width * sizeof(char));
    }
    
    // Calculate next state for each cell
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int neighbors = count_neighbors(board, width, height, x, y);
            char current = board[y][x];
            
            if (current == '0') { // Cell is alive
                if (neighbors == 2 || neighbors == 3) {
                    next_board[y][x] = '0'; // Survives
                } else {
                    next_board[y][x] = ' '; // Dies
                }
            } else { // Cell is dead
                if (neighbors == 3) {
                    next_board[y][x] = '0'; // Becomes alive
                } else {
                    next_board[y][x] = ' '; // Stays dead
                }
            }
        }
    }
    
    // Copy next state back to original board
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            board[y][x] = next_board[y][x];
        }
    }
    
    // Free temporary board
    for (int i = 0; i < height; i++) {
        free(next_board[i]);
    }
    free(next_board);
}

// Print the board
void print_board(char **board, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            putchar(board[y][x]);
        }
        putchar('\n');
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        return 1; // Invalid arguments
    }
    
    // Parse command line arguments
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    
    // Allocate board memory
    char **board = malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        board[i] = malloc(width * sizeof(char));
        // Initialize all cells as dead (space)
        for (int j = 0; j < width; j++) {
            board[i][j] = ' ';
        }
    }
    
    // Initialize pen position and state
    int pen_x = 0, pen_y = 0;
    int pen_down = 0; // 0 = up (not drawing), 1 = down (drawing)
    
    // Process drawing commands from stdin
    char command;
    while (read(0, &command, 1) == 1) {
        switch (command) {
            case 'w': // Move up
                if (pen_y > 0) pen_y--;
                break;
            case 'a': // Move left
                if (pen_x > 0) pen_x--;
                break;
            case 's': // Move down
                if (pen_y < height - 1) pen_y++;
                break;
            case 'd': // Move right
                if (pen_x < width - 1) pen_x++;
                break;
            case 'x': // Toggle pen
                pen_down = !pen_down;
                break;
        }
        
        // If pen is down and position is valid, mark cell as alive
        if (pen_down && pen_x >= 0 && pen_x < width && pen_y >= 0 && pen_y < height) {
            board[pen_y][pen_x] = '0';
        }
    }
    
    // Simulate Game of Life for specified iterations
    for (int i = 0; i < iterations; i++) {
        apply_game_of_life(board, width, height);
    }
    
    // Print final board state
    print_board(board, width, height);
    
    // Free allocated memory
    for (int i = 0; i < height; i++) {
        free(board[i]);
    }
    free(board);
    
    return 0;
}