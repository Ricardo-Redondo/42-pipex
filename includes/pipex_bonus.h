/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsao-pay <rsao-pay@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 00:52:21 by rsao-pay          #+#    #+#             */
/*   Updated: 2026/05/17 15:15:03 by rsao-pay         ###   ########.fr       */
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

int		**init_fds(int argc);
int		get_next_line(char **line);
void	hd_parent_proccess(int **fds, char **argv, char **envp);
void	hd_child_proccess(int **fds, char **argv, char **envp);
void	close_pipes(int **fds, int is_error);

#endif