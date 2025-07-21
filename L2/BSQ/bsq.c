#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int nlines;
    char empty;
    char obs;
    char full;
} map_info;

typedef struct {
    char **gred;
    int w;
    int h;
} map;

typedef struct {
    int size;
    int x;
    int y;
} square;

size_t ft_strlen(char *line) {
    size_t i = 0;
    while (line[i]) {
        i++;
    }
    return i;
}

int parseline(char *line, map_info info, map *rmap) {
    size_t len = ft_strlen(line);

    if (len != (size_t)rmap->w)
        return 0;
    for (size_t i = 0; i < len; i++) {
        if (line[i] != info.obs && line[i] != info.empty) {
            return 0;
        }
    }
    return 1;
}

void ft_strcpy(char *s1, char *s2) {
    while (*s2) {
        *s1 = *s2;
        s1++;
        s2++;
    }
    *s1 = '\0';
}

map *read_map(FILE *file, map_info *info) {
    map *rmap = malloc(sizeof(map));
    if (!rmap)
        return NULL;
    rmap->gred = malloc(sizeof(char *) * info->nlines);
    if (!rmap->gred)
        return NULL;
    
    char *line = NULL;
    size_t len = 0;
    for (int i = 0; i < info->nlines; i++) {
        if (getline(&line, &len, file) == -1) {
            free(rmap->gred);
            free(rmap);
            return NULL;
        }

        len = ft_strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
            rmap->h = info->nlines;
            rmap->w = len;
        }
        if (!parseline(line, *info, rmap)) {
            free(rmap->gred);
            free(rmap);
            return NULL;
        }
        else {
            rmap->gred[i] = malloc(sizeof(char) * len + 1);
            ft_strcpy(rmap->gred[i], line);
        }
    }
    return rmap;
}

int    getinfo(FILE *file, map_info* map) {
    char c;
    if (fscanf(file, "%d %c %c %c %c", &map->nlines, &map->empty, &map->obs, &map->full, &c) != 5) {
        return 0;
    }
    
    if (map->nlines <= 0)
        return 0;
    if (map->empty == map->obs || map->empty == map->full ||
        map->obs == map->full)
        return 0;
    return 1;
}

int min3(int a, int b, int c) {
    int min_ab = (a < b) ? a : b;
    return (min_ab < c) ? min_ab : c;
}

square findsqr(map *rmap, map_info *info) {
    square sqr = {0,0,0};
    int **dp = malloc(sizeof(int *) * rmap->h );
    for (int i = 0; i < rmap->h; i++)
        dp[i] = calloc(rmap->w, sizeof(int));

    for (int y = 0; y < rmap->h; y++) {
        for (int x = 0; x < rmap->w; x++) {
            if (rmap->gred[y][x] == info->obs)
                dp[y][x] = 0;
            else if(y == 0 || x == 0)
                dp[y][x] = 1;
            else {
                dp[y][x] = min3(dp[y - 1][x], dp[y - 1][x - 1], dp[y][x - 1]) + 1;
            }

            if (dp[y][x] > sqr.size) {
                sqr.size = dp[y][x];
                sqr.x = x - dp[y][x] + 1;
                sqr.y = y - dp[y][x] + 1;
            }
        }
    }
    for (int i = 0; i < rmap->h; i++)
        free(dp[i]);
    free(dp);
    return sqr;
}

void printres(square *sqr, map *rmap, map_info *info) {
    for (int i = sqr->y; i < sqr->y + sqr->size; i++) {
        for (int j = sqr->x; j < sqr->x + sqr->size; j++) {
            if (i < rmap->h && j < rmap->w)
                rmap->gred[i][j] = info->full;
        }
    }
    for (int i = 0 ; i < rmap->h; i++) {
        fprintf(stdout, "%s\n", rmap->gred[i]);
    }
}

void process_map(FILE *file) {
    map_info info;

    if (!getinfo(file, &info)) {
        return ;
    }

    map *map = read_map(file, &info);
    if (!map) {
        return ;
    }

    square sqr = findsqr(map, &info);
    printres(&sqr, map, &info);

    for (int i = 0; i <map->h; i++) {
        free(map->gred[i]);
    }
    free(map->gred);
    free(map);
}

void    process_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file)
        return;
    process_map(file);
}

int main(int argc, char **argv) {
    if (argc == 1)
        process_map(stdin);
    if (argc == 2) {
        process_file(argv[1]);
    }
    else {
        for (int i = 1; i < argc; i++) {
            process_file(argv[i]);
            if (i < argc - 1)
                fprintf(stdin, "\n");

        }
    }
}