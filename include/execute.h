#ifndef EXECUTE_H
# define EXECUTE_H

// # include "ast.h"
#include "shell.h"

t_ast	*get_next_node(t_ast *ast);

void	execute_builtin_cmds(t_cmd *cmd_struct, t_env *our_env);

int	execute(t_ast *ast, t_env *our_env);

#endif