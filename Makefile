# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: denizozd <denizozd@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/21 20:35:04 by denizozd          #+#    #+#              #
#    Updated: 2024/09/16 20:03:05 by denizozd         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

G = "\033[32m"
Y = "\033[33m"
B = "\033[34m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra
LIBFTPATH = ./libft
LIBFT = $(LIBFTPATH)/libft.a
CPPFLAGS	= -I/Users/$(USER)/.brew/opt/readline/include

SRCDIR = src
OBJDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(LIBFT) $(NAME)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo $(Y)Compiling $<$(X)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@printf $(UP)$(CUT)

$(LIBFT):
	@if [ ! -d $(LIBFTPATH) ]; then \
		git submodule update --init --recursive; \
	fi

	@if [ ! -f $(LIBFTPATH)/libft.a ]; then \
		make -s -C $(LIBFTPATH); \
	fi
	@echo $(G)"Created		[libft]"$(X)

$(NAME): $(LIBFT) $(OBJS)
	@echo $(Y)"Compiling	"[$(SRCS)]$(X)
	@echo $(G)"Created		"[$(SRCS)]$(X)
	@echo $(Y)"Compiling	"[$(NAME)]$(X)
	@$(CC) $(OBJS) $(LDFLAGS) $(CPPFLAGS) -lreadline -L$(LIBFTPATH) -lft -o $(NAME)
	@echo $(G)"Created		"[$(NAME)]$(X)

clean:
	@echo $(B)"Removing	objectfiles"$(X)
	@rm -rf $(OBJDIR)
	@echo $(G)"Removed		objectfiles"$(X)

fclean: clean
	@echo $(B)"Removing	executable"$(X)
	@rm -f $(NAME)
	@echo $(G)"Removed		executable"$(X)
	@echo $(B)"Removing	library"$(X)
	@cd $(LIBFTPATH) && make fclean > /dev/null
	@echo $(G)"Removed		library"$(X)

re: fclean all

.PHONY: all clean fclean re
