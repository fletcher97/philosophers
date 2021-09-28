/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 23:06:55 by mgueifao          #+#    #+#             */
/*   Updated: 2021/09/28 00:30:20 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "philo.h"

static int	get_self(t_sym *s)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&s->master);
	while (++i < s->pcount && s->philos[i].id != pthread_self())
		;
	pthread_mutex_unlock(&s->master);
	return (i);
}

int	check_state(t_sym *s, int self)
{
	long	time;

	time = get_time(s);
	if (s->pdone == s->pcount)
	{
		pthread_mutex_lock(&s->master);
		if (s->sym_state != sym_halt)
			s->sym_state = sym_halt;
		pthread_mutex_unlock(&s->master);
		return (0);
	}
	if (time - s->philos[self].last_eat > s->tdie || s->sym_state == sym_halt)
	{
		pthread_mutex_lock(&s->master);
		if (s->sym_state != sym_halt)
		{
			printf("%ld %d died\n", get_time(s), self + 1);
			s->sym_state = sym_halt;
			s->died = 1;
		}
		pthread_mutex_unlock(&s->master);
		return (0);
	}
	return (1);
}

static int	check_status(t_sym *s)
{
	pthread_mutex_lock(&s->master);
	if (s->sym_state == sym_halt)
	{
		pthread_mutex_unlock(&s->master);
		return (0);
	}
	pthread_mutex_unlock(&s->master);
	return (1);
}

void	*philo_main(void *arg)
{
	t_sym	*s;
	int		self;

	s = (t_sym *) arg;
	self = get_self(s);
	while (1)
	{
		if (!check_status(s) || !peat(s, self))
			break ;
		if (!check_status(s) || !psleep(s, self))
			break ;
		if (!check_status(s))
			break ;
		else
			pthink(s, self);
	}
	return (NULL);
}

long	get_time(t_sym *s)
{
	gettimeofday(&s->curr, NULL);
	return ((s->curr.tv_sec - s->start.tv_sec) * 1000
		+ (s->curr.tv_usec - s->start.tv_usec) / 1000);
}
