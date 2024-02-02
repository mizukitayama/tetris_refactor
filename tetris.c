#include "tetris.h"

char Table[Row][Col] = {0};
int final_score = 0;
char GameOn = true;
suseconds_t timer = 400000;
int decrease = 1000;
t_block current;

const t_block block_types[7] = {
		{(char *[]){(char[]){0, 1, 1}, (char[]){1, 1, 0}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){1, 1, 0}, (char[]){0, 1, 1}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){0, 1, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){0, 0, 1}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){1, 0, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){1, 1}, (char[]){1, 1}}, 2},
		{(char *[]){(char[]){0, 0, 0, 0}, (char[]){1, 1, 1, 1}, (char[]){0, 0, 0, 0}, (char[]){0, 0, 0, 0}}, 4}};

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
	{
		free(block.array[i]);
	}
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
			//
			if ((block.col + j < 0 || block.col + j >= Col || block.row + i >= Row))
			{
				if (array[i][j])
					return false;
			}
			else if (Table[block.row + i][block.col + j] && array[i][j])
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
	char Buffer[Row][Col] = {0};
	int i, j;
	for (i = 0; i < current.width; i++)
	{
		for (j = 0; j < current.width; j++)
		{
			if (current.array[i][j])
				Buffer[current.row + i][current.col + j] = current.array[i][j];
		}
	}
	clear();
	for (i = 0; i < Col - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
	for (i = 0; i < Row; i++)
	{
		for (j = 0; j < Col; j++)
		{
			printw("%c ", (Table[i][j] + Buffer[i][j]) ? '#' : '.');
		}
		printw("\n");
	}
	printw("\nScore: %d\n", final_score);
}

struct timeval before_now, now;
int hasToUpdate()
{
	return ((suseconds_t)(now.tv_sec * 1000000 + now.tv_usec) - ((suseconds_t)before_now.tv_sec * 1000000 + before_now.tv_usec)) > timer;
}

void set_timeout(int time)
{
	time = 1;
	timeout(1);
}

int main()
{
	srand(time(0));
	final_score = 0;
	int c;
	initscr();
	gettimeofday(&before_now, NULL);
	set_timeout(1);
	t_block new_block = malloc_copy_block(block_types[rand() % 7]);
	new_block.col = rand() % (Col - new_block.width + 1);
	new_block.row = 0;
	free_block(current);
	current = new_block;
	if (!is_block_placeable(current))
	{
		GameOn = false;
	}
	draw_map();
	while (GameOn)
	{
		if ((c = getch()) != ERR)
		{
			t_block temp = malloc_copy_block(current);
			switch (c)
			{
			case 's':
				temp.row++; // move down
				if (is_block_placeable(temp))
					current.row++;
				else
				{
					int i, j;
					for (i = 0; i < current.width; i++)
					{
						for (j = 0; j < current.width; j++)
						{
							if (current.array[i][j])
								Table[current.row + i][current.col + j] = current.array[i][j];
						}
					}
					int n, m, sum, count = 0;
					// TODO: おいたところだけの判定で良い ✅
					for (n = current.row; n < current.row + current.width; n++)
					{
						sum = 0;
						for (m = 0; m < Col; m++)
						{
							sum += Table[n][m];
						}
						if (sum == Col)
						{
							// decrease level of map
							count++;
							int l, k;
							for (k = n; k >= 1; k--)
								for (l = 0; l < Col; l++)
									Table[k][l] = Table[k - 1][l];
							for (l = 0; l < Col; l++)
								Table[k][l] = 0;
							timer -= decrease--;
						}
					}
					final_score += 100 * count;
					t_block new_block = malloc_copy_block(block_types[rand() % 7]);
					new_block.col = rand() % (Col - new_block.width + 1);
					new_block.row = 0;
					free_block(current);
					current = new_block;
					if (!is_block_placeable(current))
					{
						GameOn = false;
					}
				}
				break;
			case 'd':
				// to right
				temp.col++;
				if (is_block_placeable(temp))
					current.col++;
				break;
			case 'a':
				// to left
				temp.col--;
				if (is_block_placeable(temp))
					current.col--;
				break;
			case 'w':
				// turn
				turn_block(temp);
				if (is_block_placeable(temp))
					turn_block(current);
				break;
			}
			free_block(temp);
			draw_map();
		}
		gettimeofday(&now, NULL);
		if (hasToUpdate())
		{
			t_block temp = malloc_copy_block(current);
			switch ('s')
			{
			case 's':
				temp.row++;
				if (is_block_placeable(temp))
					current.row++;
				else
				{
					int i, j;
					for (i = 0; i < current.width; i++)
					{
						for (j = 0; j < current.width; j++)
						{
							if (current.array[i][j])
								Table[current.row + i][current.col + j] = current.array[i][j];
						}
					}
					int n, m, sum, count = 0;
					for (n = 0; n < Row; n++)
					{
						sum = 0;
						for (m = 0; m < Col; m++)
						{
							sum += Table[n][m];
						}
						if (sum == Col)
						{
							count++;
							int l, k;
							for (k = n; k >= 1; k--)
								for (l = 0; l < Col; l++)
									Table[k][l] = Table[k - 1][l];
							for (l = 0; l < Col; l++)
								Table[k][l] = 0;
							timer -= decrease--;
						}
					}
					t_block new_block = malloc_copy_block(block_types[rand() % 7]);
					new_block.col = rand() % (Col - new_block.width + 1);
					new_block.row = 0;
					free_block(current);
					current = new_block;
					if (!is_block_placeable(current))
					{
						GameOn = false;
					}
				}
				break;
			case 'd':
				temp.col++;
				if (is_block_placeable(temp))
					current.col++;
				break;
			case 'a':
				temp.col--;
				if (is_block_placeable(temp))
					current.col--;
				break;
			case 'w':
				turn_block(temp);
				if (is_block_placeable(temp))
					turn_block(current);
				break;
			}
			free_block(temp);
			draw_map();
			gettimeofday(&before_now, NULL);
		}
	}
	free_block(current);
	endwin();
	int i, j;
	for (i = 0; i < Row; i++)
	{
		for (j = 0; j < Col; j++)
		{
			printf("%c ", Table[i][j] ? '#' : '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", final_score);
	return 0;
}