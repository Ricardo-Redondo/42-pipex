/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsao-pay <rsao-pay@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 00:40:15 by rsao-pay          #+#    #+#             */
/*   Updated: 2026/05/17 15:28:40 by rsao-pay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	**init_fds(int argc)
{
	int	**fds;
	int	i;

	fds = malloc(sizeof(int *) * (argc - 4 + 1));
	if (!fds)
		error();
	i = 0;
	while (i < argc - 4)
	{
		fds[i] = malloc(sizeof(int) * 2);
		if (!fds[i] || pipe(fds[i]) == -1)
		{
			fds[i] = NULL;
			free_arr((void **)fds);
			return (NULL);
		}
		i++;
	}
	fds[i] = NULL;
	return (fds);
}

void	hd_child_proccess(int **fds, char **argv, char **envp)
{
	char	*line;
	char	*delimiter;
	pid_t	pid;

	delimiter = ft_strjoin(argv[2], "\n");
	while (get_next_line(&line))
	{
		if (ft_strncmp(line, delimiter, ft_strlen(argv[2])) == 0)
			break ;
		write(fds[0][1], line, ft_strlen(line));
		free(line);
	}
	close(fds[0][1]);
	pid = fork();
	if (pid == 0)
	{
		dup2(fds[0][0], STDIN_FILENO);
		dup2(fds[1][1], STDOUT_FILENO);
		close_pipes(fds, 0);
		execute(argv[3], envp);
	}
	waitpid(pid, NULL, 0);
	exit(0);
}

void	hd_parent_proccess(int **fds, char **argv, char **envp)
{
	int	fileout;

	fileout = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fileout == -1)
		close_pipes(fds, 1);
	dup2(fds[1][0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close_pipes(fds, 0);
	close(fileout);
	execute(argv[4], envp);
}

void	close_pipes(int **fds, int is_error)
{
	int	i;

	i = 0;
	while (fds[i])
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
	if (is_error)
		error();
}

int	get_next_line(char **line)
{
	char	*buffer;
	int		bytes;
	int		i;
	char	c;

	bytes = 0;
	i = 0;
	buffer = malloc(sizeof(char) * 10000);
	if (!buffer)
		return (-1);
	bytes = read(0, &c, 1);
	while (c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		bytes = read(0, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	*line = ft_strdup(buffer);
	free(buffer);
	return (bytes);
}
