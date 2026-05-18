/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsao-pay <rsao-pay@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 00:52:21 by rsao-pay          #+#    #+#             */
/*   Updated: 2026/05/18 22:13:42 by rsao-pay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include "pipex.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_node
{
	int				fd[2];
	struct s_node	*next;
	struct s_node	*prev;
}					t_node;

typedef struct s_pipes
{
	t_node			*head;
}					t_pipes;

// utils
void				hd_parent_proccess(t_pipes *fds, t_node *node, char **argv,
						char **envp);
void				hd_child_proccess(t_pipes *fds, t_node *node, char **argv,
						char **envp);
void				close_pipes(int *fd);
int					get_next_line(char **line);

// list_utils
void				init_pipes(t_pipes *fds, int pipes);
void				close_pipes(int *fd);
void				free_list(t_pipes *fds, int is_error);

#endif