/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 13:52:47 by clovell           #+#    #+#             */
/*   Updated: 2023/09/09 20:50:10 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "standard.h"
#include "ast.h"

t_ast	*ast_build(t_token *tokens)
{
	t_astbuilder	*builder;
	t_ast			*ast;

	astbuilder_memman(&builder, false);
	builder->thead = tokens;
	builder->current = tokens;
	ast_memman(&builder->head, E_ASTCMD, false);
	builder->work = &builder->head;
	astb_start(builder);
	ast = builder->head;
	astbuilder_memman(&builder, true);
	return (ast);
}

void	ast_memman(t_ast **ast, t_asttype type, bool destroy)
{
	const t_ast	builder = {.type = type};

	if (!destroy)
	{
		*ast = malloc(sizeof(t_ast));
		ft_assert(ast == NULL, E_ERR_MALLOCFAIL, __FILE__, __LINE__);
		**ast = builder;
		if (type == E_ASTCMD)
			cmd_memman(&(*ast)->u_node.cmd, false);
	}
	else
	{
		if (*ast == NULL)
			return ;
		if ((*ast)->type == E_ASTCMD)
			cmd_memman(&(*ast)->u_node.cmd, true);
		else
		{
			ast_memman(&(*ast)->u_node.link.first, E_ASTCMD, true);
			ast_memman(&(*ast)->u_node.link.second, E_ASTCMD, true);
		}
		free(*ast);
	}
}
