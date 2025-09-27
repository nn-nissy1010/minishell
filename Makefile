# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/12 17:10:51 by nnishiya          #+#    #+#              #
#    Updated: 2025/09/26 17:12:46 by nnishiya         ###   ########.fr        #
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
	  src/laxer/token.c \
	  src/laxer/buffer.c \
	  src/laxer/parts/parts_manage.c \
	  src/laxer/parts/parts_join.c \
	  src/laxer/scan/scan_quote.c \
	  src/laxer/scan/scan_quote_utils.c \
	  src/laxer/scan/scan_unquote.c \
	  src/laxer/scan/scan_symbol_utils.c \
	  src/parser/parser.c \
	  src/parser/parser_utils.c \
	  src/parser/parse_x.c \
	  src/parser/parse_redir.c \
	  src/parser/operate_ast.c \
	  src/exec/exec_ast.c \
	  src/exec/expand/expand.c \
	  src/exec/expand/expand_arg.c \
	  src/exec/expand/arg_buf_utils/arg_buf_build.c \
	  src/exec/expand/arg_buf_utils/arg_buf_clean.c \
  	  src/exec/expand/expand_part/expand_parts_tilde.c \
	  src/exec/expand/expand_part/expand_parts_params.c  \
	  src/exec/expand/expand_part/expand_parts.c  \
	  src/exec/expand/part_buf_utils/expand_buf_clean.c \
  	  src/exec/expand/part_buf_utils/expand_buf_emit.c \
  	  src/exec/expand/part_buf_utils/expand_buf_put_q.c \
  	  src/exec/expand/part_buf_utils/expand_buf_put.c \
	  src/exec/expand/part_buf_utils/expand_buf_reserve.c\
	  src/exec/expand/part_buf_utils/expand_buf_take.c \
	  src/exec/expand/word_split/expand_word_split_utils.c \
	  src/exec/expand/word_split/expand_word_split.c \

	




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
