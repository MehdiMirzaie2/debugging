/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin_cmds.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 10:43:28 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/11 20:33:01 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft_extra.h"

/*
check whether the string has a specific character, if so it will return
the last position of that character
*/

// int	ft_strrchrpos(const char *s, int c)
// {
// 	int		i;
// 	char	*convert;
// 	char	character;

// 	convert = (char *)s;
// 	character = (char)c;
// 	i = ft_strlen(convert);
// 	while (i >= 0)
// 	{
// 		if (convert[i] == character)
// 			return (i);
// 		i--;
// 	}
// 	return (0);
// }

void	execute_builtin_cmds(t_cmd *cmd_struct, t_env *our_env)
{
	// char	**cmd_and_args_joined;

	if (ft_strncmp(cmd_struct->cmd, "cd", 2) == 0)
		ft_cd(cmd_struct->args->str, &our_env);
	else if (ft_strncmp(cmd_struct->cmd, "env", 3) == 0)
		ft_env(our_env);
	else if (ft_strncmp(cmd_struct->cmd, "export", 6) == 0)
		export(our_env, cmd_struct->args->str);
	else if (ft_strncmp(cmd_struct->cmd, "echo", 4) == 0)
	{
		if (cmd_struct->strout == NULL)
			ft_echo(cmd_struct->args->str, NULL, 0);
		else
			ft_echo(cmd_struct->args->str, cmd_struct->strout->str, 0);
	}
	else if (ft_strncmp(cmd_struct->cmd, "unset", 5) == 0)
		unset(our_env, cmd_struct->args->str);
	else if (ft_strncmp(cmd_struct->cmd, "pwd", 3) == 0)
		ft_pwd();
}