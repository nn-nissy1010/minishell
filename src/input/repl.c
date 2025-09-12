/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:40:21 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/12 16:23:58 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int ft_isspace(unsigned char c)
{
    if (c == 32 || (c >= 9 && c <= 13))
        return 1;
    return 0;
}

int is_blank_line(const char *s) {
    while (*s) {
        if (!ft_isspace((unsigned char)*s)) {
            return 0;
        }
        s++;
    }
    return 1;
}

int main(void){
    while(1){
        char *line = readline("myshell> ");
        if (!line) {
            printf("%s", "bye!");
            break;
        }
        if (is_blank_line(line)) {
            free(line);
            continue;         
        }
        printf("入力: %s(ここで本来はコマンド処理する)\n", line);
        free(line);
    }
    return (0);
}
