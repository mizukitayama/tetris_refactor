#include "tetris.h"

// グローバル変数を初期化

char game_table[ROW][COL] = {0};
int final_score = 0;
bool game_on = true;
suseconds_t auto_drop_timer = 400000;
int timer_decrease = 1000;
t_block current_block;
struct timeval before_now, now;

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
