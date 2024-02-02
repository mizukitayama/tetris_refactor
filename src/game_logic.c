#include "tetris.h"

t_block pick_new_block()
/* ブロックをランダムで選択、開始位置を設定 */
{
	t_block new_block = malloc_copy_block(block_types[rand() % BLOCK_NUMS]);
	new_block.col = rand() % (COL - new_block.width + 1);
	new_block.row = 0;
	free_block(current_block);
	return new_block;
}

int is_block_placeable(t_block block)
/* ブロックが枠外に置かれないか、すでに置かれている場所では無いかをみる */
{
	char **array = block.array;
	int i, j;
	for (i = 0; i < block.width; i++)
	{
		for (j = 0; j < block.width; j++)
		{
			// ブロックが枠外にあるとき
			if (array[i][j] && (block.col + j < 0 || block.col + j >= COL || block.row + i >= ROW))
				return false;
			// ブロックがすでに置かれているとき
			else if (array[i][j] && game_table[block.row + i][block.col + j])
				return false;
		}
	}
	return true;
}

void check_and_clear_full_rows()
{
	int n, m, sum, count = 0;
	// ブロックを置いた行が消えるかの判定
	for (n = current_block.row; n < current_block.row + current_block.width; n++)
	{
		sum = 0;
		for (m = 0; m < COL; m++)
		{
			sum += game_table[n][m];
		}
		if (sum == COL)
		{
			// 行を消してマップを更新
			count++;
			int l, k;
			for (k = n; k >= 1; k--)
				for (l = 0; l < COL; l++)
					game_table[k][l] = game_table[k - 1][l];
			for (l = 0; l < COL; l++)
				game_table[k][l] = 0;
			auto_drop_timer -= timer_decrease--;
		}
	}
	final_score += SCORE * count;
	current_block = pick_new_block();
	if (!is_block_placeable(current_block))
	{
		game_on = false;
	}
}

int is_time_for_auto_drop()
/* before_nowから0.4秒以上経っているかを判定 */
{
	return ((suseconds_t)(now.tv_sec * 1000000 + now.tv_usec) - ((suseconds_t)before_now.tv_sec * 1000000 + before_now.tv_usec)) > auto_drop_timer;
}

void set_timeout(int time)
{
	timeout(time);
}
