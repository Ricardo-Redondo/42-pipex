/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsao-pay <rsao-pay@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 22:43:54 by rsao-pay          #+#    #+#             */
/*   Updated: 2026/05/17 15:15:23 by rsao-pay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_arr(void **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return ;
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd);
		if (access(path, X_OK) == 0)
			return (free_arr((void **)paths), path);
		free(path);
		i++;
	}
	return (free_arr((void **)paths), NULL);
}

void	execute(char *arg, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(arg, ' ');
	path = find_path(cmd[0], envp);
	if (!path)
	{
		free_arr((void **)cmd);
		error();
	}
	if (execve(path, cmd, envp) == -1)
	{
		free_arr((void **)cmd);
		free(path);
		error();
	}
}

void	error(void)
{
	perror("\033[1;31m[ERROR]");
	exit(1);
}

void	usage(void)
{
	ft_putstr_fd("\033[1;31m[ERROR] Invalid arguments\n\e[0m", 2);
	ft_putstr_fd("Usage:\n", 1);
	ft_putstr_fd("./pipex <file_1> <cmd_1> ... <cmd_n> <file_2>\n", 1);
	ft_putstr_fd("or\n", 1);
	ft_putstr_fd("./pipex here_doc <delimiter> <cmd_1> <cmd_2> <file>\n", 1);
	exit(1);
}
