/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsao-pay <rsao-pay@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 00:39:52 by rsao-pay          #+#    #+#             */
/*   Updated: 2026/05/18 22:06:43 by rsao-pay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

static void	parent_proccess(t_pipes *fds, t_node *node, int argc, char **argv,
		char **envp)
{
	int	fileout;

	fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
		free_list(fds, 1);
	dup2(fileout, STDOUT_FILENO);
	dup2(node->fd[0], STDIN_FILENO);
	close_pipes(node->fd);
	close(fileout);
	execute(argv[argc - 2], envp);
}

static t_node	*child_proccess(t_pipes *fds, t_node *node, int forks,
		char **argv, char **envp)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (++i < forks)
	{
		if (pipe(node->fd) == -1)
			free_list(fds, 1);
		pid = fork();
		if (pid == -1)
			free_list(fds, 1);
		else if (pid == 0)
		{
			dup2(node->prev->fd[0], STDIN_FILENO);
			dup2(node->fd[1], STDOUT_FILENO);
			close_pipes(node->prev->fd);
			close_pipes(node->fd);
			execute(argv[i + 2], envp);
		}
		close(node->prev->fd[0]);
		close(node->fd[1]);
		if (i + 1 < forks)
			node = node->next;
	}
	return (node);
}

static void	multiple_pipes(t_pipes *fds, int argc, char **argv, char **envp)
{
	pid_t	pid;
	t_node	*node;
	int		filein;

	node = fds->head;
	if (pipe(node->fd) == -1)
		free_list(fds, 1);
	pid = fork();
	if (pid == -1)
		free_list(fds, 1);
	else if (pid == 0)
	{
		filein = open(argv[1], O_RDONLY, 0777);
		if (filein == -1)
			free_list(fds, 1);
		dup2(filein, STDIN_FILENO);
		dup2(node->fd[1], STDOUT_FILENO);
		close_pipes(node->fd);
		close(filein);
		execute(argv[2], envp);
	}
	close(node->fd[1]);
	if (argc > 5)
		node = child_proccess(fds, node->next, argc - 4, argv, envp);
	parent_proccess(fds, node, argc, argv, envp);
}

static void	here_doc(t_pipes *fds, char **argv, char **envp)
{
	t_node	*node;
	pid_t	pid;

	node = fds->head;
	if (pipe(node->fd) == -1 || pipe(node->next->fd) == -1)
		free_list(fds, 1);
	pid = fork();
	if (pid == -1)
		free_list(fds, 1);
	else if (pid == 0)
		hd_child_proccess(fds, node, argv, envp);
	else
	{
		close(node->fd[0]);
		close(node->fd[1]);
		close(node->next->fd[1]);
		waitpid(pid, NULL, 0);
		hd_parent_proccess(fds, node->next, argv, envp);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipes	pipes;

	pipes.head = NULL;
	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			if (argc != 6)
				usage();
			init_pipes(&pipes, 2);
			here_doc(&pipes, argv, envp);
		}
		else
		{
			init_pipes(&pipes, argc - 4);
			multiple_pipes(&pipes, argc, argv, envp);
		}
	}
	else
		usage();
	return (0);
}
