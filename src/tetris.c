#include "tetris.h"

t_block malloc_copy_block(t_block block)
{
	t_block new_block = block;
	char **copyblock = block.array;
	new_block.array = (char **)malloc(new_block.width * sizeof(char *));
	int i, j;
	for (i = 0; i < new_block.width; i++)
	{
		new_block.array[i] = (char *)malloc(new_block.width * sizeof(char));
		for (j = 0; j < new_block.width; j++)
		{
			new_block.array[i][j] = copyblock[i][j];
		}
	}
	return new_block;
}

void free_block(t_block block)
{
	int i;
	for (i = 0; i < block.width; i++)
		free(block.array[i]);
	free(block.array);
}

int is_block_placeable(t_block block)
{
	char **array = block.array;
	int i, j;
	for (i = 0; i < block.width; i++)
	{
		for (j = 0; j < block.width; j++)
		{
			if ((block.col + j < 0 || block.col + j >= COL || block.row + i >= ROW))
			{
				if (array[i][j])
					return false;
			}
			else if (game_table[block.row + i][block.col + j] && array[i][j])
				return false;
		}
	}
	return true;
}

void turn_block(t_block block)
{
	t_block temp = malloc_copy_block(block);
	int i, j, k, width;
	width = block.width;

	for (i = 0; i < width; i++)
	{
		for (j = 0, k = width - 1; j < width; j++, k--)
		{
			block.array[i][j] = temp.array[k][i];
		}
	}
	free_block(temp);
}

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

struct timeval before_now, now;
int hasToUpdate()
{
	return ((suseconds_t)(now.tv_sec * 1000000 + now.tv_usec) - ((suseconds_t)before_now.tv_sec * 1000000 + before_now.tv_usec)) > auto_drop_timer;
}

void set_timeout(int time)
{
	timeout(time);
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
	t_block new_block = malloc_copy_block(block_types[rand() % BLOCK_NUMS]);
	new_block.col = rand() % (COL - new_block.width + 1);
	new_block.row = 0;
	free_block(current_block);
	current_block = new_block;
	if (!is_block_placeable(current_block))
	{
		game_on = false;
	}
}

void move_block(int c)
{
	t_block temp = malloc_copy_block(current_block);
	switch (c)
	{
	case DOWN_KEY:
		temp.row++; // 下に移動
		if (is_block_placeable(temp))
			current_block.row++;
		else
		{
			int i, j;
			for (i = 0; i < current_block.width; i++)
			{
				for (j = 0; j < current_block.width; j++)
				{
					if (current_block.array[i][j])
						game_table[current_block.row + i][current_block.col + j] = current_block.array[i][j];
				}
			}
			check_and_clear_full_rows();
		}
		break;
	case RIGHT_KEY:
		// 右に移動
		temp.col++;
		if (is_block_placeable(temp))
			current_block.col++;
		break;
	case LEFT_KEY:
		// 左に移動
		temp.col--;
		if (is_block_placeable(temp))
			current_block.col--;
		break;
	case TURN_KEY:
		// 回転
		turn_block(temp);
		if (is_block_placeable(temp))
			turn_block(current_block);
		break;
	}
	free_block(temp);
	draw_map();
}

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

	// ブロックをランダムで選択、開始位置を設定
	t_block new_block = malloc_copy_block(block_types[rand() % BLOCK_NUMS]);
	new_block.col = rand() % (COL - new_block.width + 1);
	new_block.row = 0;

	current_block = new_block;
	if (!is_block_placeable(current_block))
	{
		game_on = false;
	}
	draw_map();
	while (game_on)
	{
		if ((c = getch()) != ERR)
			move_block(c);
		gettimeofday(&now, NULL);
		// 最後に動かした時間から0.4秒経過したら、ブロックを下に動かす
		if (hasToUpdate())
		{
			move_block(DOWN_KEY);
			gettimeofday(&before_now, NULL);
		}
	}
	free_block(current_block);
	end_game();
	return 0;
}
