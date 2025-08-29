NAME = minishell

CFLAGS = -Wall -Wextra -Werror -g

SRC =	minishell.c \
		execute/builtins.c \
		builtin_commands/manuel_cd.c \
		builtin_commands/manuel_echo.c \
		builtin_commands/manuel_env.c \
		builtin_commands/manuel_exit.c \
		builtin_commands/manuel_export.c \
		builtin_commands/manuel_pwd.c \
		builtin_commands/manuel_unset.c \
		env_functions/env_utils.c \
		execute/exec.c \
		execute/exec_utils.c \
		exit/exit_minishell.c \
		expander/expand_var_utils.c \
		expander/expander.c \
		garbage/garbage_collector.c \
		garbage/garbage_lib.c \
		garbage/grbg_itoa.c \
		redirections/handle_redirections.c \
		lexer/handle_spaces.c \
		lexer/handle_spaces_utils.c \
		redirections/heredoc.c \
		utils/init.c \
		lexer/lexer.c \
		list_functions/list_functions.c \
		list_functions/list_utils.c \
		parser/parser.c \
		parser/parser_utils.c \
		lexer/signal_handler.c \
		utils/split_input.c \
		utils/utils.c \
		utils/error_utils.c \
		utils/quotes_utils.c \
		utils/str_utils.c

OBJ =	$(SRC:.c=.o)

LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(SRC)
	make -C ./libft -s
	cc -o $(NAME) $(CFLAGS) $(SRC) -lreadline $(LIBFT)

clean:
	make clean -C ./libft -s

fclean: clean
	make fclean -C ./libft -s
	rm -rf $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re