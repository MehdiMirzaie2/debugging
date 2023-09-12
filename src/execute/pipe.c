/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 22:30:44 by mehdimirzai       #+#    #+#             */
/*   Updated: 2023/09/11 22:34:34 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	open_pipes(int process_num, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < process_num + 1)
	{
		if (pipe(pipes[i++]) == -1)
		{
			printf("Error with creating pipe\n");
			return (1);
		}
	}
	return (0);
}

int	create_middle_process(int process_num, int *pids, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < process_num)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			printf("Error with creating process\n");
			return (2);
		}
		if (pids[i] == 0)
			middle_child(args, pipes, i);
		i++;
	}
	return (0);
}

void	middle_child(const t_args args, int pipes[][2], int i)
{
	int	j;

	j = 0;
	while (j < *(args.process_num) + 1)
	{
		if (i != j)
			close(pipes[j][0]);
		if (i + 1 != j)
			close(pipes[j][1]);
		j++;
	}
	if ((redirect_input(pipes[i][0], STDIN_FILENO) == -1)
		|| redirect_output(pipes[i + 1][1], STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	pipex(args.av[i + 3], args.env);
}