/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsao-pay <rsao-pay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 00:40:15 by rsao-pay          #+#    #+#             */
/*   Updated: 2026/05/28 11:16:31 by rsao-pay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	hd_child_proccess(t_pipes *fds, t_node *node, char **argv, char **envp)
{
	char	*line;
	char	*delimiter;
	pid_t	pid;

	delimiter = ft_strjoin(argv[2], "\n");
	while (get_next_line(&line))
	{
		if (ft_strncmp(line, delimiter, ft_strlen(argv[2])) == 0)
			break ;
		write(node->fd[1], line, ft_strlen(line));
		free(line);
	}
	close(node->fd[1]);
	pid = fork();
	if (pid == -1)
		free_list(fds, 1);
	if (pid == 0)
	{
		dup2(node->fd[0], STDIN_FILENO);
		dup2(node->next->fd[1], STDOUT_FILENO);
		close_pipes(node->fd);
		close(node->next->fd[1]);
		execute(argv[3], envp);
	}
	waitpid(pid, NULL, 0);
}

void	hd_parent_proccess(t_pipes *fds, t_node *node, char **argv, char **envp)
{
	int	fileout;

	fileout = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fileout == -1)
		free_list(fds, 1);
	dup2(node->fd[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close_pipes(node->fd);
	close(fileout);
	execute(argv[4], envp);
}

void	close_pipes(int *fd)
{
	close(fd[0]);
	close(fd[1]);
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
