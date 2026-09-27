NAME = woody_woodpacker
STUB = asm/stub.bin

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g

SRC_DIR  = src
INCLUDES = -Iincludes

SRC = $(SRC_DIR)/find.c \
 	  $(SRC_DIR)/utils.c \
	  $(SRC_DIR)/crypto.c \
	  $(SRC_DIR)/patcher.c \
	  $(SRC_DIR)/main.c

OBJ = $(SRC:.c=.o)

all: $(STUB) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(INCLUDES) -o $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(STUB): asm/stub.s
		nasm -f bin asm/stub.s -o $(STUB)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -f woody
	rm -f $(STUB)

re: fclean all

.PHONY: all clean fclean re