/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:07:14 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/18 13:15:22 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"
#include <stdio.h> // must delete later
/*
currently not taking care of commands that have their paths included in the
input.
*/

void	terminate(int sig)
{
	if (sig == SIGUSR1)
	{
		ft_putstr_fd("exiting child\n", 2);
		exit(130);
	}
}

void	handle_heredoc(t_ast *ast)
{
	const char	*template = "/tmp/mytempfileXXXXXX";
	int			fd;
	char		*filename;
	char		*lines;
	int			limiter_len;
	// const	int	in = dup(STDIN_FILENO);
	
	filename = ft_strdup(template);
	fd = open(filename, O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		ft_putstr_fd("error making temp file for heredoc\n", 2);
		exit(127);
	}
	limiter_len = ft_strlen(ast->u_node.cmd->heredoc->str);
	// printf("got to %d\n", __LINE__);
	lines = readline("heredoc> ");
	while (ft_strncmp(lines, ast->u_node.cmd->heredoc->str, limiter_len) != 0)
	{
		// signal(SIGUSR1, terminate);
		ft_putstr_fd(lines, fd);
		ft_putchar_fd('\n', fd);
		free(lines);
		lines = readline("heredoc> ");
		if (!lines || g_value == SIGUSR1)
		{
			printf("deleting\n");
			// dup2(in, STDIN_FILENO);
			// close(in);
			if (unlink(filename) == -1)
				perror("unlink");
			exit(130);
		}
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	dup2(fd, STDIN_FILENO); // doing this so the stdin buffer is cleared.
	close(fd);
	if (unlink(filename) == -1)
		perror("unlink");
}

int	open_file(t_ast *ast, int pipe1[2], int num_cmds, int i)
{
	int	file_fd;

	file_fd = -1;
	if (ast->u_node.cmd->heredoc != NULL)
		handle_heredoc(ast);
	if (ast->u_node.cmd->strin != NULL)
	{
		file_fd = open(ast->u_node.cmd->strin->str, O_RDONLY);
		if (redirect(file_fd, STDIN_FILENO) < 0)
			ft_putstr_fd("could not re to infile\n", 2);
	}
	if (ast->u_node.cmd->strout != NULL)
	{
		
		file_fd = open(ast->u_node.cmd->strout->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file_fd < 0)
			ft_putstr_fd("could not open outfile\n", 2);
		redirect(file_fd, STDOUT_FILENO);
		close(pipe1[1]);
	}
	else
	{
		if (i == num_cmds -1)
			file_fd = 0;
		else
		{
			if (dup2(pipe1[1], STDOUT_FILENO) < 0)
				perror("could not dup pipe1[1]\n");
			else
				close(pipe1[1]);
		}
		file_fd = 0;
	}
	return (file_fd);
}

void	execute(t_ast *ast_node, t_env **our_env)
{
	if (is_builtin(ast_node->u_node.cmd->cmd))
		execute_builtin_cmds(ast_node->u_node.cmd, our_env);	
	else
		execute_system_cmds(ast_node->u_node.cmd, *our_env);
}

void	one_command(t_ast *ast, t_env **our_env)
{
	if (is_builtin(ast->u_node.cmd->cmd))
		execute_builtin_cmds(ast->u_node.cmd, our_env);	
	else
	{
		if (fork() == 0) //add error checks
			execute_system_cmds(ast->u_node.cmd, *our_env);	
		wait(NULL);
	}
}

int	process_ast(t_ast *ast, t_env **our_env)
{
	t_ast	*next_ast_node;
	int		pipe1[2];
	int		file_fd;
	int 	i = 0;
	int		num_cmds;
	int		out = dup(STDOUT_FILENO);
	int		in = dup(STDIN_FILENO);
	bool	reset = false;
	int		child_id = 0;
	int		status;

	
	if (ast->type == E_ASTCMD && ast->u_node.cmd->heredoc == NULL)
		one_command(ast, our_env);
	else
	{
		num_cmds = get_num_cmd(ast);
		// ft_putstr_fd("\n", 2);
		while (i < num_cmds)
		{
			if (i != 0 && i == num_cmds - 1)
				reset = true;
			next_ast_node = get_next_node(ast, reset);
			// printf("%s\n", next_ast_node->u_node.cmd->cmd);
			if (pipe(pipe1) < 0)
				perror("error makeing pipe\n");
			if ((child_id = fork()) == 0)
			{
				file_fd = open_file(next_ast_node, pipe1, num_cmds, i);
				close(pipe1[0]);
				if (file_fd < 0)
				{
					dup2(out, STDOUT_FILENO);
					printf("errro %d %d\n", file_fd, __LINE__);
					exit(EXIT_FAILURE);
				}
				execute(next_ast_node, our_env);
				exit(EXIT_SUCCESS);
			}
			wait(&status); // wait for child
			if (status == 130)
				continue;
			if (dup2(pipe1[0], STDIN_FILENO) < 0)
				perror("dup to pipe1[0] error\n");
			close(pipe1[0]);
			close(pipe1[1]);
			i++;
		}
		wait(NULL);
	}
	dup2(in, STDIN_FILENO);
	return (child_id);
}
// <infile sort | uniq -c | sort -r | head -3 > outfile