# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 10:22:11 by tkuwahat          #+#    #+#              #
#    Updated: 2025/09/06 12:25:46 by tkuwahat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	=libft.a

SRCS 	=ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_memset.c \
		ft_strlen.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
		ft_tolower.c ft_toupper.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c \
		ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c\
		ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
		ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c\
		ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c \
		ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c\
		ft_lstmap_bonus.c ft_strspn.c ft_atol.c\
		get_next_line_utils.c get_next_line.c \
		ft_printf.c ft_put_alnum.c ft_put_hex.c ft_put_ptr.c ft_uitoa.c \
		ft_strcspn.c ft_strcmp.c

OBJS	=$(SRCS:.c=.o)
CC	= cc
CFLAGS	=-Wall -Wextra -Werror
CPPFLAGS = -I.
RM =rm -f


all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all
