#ifndef EXECUTE_H
# define EXECUTE_H

# include "shell.h"

# define MAX_PROCESS_NUM 30

typedef struct s_pipex		t_pipex;
struct s_pipex
{
	int		process_num;
	int		pids[MAX_PROCESS_NUM];
	// int		pipes[MAX_PROCESS_NUM][2];
	t_env	*our_env;
	t_ast	*cmd;

	int		i;
};

t_ast	*get_next_node(t_ast *ast);

void	execute_builtin_cmds(t_cmd *cmd_struct, t_env *our_env);

void	execute_system_cmds(t_cmd *cmd, t_env *env);

// int	execute(t_ast *ast, t_env *our_env);
int	process_ast(t_ast *ast, t_env *our_env);
//pipe
// void	middle_child(const t_pipex pipex, int pipes[][2]);
// int	open_pipes(t_pipex pipex, int pipes[][2]);
// int	create_middle_process(t_pipex pipex);
void	exec(t_pipex pipex);

// utils
int	get_num_cmd(t_ast *ast);
int re_output(int from, int to);
int	re_input(int from, int to);
bool	is_builtin(char	*cmd);

#endif