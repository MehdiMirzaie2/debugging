/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 22:30:44 by mehdimirzai       #+#    #+#             */
/*   Updated: 2023/09/13 09:18:01 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	exec(t_pipex pipex)
{
	char	*cmd;

	cmd = pipex.cmd->u_node.cmd->cmd;
	if (is_builtin(cmd))
		execute_builtin_cmds(pipex.cmd->u_node.cmd, pipex.our_env);
	else
		execute_system_cmds(pipex.cmd->u_node.cmd, pipex.our_env);
}

void	middle_child(const t_pipex pipex, int pipes[][2])
{
	int	j;
	int fd_in;
	int	fd_out;

	j = 0;
	if (fork() == 0)
	{
		while (j < pipex.process_num + 1)
		{
			if (pipex.i != j)
				close(pipes[j][0]);
			if (pipex.i + 1 != j)
				close(pipes[j][1]);
			j++;
		}
	// checking for the first command if it has an input file 
	// if so make sure the input is redirected to that file.
		fd_in = pipes[pipex.i][0];
		if (pipex.i == 0 && pipex.cmd->u_node.cmd->strin != NULL)
		{
			close(fd_in);
			fd_in = open(pipex.cmd->u_node.cmd->strin->str, O_RDONLY);
		}
		fd_out = pipes[pipex.i + 1][1];
		if (pipex.i == pipex.process_num && pipex.cmd->u_node.cmd->strout != NULL)
		{
			close(fd_out);
			fd_out = open(pipex.cmd->u_node.cmd->strout->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		if ((re_input(fd_in, STDIN_FILENO) == -1)
			|| re_output(fd_out, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		exec(pipex);
	}
}

int	open_pipes(t_pipex pipex, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < pipex.process_num - 1)
	{
		if (pipe(pipes[i++]) == -1)
		{
			printf("Error with creating pipe\n");
			return (1);
		}
	}
	return (0);
}

// int	create_middle_process(t_pipex pipex)
// {
// 	int	i;

// 	i = 0;
// 	while (i < pipex.process_num)
// 	{
// 		pipex.pids[i] = fork();
// 		if (pipex.pids[i] == -1)
// 		{
// 			printf("Error with creating process\n");
// 			return (2);
// 		}
// 		if (pipex.pids[i] == 0)
// 			middle_child(pipex);
// 		i++;
// 	}
// 	return (0);
// }
