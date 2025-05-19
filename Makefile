NAME = philosophers

CC = cc
CFLAGS = -fsanitize=address -Wall -Wextra -Werror 
SRC = main.c arg_checks.c utils.c loop.c control.c
      
OBJ = $(SRC:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

all: $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re