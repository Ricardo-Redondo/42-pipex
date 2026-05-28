/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsao-pay <rsao-pay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 20:08:17 by rsao-pay          #+#    #+#             */
/*   Updated: 2026/05/28 11:16:21 by rsao-pay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_list(t_pipes *fds, int is_error)
{
	t_node	*node;
	t_node	*pos;

	node = fds->head;
	while (node)
	{
		pos = node;
		close_pipes(pos->fd);
		free(pos);
		node = node->next;
	}
	if (is_error)
		error();
}

static t_node	*create_node(void)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

static void	add_node(t_pipes *fds)
{
	t_node	*head;
	t_node	*node;

	node = create_node();
	if (!node)
		free_list(fds, 1);
	if (!fds->head)
		fds->head = node;
	else
	{
		head = fds->head;
		while (head->next)
			head = head->next;
		head->next = node;
		node->prev = head;
	}
}

void	init_pipes(t_pipes *fds, int pipes)
{
	while (pipes > 0)
	{
		add_node(fds);
		pipes--;
	}
}
