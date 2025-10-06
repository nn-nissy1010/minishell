# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/12 17:10:51 by nnishiya          #+#    #+#              #
#    Updated: 2025/10/06 17:24:57 by tkuwahat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS =src/main.c \
      src/exit_status.c \
	  src/utils/utils.c \
      src/input/repl.c \
	  src/input/utils/read_command.c \
	  src/input/utils/read_command_utils.c \
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
	  src/env_table/builtin/env_table_builtin_utils3.c \
	  src/laxer/laxer.c \
	  src/laxer/token.c \
	  src/laxer/operate_token.c \
	  src/laxer/buffer.c \
	  src/laxer/parts/parts_manage.c \
	  src/laxer/parts/parts_join.c \
	  src/laxer/scan/scan_quote.c \
	  src/laxer/scan/scan_quote_utils.c \
	  src/laxer/scan/scan_unquote.c \
	  src/laxer/scan/scan_symbol_utils.c \
	  src/laxer/scan/scan_param.c \
	  src/parser/parser.c \
	  src/parser/parser_utils.c \
	  src/parser/parse_x.c \
	  src/parser/parse_redir.c \
	  src/parser/operate_ast.c \
	  src/exec/exec_ast.c \
	  src/exec/exec_cmd/exec_cmd_check_fd.c \
	  src/exec/exec_cmd/exec_cmd_single.c \
	  src/exec/exec_cmd/exec_cmd.c \
	  src/exec/exec_cmd/exec_cmd_destroy.c \
	  src/exec/exec_cmd/exec_cmd_destroy_main.c \
	  src/exec/exec_cmd/builtin_cmd/exec_cmd_bi_cd.c \
	  src/exec/exec_cmd/builtin_cmd/exec_cmd_bi_echo.c \
	  src/exec/exec_cmd/builtin_cmd/exec_cmd_bi_env.c \
	  src/exec/exec_cmd/builtin_cmd/exec_cmd_bi_exit.c \
	  src/exec/exec_cmd/builtin_cmd/exec_cmd_bi_export.c \
	  src/exec/exec_cmd/builtin_cmd/exec_cmd_bi_pwd.c \
	  src/exec/exec_cmd/builtin_cmd/exec_cmd_bi_unset.c \
	  src/exec/exec_cmd/builtin_cmd/exec_cmd_bi_utils.c \
	  src/exec/exec_cmd/child/exec_cmd_child_direct.c \
	  src/exec/exec_cmd/child/exec_cmd_child_external.c \
	  src/exec/exec_cmd/child/exec_cmd_child_main.c \
	  src/exec/exec_cmd/child/exec_cmd_child_search_utils.c \
	  src/exec/exec_cmd/child/exec_cmd_child_search.c \
	  src/exec/exec_cmd/child/exec_cmd_child.c \
	  src/exec/exec_cmd/child/exec_error.c \
	  src/exec/exec_cmd/parent/exec_cmd_parent_main.c \
	  src/exec/exec_cmd/redirect/exec_cmd_backup_fd.c \
	  src/exec/exec_cmd/redirect/exec_cmd_redir_main.c \
	  src/exec/exec_cmd/redirect/exec_cmd_redir_one.c \
	  src/exec/exec_pipe/exec_pipe_call_child.c \
	  src/exec/exec_pipe/exec_pipe_destroy.c \
	  src/exec/exec_pipe/exec_pipe_flow_child.c \
	  src/exec/exec_pipe/exec_pipe_main.c \
	  src/exec/exec_pipe/exec_pipe_signal_manage.c \
 	  src/exec/expand/expand.c \
	  src/exec/expand/expand_arg.c \
	  src/exec/expand/expand_redir_frow.c \
	  src/exec/expand/arg_buf_utils/arg_buf_build.c \
	  src/exec/expand/arg_buf_utils/arg_buf_grow_buf.c \
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
	  src/exec/expand/heredoc/heredoc_main.c \
	  src/exec/expand/heredoc/heredoc_build_delim.c \
	  src/exec/expand/heredoc/heredoc_rl_expand.c \
	  src/exec/expand/heredoc/heredoc_signal.c \
	  src/exec/expand/heredoc/heredoc_signal_utils.c \
	  src/exec/expand/wildcards/wildcards_sort.c \
  	  src/exec/expand/wildcards/wildcards_utils.c \
	  src/exec/expand/wildcards/wildcards.c \
	  src/exec/expand/wildcards/wildcards_dir.c \

OBJS        = $(SRCS:.c=.o)

# --- libft ---
LIBFT_DIR   = libft
LIBFT_LIB   = $(LIBFT_DIR)/libft.a

# --- compiler ---
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
CPPFLAGS    = -I includes -I . -I $(LIBFT_DIR)
LDFLAGS     = -lreadline -lhistory

RM          = rm -f

all: $(NAME)

# build libft first
$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR)

# build minishell
$(NAME): $(LIBFT_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB) $(LDFLAGS)

# compile .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# clean
clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(RM) $(OBJS)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
