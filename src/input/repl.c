/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:40:21 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/12 15:48:15 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void){
    while(1){
        char *line = readline("myshell> ");
        if (!line) {
            printf("%s", "bye!");
            break;
        }
    }
    return (0);
}
