#include "tetris.h"

void draw_map()
{
	char block_on_map[ROW][COL] = {0};
	int i, j;
	for (i = 0; i < current_block.width; i++)
	{
		for (j = 0; j < current_block.width; j++)
		{
			if (current_block.array[i][j])
				block_on_map[current_block.row + i][current_block.col + j] = current_block.array[i][j];
		}
	}
	clear();
	for (i = 0; i < COL - 9; i++)
		printw(" ");
	printw(WINDOW_NAME);
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			printw("%c ", (game_table[i][j] + block_on_map[i][j]) ? BLOCK : BLANK);
		}
		printw("\n");
	}
	printw("\nScore: %d\n", final_score);
}
