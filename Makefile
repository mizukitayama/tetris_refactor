CC      = cc
RM      = rm -rf
MKDIR		= mkdir -p

CCFLAGS = -Wall -Werror -Wextra

SRCS    = tetris.c globals.c block_operations.c drawing.c game_logic.c

NAME    = tetris

OBJ_DIR = obj/
SRC_DIR = src/

SRCS_OBJS = $(SRCS:.c=.o)
OBJS      = $(addprefix $(OBJ_DIR), $(SRCS_OBJS))

INCLUDE = -I inc/
NCURSES	= -lncurses

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(INCLUDE) -o $(NAME) $(OBJS) $(NCURSES)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(INCLUDE) -o $@ -c $^

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)
