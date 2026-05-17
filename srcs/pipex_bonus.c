/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsao-pay <rsao-pay@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 00:39:52 by rsao-pay          #+#    #+#             */
/*   Updated: 2026/05/17 15:44:38 by rsao-pay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

static void	parent_proccess(int **fds, int argc, char **argv, char **envp)
{
	int	fileout;
	int	i;

	i = argc - 5;
	fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
		close_pipes(fds, 1);
	dup2(fileout, STDOUT_FILENO);
	dup2(fds[i][0], STDIN_FILENO);
	close_pipes(fds, 0);
	close(fileout);
	execute(argv[argc - 2], envp);
}

static int	child_proccess(int **fds, int forks, char **argv, char **envp)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (++i < forks)
	{
		pid = fork();
		if (pid == -1)
			close_pipes(fds, 1);
		else if (pid == 0)
		{
			dup2(fds[i - 1][0], STDIN_FILENO);
			dup2(fds[i][1], STDOUT_FILENO);
			close_pipes(fds, 0);
			execute(argv[i + 2], envp);
		}
	}
	return (i - 1);
}

static void	multiple_pipes(int **fds, int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		filein;

	pid = fork();
	if (pid == -1)
		close_pipes(fds, 1);
	else if (pid == 0)
	{
		filein = open(argv[1], O_RDONLY, 0777);
		if (filein == -1)
			close_pipes(fds, 1);
		dup2(filein, STDIN_FILENO);
		dup2(fds[0][1], STDOUT_FILENO);
		close_pipes(fds, 0);
		close(filein);
		execute(argv[2], envp);
	}
	child_proccess(fds, argc - 4, argv, envp);
	parent_proccess(fds, argc, argv, envp);
}

static void	here_doc(int **fds, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		close_pipes(fds, 1);
	else if (pid == 0)
		hd_child_proccess(fds, argv, envp);
	else
	{
		close(fds[0][0]);
		close(fds[0][1]);
		close(fds[1][1]);
		waitpid(pid, NULL, 0);
		hd_parent_proccess(fds, argv, envp);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	**fds;

	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			if (argc != 6)
				usage();
			fds = init_fds(6);
			here_doc(fds, argv, envp);
		}
		else
		{
			fds = init_fds(argc);
			if (!fds)
				error();
			multiple_pipes(fds, argc, argv, envp);
		}
	}
	else
		usage();
	return (0);
}
