#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>

const int grid_width = 30;
const int grid_height = 30;
pthread_t* tab;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);

    if (tab != NULL)
    {
        for (int i = 0; i < grid_height; ++i)
        {
            for (int j = 0; j < grid_width; ++j)
            {
                if (tab[i * grid_width + j] != 0)
                {
                    pthread_kill(tab[i * grid_width + j], SIGKILL);
                }
            }
        }
        free(tab);
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void* calculate_cell(void* arg)
{
    intptr_t* args = (intptr_t*)arg;
    intptr_t row = args[0];
    intptr_t col = args[1];
    char* src = (char*)args[2];
    char* dst = (char*)args[3];

    dst[row * grid_width + col] = is_alive((int)row, (int)col, src);

    free(args);

    return NULL;
}


void init_threads(char* src, char* dst)
{
    tab = calloc(grid_width * grid_height, sizeof(pthread_t));

    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            intptr_t* args = malloc(4 * sizeof(intptr_t));
            args[0] = (intptr_t)i;
            args[1] = (intptr_t)j;
            args[2] = (intptr_t)src;
            args[3] = (intptr_t)dst;

            pthread_create(&tab[i * grid_width + j], NULL, calculate_cell, (void*)args);
        }
    }
}
