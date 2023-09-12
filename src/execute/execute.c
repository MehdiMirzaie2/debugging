/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:07:14 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/12 10:59:13 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"
#include <stdio.h> // must delete later
/*
currently not taking care of commands that have their paths included in the
input.
*/

bool	is_builtin(char	*cmd)
{
	if (!ft_strncmp(cmd, "cd", 2) || !ft_strncmp(cmd, "env", 3)
		|| !ft_strncmp(cmd, "echo", 4) || !ft_strncmp(cmd, "pwd", 3))
		return (true);
	return (false);
}

int	re_input(int from, int to)
{
	if (dup2(from, to) == -1)
		return (-1);
	if (close(from) == -1)
		return (-1);
	return (0);
}

int re_output(int from, int to)
{
	if (dup2(from, to) == -1)
		return (-1);
	if (close(from) == -1)
		return (-1);
	return (0);
}


// int	get_process_num(t_ast *ast)
// {
// 	int	num;

// 	num = 1;
// 	while (ast)
// 	{
// 		ast = ast->u_node.link;
// 		num++;
// 	}
// 	return (num);
// }

// typedef struct s_pipex		t_pipex;
// struct s_pipex
// {
// 	int		process_num;
// 	int		*pids;
// 	int		pipes[][2];
// };

// #define MAX_PROCESS_NUM 30

// int	open_pipes(t_pipex pipex)
// {
// 	int	i;

// 	i = 0;
// 	while (i < pipex.process_num + 1)
// 	{
// 		if (pipe(pipex.pipes[i++]) == -1)
// 		{
// 			printf("Error with creating pipe\n");
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

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
// 			middle_child(args, pipes, i);
// 		i++;
// 	}
// 	return (0);
// }

int	execute(t_ast *ast, t_env *our_env)
{
	t_ast	*next_ast_node;
	char	*cmd;
	// int		pids[MAX_PROCESS_NUM];
	// int		pipes[MAX_PROCESS_NUM + 1];

	if (ast->type == E_ASTCMD)
	{
		if (is_builtin(ast->u_node.cmd->cmd))
			execute_builtin_cmds(ast->u_node.cmd, our_env);	
		else
		{
			if (fork() == 0)
				execute_system_cmds(ast->u_node.cmd, our_env);	
			wait(NULL);
		}
	}
	else
	{
		/*creating the pipes*/
		// int process_num;
		
		// process_num = get_process_num(t_ast);
		// ppp = (t_pipex){.pids = pids, .pipes = pipes, .process_num = process_num};
		// open_pipes(ppp);

		/*executing*/
		next_ast_node = get_next_node(ast);
		while (next_ast_node != NULL)
		{
			cmd = next_ast_node->u_node.cmd->cmd;
			if (is_builtin(cmd))
				execute_builtin_cmds(next_ast_node->u_node.cmd, our_env);
			else
				execute_system_cmds(next_ast_node->u_node.cmd, our_env);
			next_ast_node = get_next_node(ast);
		}	
	}
	return (0);
}

// figure out how many process are needed then work out, then figure the implemention of
// two pipes then multiple pipes.

// take care of input are output redirections
