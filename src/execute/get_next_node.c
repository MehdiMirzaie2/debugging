/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 10:15:27 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/11 20:28:29 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "shell.h"

t_ast	*get_next_node(t_ast *ast)
{
	// t_ast			*ast = ast;
	static int		go_right = 0;
	static bool		checked_left = false;
	static bool		is_recursive = false;
	static bool		checked_all = false;
	t_ast			*next_node;
	int				i;

	i = 0;
	if (checked_all == true)
		return (NULL);
	if (is_recursive == false)
		while (i < go_right)
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
		/* check if the second node has multiple commads if it does
			point ast to second then recursively call the fucntion.
			this will execute the first if statement, and return the first command insider second.
		*/
		if (ast->u_node.link.second->type == E_ASTLINK)
		{
			ast = ast->u_node.link.second;
			checked_left = false;
			is_recursive = true;
			go_right++;
			get_next_node(ast);
		}
		/* if we ever reach this point it means we are at the bottom of the nodes, therefore, no more commands,
		that is why we make ast == NULL"
		*/
		next_node = ast->u_node.link.second;
		checked_all = true;
	}
	is_recursive = false;
	return (next_node);
}

// t_cmd	*get_next_cmd(t_ast *ast)
// {
// 	t_ast	*node = get_next_node(ast);
// 	char	**cmd;


// }