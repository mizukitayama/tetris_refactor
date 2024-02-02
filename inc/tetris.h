#	ifndef TETRIS_H
# define TETRIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <stdbool.h>


#define ROW 20
#define COL 8

#define SCORE			100

#define BLOCK				'#'
#define BLANK				'.'

#define RIGHT_KEY		'd'
#define LEFT_KEY		'a'
#define DOWN_KEY		's'
#define TURN_KEY		'w'

#define WINDOW_NAME "42 Tetris\n"
#define END_MESSAGE "\nGame over!\n"

#define BLOCK_NUMS	7


typedef struct s_block
{
	char **array;
	int width, row, col;
}	t_block;


const t_block block_types[BLOCK_NUMS] = {
		{(char *[]){(char[]){0, 1, 1},
								(char[]){1, 1, 0},
								(char[]){0, 0, 0}}, 3},

		{(char *[]){(char[]){1, 1, 0},
								(char[]){0, 1, 1},
								(char[]){0, 0, 0}}, 3},

		{(char *[]){(char[]){0, 1, 0},
								(char[]){1, 1, 1},
								(char[]){0, 0, 0}}, 3},

		{(char *[]){(char[]){0, 0, 1},
								(char[]){1, 1, 1},
								(char[]){0, 0, 0}}, 3},

		{(char *[]){(char[]){1, 0, 0},
								(char[]){1, 1, 1},
								(char[]){0, 0, 0}}, 3},

		{(char *[]){(char[]){1, 1},
								(char[]){1, 1}}, 2},

		{(char *[]){(char[]){0, 0, 0, 0},
								(char[]){1, 1, 1, 1},
								(char[]){0, 0, 0, 0},
								(char[]){0, 0, 0, 0}}, 4}};


char				game_table[ROW][COL] = {0};
int					final_score = 0;
bool				game_on = true;
suseconds_t	auto_drop_timer = 400000;
int					timer_decrease = 1000;
t_block			current_block;
struct			timeval before_now, now;

#	endif