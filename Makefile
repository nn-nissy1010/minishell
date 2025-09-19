# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/12 17:10:51 by nnishiya          #+#    #+#              #
#    Updated: 2025/09/19 16:11:34 by nnishiya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS =src/main.c \
      src/exit_status.c \
	  src/utils/utils.c \
      src/input/repl.c \
	  src/input/utils/read_command.c \
	  src/input/utils/signal.c \
	  src/input/utils/syntax_check.c \
	  src/input/utils/syntax_check_utils.c \
  	  src/env_table/env_table.c \
	  src/env_table/env_table_init.c \
      src/env_table/env_table_set.c \
	  src/env_table/env_table_destroy.c \
	  src/env_table/utils/env_table_core.c \
	  src/env_table/utils/env_table_parse.c \
	  src/env_table/utils/env_table_find.c \
	  src/env_table/builtin/env_table_builtin.c \
	  src/env_table/builtin/env_table_builtin_utils.c \
  	  src/env_table/builtin/env_table_builtin_utils2.c \
	  src/laxer/laxer.c \



OBJS = $(SRCS:.c=.o)

LIBFT_DIR =libft
LIBFT_LIB =$(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -Wall -Werror -Wextra
CPPFLAGS = -I includes -I . -I $(LIBFT_DIR)
LDFLAGS = -lreadline -lhistory

RM = rm -f

all : $(NAME)

$(LIBFT_LIB):
		$(MAKE) -C $(LIBFT_DIR)

$(NAME):$(OBJS) $(LIBFT_LIB)
		$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB)

%.o:%.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(RM) $(OBJS)

fclean:clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)

re:fclean all

.PHONY: all clean fclean re
