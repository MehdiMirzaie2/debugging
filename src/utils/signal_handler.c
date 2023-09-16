/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:48:35 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/11 15:35:03 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void handle_sigint(int sig)
{
    if (sig == SIGINT)
    {
        (void)sig;
        write(2, "\n", 1);
        // rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
    else if (sig == SIGQUIT)
        g_value = SIGQUIT;
}
