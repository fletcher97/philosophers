/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ripper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 22:22:15 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/28 02:49:55 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ripper(void *sym)
{
	t_sym	*s;
	int		i;

	s = sym;
	sem_wait(s->sem_died);
	sem_wait(s->sem_died);
	if (s->tr_me)
		return (NULL);
	s->tr_me = 1;
	i = 0;
	while (i < s->pcount)
	{
		sem_post(s->sem_done);
		kill(s->pids[i], SIGTERM);
		i++;
	}
	return (NULL);
}

void	*completionist(void *sym)
{
	t_sym	*s;
	int		i;

	s = sym;
	i = 0;
	sem_wait(s->sem_done);
	while (i++ < s->pcount)
		sem_wait(s->sem_done);
	if (s->tr_me)
		return (NULL);
	s->tr_me = 1;
	i = 0;
	while (i < s->pcount)
	{
		kill(s->pids[i], SIGTERM);
		i++;
	}
	sem_post(s->sem_died);
	return (NULL);
}
