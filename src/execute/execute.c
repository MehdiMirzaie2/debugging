/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:07:14 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/15 14:10:52 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"
#include <stdio.h> // must delete later
/*
currently not taking care of commands that have their paths included in the
input.
*/

void	handle_heredoc(t_ast *ast)
{
	const char	*template = "/tmp/mytempfileXXXXXX";
	int			fd;
	char		*filename;
	char		*lines;
	int			limiter_len;

	filename = ft_strdup(template);
	fd = open(filename, O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		ft_putstr_fd("error making temp file for heredoc\n", 2);
		exit(127);
	}
	limiter_len = ft_strlen(ast->u_node.cmd->heredoc->str);
	printf("got to %d\n", __LINE__);
	lines = readline("heredoc> ");
	while (ft_strncmp(lines, ast->u_node.cmd->heredoc->str, limiter_len) != 0)
	{
		ft_putstr_fd(lines, fd);
		ft_putchar_fd('\n', fd);
		free(lines);
		lines = readline("heredoc> ");
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	dup2(fd, STDIN_FILENO); // doing this so the stdin buffer is cleared.
	close(fd);
	if (unlink(filename) == -1)
		perror("unlink");
}


int	open_file(t_ast *ast, int pipe1[2], bool isone_cmd)
{
	int	file_fd;

	file_fd = -1;
	if (ast->u_node.cmd->strin != NULL)
	{
		file_fd = open(ast->u_node.cmd->strin->str, O_RDONLY);
		if (redirect(file_fd, STDIN_FILENO) < 0)
			ft_putstr_fd("could not re to infile\n", 2);
	}
	if (ast->u_node.cmd->heredoc != NULL)
	{
		handle_heredoc(ast);
	}
	if (ast->u_node.cmd->strout != NULL)
	{
		file_fd = open(ast->u_node.cmd->strout->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file_fd < 0)
		{
			ft_putstr_fd("could not open outfile\n", 2);
		}
		redirect(file_fd, STDOUT_FILENO);
		close(pipe1[1]);
	}
	else
	{
		if (isone_cmd)
			file_fd = 0;
		else
		{
			if (dup2(pipe1[1], STDOUT_FILENO) < 0)
			printf("dup2 error on  %d\n", __LINE__);
		}
		close(pipe1[1]);
		file_fd = 0;
	}
	return (file_fd);
}

void	execute(t_ast *ast_node, t_env *our_env)
{
	if (is_builtin(ast_node->u_node.cmd->cmd))
		execute_builtin_cmds(ast_node->u_node.cmd, our_env);	
	else
		execute_system_cmds(ast_node->u_node.cmd, our_env);
}

// void	one_command(t_ast *ast, t_env *our_env)
// {
// 	if (is_builtin(ast->u_node.cmd->cmd))
// 		execute_builtin_cmds(ast->u_node.cmd, our_env);	
// 	else
// 	{
// 		if (fork() == 0) //add error checks
// 			execute_system_cmds(ast->u_node.cmd, our_env);	
// 		wait(NULL);
// 	}
// }

int	process_ast(t_ast *ast, t_env *our_env)
{
	t_ast	*next_ast_node;
	int		pipe1[2];
	int		file_fd;
	int 	i = 0;
	int		num_cmds;
	bool	isone_cmd = false;

	isone_cmd = false;
	if (ast->type == E_ASTCMD)
		isone_cmd = true;
	num_cmds = get_num_cmd(ast);
	while (i < num_cmds)
	{
		if (isone_cmd)
			next_ast_node = ast;
		else
			next_ast_node = get_next_node(ast);
		if (pipe(pipe1) < 0)
			perror("error makeing pipe\n");
		if (fork() == 0)
		{
			file_fd = open_file(next_ast_node, pipe1, isone_cmd);
			close(pipe1[0]);
			if (file_fd < 0)
			{
				perror("pipe error\n");
				exit(EXIT_FAILURE);
			}
			execute(next_ast_node, our_env);
		}
		wait(NULL); // wait for child
		if (!isone_cmd)
			if (dup2(pipe1[0], STDIN_FILENO) < 0)
				perror("dup to pipe1[0] error\n");
		close(pipe1[0]);
		close(pipe1[1]);
		i++;
	}
	return (0);
}
