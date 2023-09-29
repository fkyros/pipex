#colors----------------------------------------------------

RED = '\033[1;31m'
GREEN   = '\033[1;32m'
BLUE    = '\033[1;34m'
RESET   = '\033[0;0m'

#variables-------------------------------------------------

NAME = pipex

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

HEADER = inc/pipex.h

SRC = src/pipex.c \
	  src/main.c \
	  src/envp.c \
	  src/utils/free_command.c \
	  src/error_handling/ft_command_error.c \
	  src/error_handling/ft_error.c \
	  src/error_handling/ft_file_error.c \
	  src/funcs_with_error_check/pipe_with_error_check.c \
	  src/funcs_with_error_check/fork_with_error_check.c

OBJ = $(SRC:src/%.c=bin/%.o)

#libft----------------------------------------------------

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a
LIBFT_HEADER = $(LIBFT_DIR)libft.h

#gnl------------------------------------------------------


#recipes--------------------------------------------------

all:	$(NAME)

$(LIBFT):	$(LIBFT_HEADER)
	@echo $(BLUE)"compiling libft..."$(RESET)
	@make --directory=$(LIBFT_DIR)

bin/%.o: src/%.c $(HEADER)
	@echo $(BLUE)"creating binaries..."$(RESET)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(LIBFT) $(OBJ)
	@echo $(BLUE)"compiling $(NAME)..."$(RESET)
	$(CC) $(CFLAGS) -D BONUS=0 $(OBJ) $(LIBFT) -o $(NAME)
	@echo $(GREEN)"$(NAME) compiled!"$(RESET)

bonus:	.bonus

.bonus:	$(LIBFT) $(OBJ)
	@echo $(BLUE)"compiling $(NAME) with bonus..."$(RESET)
	$(CC) $(CFLAGS) -D BONUS=1 $(OBJ) $(LIBFT) -o $(NAME)
	@echo $(GREEN)"$(NAME) bonus compiled!"$(RESET)
	@touch .bonus

clean:
	@echo $(RED)"deleting binaries..."$(RESET)
	@make clean --directory=$(LIBFT_DIR)
	rm -rf bin/
	@rm -f .bonus

fclean:	clean
	rm -f $(LIBFT)
	@echo $(RED)"libft deleted!"$(RESET)
	rm -f $(NAME)
	@echo $(RED)"$(NAME) deleted!"$(RESET)

re:	fclean all

.PHONY: all clean fclean re bonus
