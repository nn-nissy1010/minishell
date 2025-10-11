/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:56:47 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/06/01 15:55:45 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <limits.h>
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

# define HEX_UP_BASE "0123456789ABCDEF"
# define HEX_LOW_BASE "0123456789abcdef"

char	*ft_uitoa(unsigned int n);

int		ft_printf(const char *format, ...);

int		ft_put_char_fd(char c, int fd);
int		ft_put_str_fd(char *str, int fd);
int		ft_put_int_fd(int nbr, int fd);
int		ft_put_unsigned_fd(unsigned int nbr, int fd);
int		ft_put_ptr_fd(void *ptr, int fd);
int		ft_put_hex_fd(unsigned int num, int fd, const char *base);

#endif
