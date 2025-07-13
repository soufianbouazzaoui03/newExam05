#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    int rows;
    char empty;
    char obstacle;
    char full;
} MapInfo;

typedef struct {
    char **grid;
    int rows;
    int cols;
} Map;

typedef struct {
    int size;
    int row;
    int col;
} Square;

// Function to print error message
void print_error(void) {
    fprintf(stderr, "map error\n");
}

// Function to free map memory
void free_map(Map *map) {
    if (map && map->grid) {
        for (int i = 0; i < map->rows; i++) {
            free(map->grid[i]);
        }
        free(map->grid);
        map->grid = NULL;
    }
}

// Function to validate map info
int validate_map_info(MapInfo *info) {
    if (info->rows <= 0) return 0;
    if (info->empty == info->obstacle || 
        info->empty == info->full || 
        info->obstacle == info->full) return 0;
    return 1;
}

// Function to parse the first line
int parse_map_info(char *line, MapInfo *info) {
    char empty_str[2], obstacle_str[2], full_str[2];
    
    if (sscanf(line, "%d %s %s %s", &info->rows, empty_str, obstacle_str, full_str) != 4) {
        return 0;
    }
    
    info->empty = empty_str[0];
    info->obstacle = obstacle_str[0];
    info->full = full_str[0];
    
    return validate_map_info(info);
}

// Function to validate a map line
int validate_line(char *line, MapInfo *info, int expected_length) {
    int len = strlen(line);
    
    // Remove newline if present
    if (len > 0 && line[len-1] == '\n') {
        line[len-1] = '\0';
        len--;
    }
    
    if (expected_length != -1 && len != expected_length) return 0;
    
    for (int i = 0; i < len; i++) {
        if (line[i] != info->empty && line[i] != info->obstacle && line[i] != info->full) {
            return 0;
        }
    }
    
    return len;
}

// Function to read map from file
Map* read_map(FILE *file, MapInfo *info) {
    Map *map = malloc(sizeof(Map));
    if (!map) return NULL;
    
    map->grid = malloc(info->rows * sizeof(char*));
    if (!map->grid) {
        free(map);
        return NULL;
    }
    
    map->rows = info->rows;
    map->cols = -1;
    
    char *line = NULL;
    size_t len = 0;
    
    for (int i = 0; i < info->rows; i++) {
        if (getline(&line, &len, file) == -1) {
            free_map(map);
            free(map);
            free(line);
            return NULL;
        }
        
        int line_len = validate_line(line, info, map->cols);
        if (line_len == 0) {
            free_map(map);
            free(map);
            free(line);
            return NULL;
        }
        
        if (map->cols == -1) {
            map->cols = line_len;
        }
        
        map->grid[i] = malloc((line_len + 1) * sizeof(char));
        if (!map->grid[i]) {
            free_map(map);
            free(map);
            free(line);
            return NULL;
        }
        
        strcpy(map->grid[i], line);
    }
    
    free(line);
    
    // Validate map has at least one cell
    if (map->rows == 0 || map->cols == 0) {
        free_map(map);
        free(map);
        return NULL;
    }
    
    return map;
}

// Function to find minimum of three numbers
int min3(int a, int b, int c) {
    int min_ab = (a < b) ? a : b;
    return (min_ab < c) ? min_ab : c;
}

// Function to find the largest square using DP
Square find_largest_square(Map *map, MapInfo *info) {
    Square result = {0, 0, 0};
    
    if (!map || map->rows == 0 || map->cols == 0) return result;
    
    // Create DP table
    int **dp = malloc(map->rows * sizeof(int*));
    for (int i = 0; i < map->rows; i++) {
        dp[i] = calloc(map->cols, sizeof(int));
    }
    
    // Fill DP table
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            if (map->grid[i][j] == info->obstacle || map->grid[i][j] == info->full) {
                dp[i][j] = 0;
            } else if (i == 0 || j == 0) {
                dp[i][j] = 1;
            } else {
                dp[i][j] = min3(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1;
            }
            
            // Update result if we found a larger square
            // Priority: larger size, then top-most, then left-most
            if (dp[i][j] > result.size || 
                (dp[i][j] == result.size && i - dp[i][j] + 1 < result.row) ||
                (dp[i][j] == result.size && i - dp[i][j] + 1 == result.row && j - dp[i][j] + 1 < result.col)) {
                result.size = dp[i][j];
                result.row = i - dp[i][j] + 1;
                result.col = j - dp[i][j] + 1;
            }
        }
    }
    
    // Free DP table
    for (int i = 0; i < map->rows; i++) {
        free(dp[i]);
    }
    free(dp);
    
    return result;
}

// Function to fill the square and print the map
void print_solution(Map *map, Square square, MapInfo *info) {
    // Fill the square
    for (int i = square.row; i < square.row + square.size; i++) {
        for (int j = square.col; j < square.col + square.size; j++) {
            if (i < map->rows && j < map->cols) {
                map->grid[i][j] = info->full;
            }
        }
    }
    
    // Print the map
    for (int i = 0; i < map->rows; i++) {
        printf("%s\n", map->grid[i]);
    }
}

// Function to process a single map
void process_map(FILE *file) {
    char *line = NULL;
    size_t len = 0;
    MapInfo info;
    
    // Read first line
    if (getline(&line, &len, file) == -1) {
        free(line);
        return;
    }
    
    // Parse map info
    if (!parse_map_info(line, &info)) {
        print_error();
        free(line);
        return;
    }
    
    // Read map
    Map *map = read_map(file, &info);
    if (!map) {
        print_error();
        free(line);
        return;
    }
    
    // Find largest square
    Square square = find_largest_square(map, &info);
    
    // Print solution
    print_solution(map, square, &info);
    
    // Cleanup
    free_map(map);
    free(map);
    free(line);
}

// Function to process file
void process_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        print_error();
        return;
    }
    
    process_map(file);
    fclose(file);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        // Read from stdin
        process_map(stdin);
    } else {
        // Process each file
        for (int i = 1; i < argc; i++) {
            process_file(argv[i]);
            if (i < argc - 1) {
                printf("\n");
            }
        }
    }
    
    return 0;
}