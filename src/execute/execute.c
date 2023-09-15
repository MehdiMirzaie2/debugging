/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdimirzaie <mehdimirzaie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:07:14 by mmirzaie          #+#    #+#             */
/*   Updated: 2023/09/15 11:12:47 by mehdimirzai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"
#include <stdio.h> // must delete later
/*
currently not taking care of commands that have their paths included in the
input.
*/

int	open_file(t_ast *ast, int pipe1[2])
{
	int	file_fd;

	file_fd = -1;
	if (ast->u_node.cmd->strin != NULL)
	{
		file_fd = open(ast->u_node.cmd->strin->str, O_RDONLY);
		// ft_putstr_fd("redirect to infile\n", 2);
		if (redirect(file_fd, STDIN_FILENO) < 0)
			ft_putstr_fd("could not re to infile\n", 2);
		ft_putstr_fd("redirect to infile\n", 2);
	}
	if (ast->u_node.cmd->strout != NULL)
	{
		file_fd = open(ast->u_node.cmd->strout->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file_fd < 0)
		{
			ft_putstr_fd("could not open outfile\n", 2);	
			// exit(EXIT_FAILURE);
		}
		ft_putstr_fd("redirect to outfile", 2);
		redirect(file_fd, STDOUT_FILENO);
		close(pipe1[1]);
		// printf("re outfile works\n");
	}
	else
	{
		// ft_putstr_fd("out to p1[1]", 2);
		if (dup2(pipe1[1], STDOUT_FILENO) < 0)
			perror("wrong\n");
		else
		{
			ft_putstr_fd("pipe[1] is okay\n", 2);
			close(pipe1[1]);
		}
		file_fd = 0;
	}
	ft_putstr_fd("\n", 2);
	ft_putnbr_fd(file_fd, 2);
	ft_putstr_fd("\n", 2);
	return (file_fd);
}

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
	int		file_fd;
	int 	i = 0;
	int		num_cmds;
	int		out = dup(STDOUT_FILENO);
	// int		in;

	if (ast->type == E_ASTCMD)
		one_command(ast, our_env);
	else
	{
		num_cmds = get_num_cmd(ast);
		ft_putstr_fd("\n", 2);
		while (i < num_cmds)
		{
			next_ast_node = get_next_node(ast);
			if (pipe(pipe1) < 0)
				perror("error makeing pipe\n");
			if (fork() == 0)
			{
				file_fd = open_file(next_ast_node, pipe1);
				close(pipe1[0]);
				if (file_fd < 0)
				{
					dup2(out, STDOUT_FILENO);
					printf("errro %d %d\n", file_fd, __LINE__);
					exit(EXIT_FAILURE);
				}
				execute(next_ast_node, our_env);
			}
			wait(NULL); // wait for child
			if (dup2(pipe1[0], STDIN_FILENO) < 0)
				perror("dup to pipe1[0] error\n");
			close(pipe1[0]);
			close(pipe1[1]);
			i++;
			ft_putstr_fd("________________________________________\n\n", 2);
		}
	}
	return (0);
}

// <infile sort | uniq -c | sort -r | head -3 > outfile














// int	process_ast(t_ast *ast, t_env *our_env)
// {
// 	t_ast	*next_ast_node;
// 	int		pipe1[2];
// 	int		file_fd;
// 	int 	i = 0;
// 	int		num_cmds;
// 	int		out = dup(STDOUT_FILENO);
// 	int		in;

// 	if (ast->type == E_ASTCMD)
// 		one_command(ast, our_env);
// 	else
// 	{
// 		num_cmds = get_num_cmd(ast);
// 		ft_putstr_fd("\n", 2);
// 		while (i < num_cmds)
// 		{
// 			next_ast_node = get_next_node(ast);
// 			if (pipe(pipe1) < 0)
// 				perror("error makeing pipe\n");
// 			if (fork() == 0)
// 			{
// 				if (i > 0)
// 					dup2(in, STDIN_FILENO);
// 				close(pipe1[0]); //closing read end of pipe because it is not being used
// 				file_fd = open_file(next_ast_node, pipe1);
// 				if (file_fd < 0)
// 				{
// 					dup2(out, STDOUT_FILENO);
// 					printf("errro %d %d\n", file_fd, __LINE__);
// 					exit(EXIT_FAILURE);
// 				}
// 				// close(pipe1[1]);
// 				if (fork() == 0) //child inside child
// 				{
// 					// if (fork() == 0)
// 					// {
// 						dup2(STDERR_FILENO, STDOUT_FILENO);
// 						close(STDERR_FILENO);
// 						printf("\nchild``````````````````````````````````\n");
// 						execute(next_ast_node, our_env);
// 					// }
// 					// wait(NULL);
// 					ft_putstr_fd("``````````````````````````````````\n", 2);
// 					// execute(next_ast_node, our_env);
// 				}
// 				wait(NULL); //wait for child inside child
// 				execute(next_ast_node, our_env);
// 				// exit(EXIT_SUCCESS);
// 			}
// 			// perror("```````````\n");
// 			// char *line = readline("hi");
// 			// while (line)
// 			// {
// 			// 	line = readline("hi");
// 			// 	perror(line);
// 			// }
// 			// perror("```````````\n");
// 			wait(NULL); // wait for child
// 			in = dup(pipe1[0]);
// 			// if (dup2(pipe1[0], STDIN_FILENO) < 0)
// 			// 	perror("dup to pipe1[0] error\n");
// 			close(pipe1[0]);
// 			close(pipe1[1]);
			
