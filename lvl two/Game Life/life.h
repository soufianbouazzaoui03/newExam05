#ifndef LIFE_H
#define LIFE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define LIVE_CELL '0'
#define DEAD_CELL ' '

void print_grid(char **grid, int we, int he);
void creat_grid(char **grid, int we, int he);
int count_nb(char **grid, int x, int y, int we, int he);
void simulate(char **grid, char **new_grid, int we, int he);

#endif