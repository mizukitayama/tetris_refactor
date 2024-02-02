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

void turn_block(t_block block)
/* ブロックを右に回転する */
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

void move_block(int c)
{
	t_block temp = malloc_copy_block(current_block);
	switch (c)
	{
	case DOWN_KEY:
		// 下に移動
		temp.row++;
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
