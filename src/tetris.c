#include "tetris.h"

void end_game()
{
	endwin();
	size_t i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			printf("%c ", game_table[i][j] ? BLOCK : BLANK);
		}
		printf("\n");
	}
	printf(END_MESSAGE);
	printf("\nScore: %d\n", final_score);
}

int main()
{
	srand(time(0));
	int c;
	initscr();
	gettimeofday(&before_now, NULL);
	set_timeout(1);

	current_block = pick_new_block();
	if (!is_block_placeable(current_block))
	{
		game_on = false;
	}
	draw_map();
	while (game_on)
	{
		// キーボードでブロックを動かす
		if ((c = getch()) != ERR)
			move_block(c);

		// 最後に動かした時間から0.4秒経過したら、ブロックを下に動かす
		gettimeofday(&now, NULL);
		if (is_time_for_auto_drop())
		{
			move_block(DOWN_KEY);
			gettimeofday(&before_now, NULL);
		}
	}
	free_block(current_block);
	end_game();
	return 0;
}
