#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
// #include <bool.h>

#define Row 20
#define Col 10
#define True 1
#define False 0

char Table[Row][Col] = {0};
int final_score = 0;
char GameOn = True;
suseconds_t timer = 400000;
int decrease = 1000;

typedef struct
{
	char **array;
	int width, row, col;
} Struct;

Struct current;

const Struct shape_types[7] = {
		{(char *[]){(char[]){0, 1, 1}, (char[]){1, 1, 0}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){1, 1, 0}, (char[]){0, 1, 1}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){0, 1, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){0, 0, 1}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){1, 0, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},
		{(char *[]){(char[]){1, 1}, (char[]){1, 1}}, 2},
		{(char *[]){(char[]){0, 0, 0, 0}, (char[]){1, 1, 1, 1}, (char[]){0, 0, 0, 0}, (char[]){0, 0, 0, 0}}, 4}};

Struct malloc_copy_shape(Struct shape)
{
	Struct new_shape = shape;
	char **copyshape = shape.array;
	new_shape.array = (char **)malloc(new_shape.width * sizeof(char *));
	int i, j;
	for (i = 0; i < new_shape.width; i++)
	{
		new_shape.array[i] = (char *)malloc(new_shape.width * sizeof(char));
		for (j = 0; j < new_shape.width; j++)
		{
			new_shape.array[i][j] = copyshape[i][j];
		}
	}
	return new_shape;
}

void free_shape(Struct shape)
{
	int i;
	for (i = 0; i < shape.width; i++)
	{
		free(shape.array[i]);
	}
	free(shape.array);
}

int is_shape_placeable(Struct shape)
{
	char **array = shape.array;
	int i, j;
	for (i = 0; i < shape.width; i++)
	{
		for (j = 0; j < shape.width; j++)
		{
			//
			if ((shape.col + j < 0 || shape.col + j >= Col || shape.row + i >= Row))
			{
				if (array[i][j])
					return False;
			}
			else if (Table[shape.row + i][shape.col + j] && array[i][j])
				return False;
		}
	}
	return True;
}

void to_left(Struct shape) // TODO：関数の名前
{
	Struct temp = malloc_copy_shape(shape);
	int i, j, width;

	width = shape.width;
	bool is_left_blank = true;
	while (is_left_blank)
	{
		for (i = 0; i < width; i++)
		{
			if (shape.array[i][0])
				is_left_blank = false;
		}
		if (is_left_blank)
		{
			for (i = 0; i < width; i++)
				for (j = 0; j < width; j++)
				{
					if (j == width - 1)
						shape.array[i][j] = 0;
					else
						shape.array[i][j] = temp.array[i][j + 1];
				}
		}
		free_shape(temp);
		temp = malloc_copy_shape(shape);
	}
}

void turn_shape(Struct shape)
{
	Struct temp = malloc_copy_shape(shape);
	int i, j, k, width;
	width = shape.width;

	// TODO: 回し方を考える。今のだと、マップの端で回した時に回らない形があったり、回り方が不自然なところがある。
	for (i = 0; i < width; i++)
	{
		for (j = 0, k = width - 1; j < width; j++, k--)
		{
			shape.array[i][j] = temp.array[k][i];
		}
	}
	free_shape(temp);
	to_left(shape);
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
	Struct new_shape = malloc_copy_shape(shape_types[rand() % 7]);
	new_shape.col = rand() % (Col - new_shape.width + 1);
	new_shape.row = 0;
	free_shape(current);
	current = new_shape;
	if (!is_shape_placeable(current))
	{
		GameOn = False;
	}
	draw_map();
	while (GameOn)
	{
		if ((c = getch()) != ERR)
		{
			Struct temp = malloc_copy_shape(current);
			switch (c)
			{
			case 's':
				temp.row++; // move down
				if (is_shape_placeable(temp))
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
					Struct new_shape = malloc_copy_shape(shape_types[rand() % 7]);
					new_shape.col = rand() % (Col - new_shape.width + 1);
					new_shape.row = 0;
					free_shape(current);
					current = new_shape;
					if (!is_shape_placeable(current))
					{
						GameOn = False;
					}
				}
				break;
			case 'd':
				// to right
				temp.col++;
				if (is_shape_placeable(temp))
					current.col++;
				break;
			case 'a':
				// to left
				temp.col--;
				if (is_shape_placeable(temp))
					current.col--;
				break;
			case 'w':
				// turn
				turn_shape(temp);
				if (is_shape_placeable(temp))
					turn_shape(current);
				break;
			}
			free_shape(temp);
			draw_map();
		}
		gettimeofday(&now, NULL);
		if (hasToUpdate())
		{
			Struct temp = malloc_copy_shape(current);
			switch ('s')
			{
			case 's':
				temp.row++;
				if (is_shape_placeable(temp))
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
					Struct new_shape = malloc_copy_shape(shape_types[rand() % 7]);
					new_shape.col = rand() % (Col - new_shape.width + 1);
					new_shape.row = 0;
					free_shape(current);
					current = new_shape;
					if (!is_shape_placeable(current))
					{
						GameOn = False;
					}
				}
				break;
			case 'd':
				temp.col++;
				if (is_shape_placeable(temp))
					current.col++;
				break;
			case 'a':
				temp.col--;
				if (is_shape_placeable(temp))
					current.col--;
				break;
			case 'w':
				turn_shape(temp);
				if (is_shape_placeable(temp))
					turn_shape(current);
				break;
			}
			free_shape(temp);
			draw_map();
			gettimeofday(&before_now, NULL);
		}
	}
	free_shape(current);
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
