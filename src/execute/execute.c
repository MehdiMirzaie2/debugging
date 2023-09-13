/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:07:14 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/13 15:23:43 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"
#include <stdio.h> // must delete later
/*
currently not taking care of commands that have their paths included in the
input.
*/

void	execute(t_ast *ast_node, t_env *our_env)
{
	if (is_builtin(ast_node->u_node.cmd->cmd))
		execute_builtin_cmds(ast_node->u_node.cmd, our_env);	
	else
		execute_system_cmds(ast_node->u_node.cmd, our_env);
}

void	one_command(t_ast *ast, t_env *our_env)
{
	if (is_builtin(ast->u_node.cmd->cmd))
		execute_builtin_cmds(ast->u_node.cmd, our_env);	
	else
	{
		if (fork() == 0) //add error checks
			execute_system_cmds(ast->u_node.cmd, our_env);	
		wait(NULL);
	}
}

int	process_ast(t_ast *ast, t_env *our_env)
{
	t_ast	*next_ast_node;
	int		pipe1[2];
	// int		pid;
	int 	i = 0;
	int		num_cmds;

	if (ast->type == E_ASTCMD)
		one_command(ast, our_env);
	else
	{
		if (fork() == 0)
		{
			num_cmds = get_num_cmd(ast);
			while (i < num_cmds)
			{
				next_ast_node = get_next_node(ast);
				pipe(pipe1);
				if (fork() == 0)
				{
					close(pipe1[0]); //closing read end of pipe because it is not being used
					if (i != num_cmds - 1)
						dup2(pipe1[1], STDOUT_FILENO);
					close(pipe1[1]);
					execute(next_ast_node, our_env);
				}
				wait(NULL);
				if (i != num_cmds - 1)
					dup2(pipe1[0], STDIN_FILENO);
				close(pipe1[0]);
				close(pipe1[1]);
				i++;
			}
			exit(EXIT_SUCCESS);
		}
		wait(NULL);
	}
	return (0);
}
