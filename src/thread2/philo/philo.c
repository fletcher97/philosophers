/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 23:06:55 by mgueifao          #+#    #+#             */
/*   Updated: 2021/09/23 02:15:05 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "philo.h"

int	get_self(t_sym *s)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&s->master);
	while (++i < s->pcount && s->philos[i].id != pthread_self())
		;
	pthread_mutex_unlock(&s->master);
	return (i);
}

void	wait_start(t_sym *s, int self)
{
	while (1)
	{
		pthread_mutex_lock(&s->master);
		if (s->sym_state == sym_running)
		{
			pthread_mutex_unlock(&s->master);
			break;
		}
		pthread_mutex_unlock(&s->master);
	}
	s->philos[self].last_eat = 0;
}

void	*philo_main(void *arg)
{
	t_sym	*s;
	int		self;

	s = (t_sym *) arg;
	self = get_self(s);
	wait_start(s, self);
	while (1)
	{
		pthread_mutex_lock(&s->master);
		if (s->pdone == s->pcount || s->died)
			s->sym_state = sym_halt;
		if (s->sym_state == sym_halt)
			break;
		pthread_mutex_unlock(&s->master);
		if (!peat(s, self))
			break;
		if (!psleep(s, self))
			break;
		pthink(s, self);
	}
	return (NULL);
}