// 			// wait(NULL);
// 			ft_putstr_fd("\n pid = ", 2);
// 			ft_putnbr_fd(getpid(), 2);
// 			ft_putstr_fd("\n", 2);
// 			i++;
// 			ft_putstr_fd("________________________________________\n\n", 2);
// 		}
// 	}
// 	return (0);
// }



// <infile sort -r | uniq -c | head -2 > outfile



// int	process_ast(t_ast *ast, t_env *our_env)
// {
// 	t_ast	*next_ast_node;
// 	int		pipe1[2];
// 	// int		pipe2[2];
// 	// int		temp_storage;
// 	int		file_fd;
// 	// int		pid;
// 	int 	i = 0;
// 	int		num_cmds;

// 	if (ast->type == E_ASTCMD)
// 		one_command(ast, our_env);
// 	else
// 	{
// 		num_cmds = get_num_cmd(ast);
// 		ft_putstr_fd("parrent child fd: ", 2);
// 		ft_putnbr_fd(getpid(), 2);
// 		ft_putstr_fd("\n", 2);
// 		while (i < num_cmds)
// 		{
// 			ft_putnbr_fd(i, 2);
// 			ft_putstr_fd(" ", 2);
// 			next_ast_node = get_next_node(ast);
// 			if (fork() == 0)
// 			{
// 				pipe(pipe1);
// 				int pid = fork();
// 				if (pid == 0)
// 				{
// 					close(pipe1[0]); //closing read end of pipe because it is not being used
// 					file_fd = open_file(next_ast_node, pipe1);
// 					if (file_fd < 0)
// 						exit(EXIT_FAILURE);
// 					close(pipe1[1]);
// 					execute(next_ast_node, our_env);
// 				}
// 				wait(NULL);
// 				dup2(pipe1[0], STDIN_FILENO);
// 				close(pipe1[0]);
// 				close(pipe1[1]);
// 				exit(EXIT_SUCCESS);
// 			}
// 			wait(NULL);
// 			i++;
// 		}
// 		ft_putstr_fd("\n\nparrent fd: ", 2);
// 		ft_putnbr_fd(getpid(), 2);
// 		ft_putstr_fd("\n", 2);
// 	}
// 	return (0);
// }













// int	process_ast(t_ast *ast, t_env *our_env)
// {
// 	t_ast	*next_ast_node;
// 	int		pipe1[2];
// 	// int		pipe2[2];
// 	// int		temp_storage;
// 	int		file_fd;
// 	// int		pid;
// 	int 	i = 0;
// 	int		num_cmds;

// 	if (ast->type == E_ASTCMD)
// 		one_command(ast, our_env);
// 	else
// 	{
// 		if (fork() == 0)
// 		{
// 			num_cmds = get_num_cmd(ast);
// 			ft_putstr_fd("first child fd: ", 2);
// 			ft_putnbr_fd(getpid(), 2);
// 			ft_putstr_fd("\n", 2);
// 			while (i < num_cmds)
// 			{
// 				next_ast_node = get_next_node(ast);
// 				pipe(pipe1);
// 				int pid = fork();
// 				if (pid == 0)
// 				{
// 					ft_putstr_fd("child fd: ", 2);
// 					ft_putnbr_fd(getpid(), 2);
// 					ft_putstr_fd("\n", 2);
// 					close(pipe1[0]); //closing read end of pipe because it is not being used

// 					ft_putnbr_fd(i, 2);
// 					ft_putstr_fd(" ", 2);
// 					file_fd = open_file(next_ast_node, pipe1);
// 					if (file_fd < 0)
// 						exit(EXIT_FAILURE);
// 					close(pipe1[1]);
// 					execute(next_ast_node, our_env);
// 				}
// 				wait(NULL);
// 				dup2(pipe1[0], STDIN_FILENO);
// 				// ft_putstr_fd("changed\n", 2);
// 				close(pipe1[0]);
// 				close(pipe1[1]);
// 				i++;
// 			}
// 			exit(EXIT_SUCCESS);
// 		}
// 		wait(NULL);
// 	}
// 	return (0);
// }
