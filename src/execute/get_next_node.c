/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 10:15:27 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/15 14:07:59 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "shell.h"

t_ast	*get_next_node(t_ast *ast)
{
	static int		go_right = 0;
	static bool		checked_left = false;
	static bool		is_recursive = false;
	t_ast			*next_node = NULL;
	int				i;

	i = 0;
	if (!ast)
		return (NULL);
	if (is_recursive == false)
		while (i++ < go_right)
			ast = ast->u_node.link.second;
	if (checked_left == false)
	{
		next_node = ast->u_node.link.first;
		checked_left = true;
	}
	else
	{
		if (ast->type == E_ASTCMD)
			return (NULL);
		if (ast->type == E_ASTLINK)
		{
			if (ast->u_node.link.second->type == E_ASTCMD)
				next_node = ast->u_node.link.second;
			else{
				ast = ast->u_node.link.second;
				checked_left = false;
				is_recursive = true;
				go_right++;
				next_node = ast->u_node.link.first;
				checked_left = true;	
			}
		}
	}
	is_recursive = false;
	return (next_node);
}
