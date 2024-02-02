#	ifndef TETRIS_H
# define TETRIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <stdbool.h>


#define ROW 				20
#define COL					8
#define SCORE				100
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


const t_block block_types[BLOCK_NUMS];
extern char game_table[ROW][COL];
extern int final_score;
extern bool game_on;
extern suseconds_t auto_drop_timer;
extern int timer_decrease;
extern t_block current_block;
extern struct timeval before_now, now;

/* block_opoerations.c */
t_block malloc_copy_block(t_block block);
void free_block(t_block block);
void turn_block(t_block block);
void move_block(int c);

/* drawing.c */
void draw_map();

/* game_logic.c */
int is_block_placeable(t_block block);
void check_and_clear_full_rows();
int is_time_for_auto_drop();
void set_timeout(int time);
t_block pick_new_block();


#	endif
