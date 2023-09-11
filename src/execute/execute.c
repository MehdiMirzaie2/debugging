/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmirzaie <mmirzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:07:14 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/11 15:12:10 by mmirzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"

/*
currently not taking care of commands that have their paths included in the
input.
*/

bool	is_builtin(char	*cmd)
{
	if (ft_strncmp(cmd, "cd", 2) || ft_strncmp(cmd, "env", 3)
		|| ft_strncmp(cmd, "echo", 4) || ft_strncmp(cmd, "pwd", 3))
		return (true);
	return (false);
}
#include <stdio.h>
int	execute(t_ast *ast, t_env *our_env)
{
	// printf("execute %d\n", __LINE__);
	t_ast	*next_ast_node;
	char	*cmd;

	next_ast_node = get_next_node(ast);
	while (next_ast_node != NULL)
	{
		cmd = next_ast_node->u_node.cmd->cmd;
		if (is_builtin(cmd))
			execute_builtin_cmds(next_ast_node->u_node.cmd, our_env);
		next_ast_node = get_next_node(ast);
		// else
		// {
		// 	execute_
		// }

		// next_ast
	}
	return (0);
}