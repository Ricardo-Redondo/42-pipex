/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsao-pay <rsao-pay@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 19:41:05 by rsao-pay          #+#    #+#             */
/*   Updated: 2026/05/17 00:29:54 by rsao-pay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

void	error(void);
void	free_arr(char **arr);
char	*find_path(char *cmd, char **envp);
void	execute(char *arg, char **envp);
void	child_proccess(int *fd, char **argv, char **envp);
void	parent_proccess(int *fd, char **argv, char **envp);

#endif