
NAME = minishell

SRCS =src/main.c


OBJS = $(SRCS:.c=.o)


LIBFT_DIR =libft
LIBFT_LIB =$(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -Wall -Werror -Wextra
CPPFLAGS = -I . -I $(LIBFT_DIR)

RM = rm -f

all : $(NAME)

$(LIBFT_LIB):
		$(MAKE) -C $(LIBFT_DIR)

$(NAME):$(OBJS) $(LIBFT_LIB)
		$(CC) $(CFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB)

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
