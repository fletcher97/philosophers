/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_ac.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 00:50:44 by mgueifao          #+#    #+#             */
/*   Updated: 2021/09/24 23:29:11 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

#include "philo.h"

static long get_time(t_sym *s)
{
	gettimeofday(&s->curr, NULL);
	return ((s->curr.tv_sec - s->start.tv_sec) * 1000 + (s->curr.tv_usec - s->start.tv_usec) / 1000);
}

static int	check_state(t_sym *s, int self)
{
	long	time;

	time = get_time(s);
	if (time - s->philos[self].last_eat > s->tdie)
		return (0);
	return (1);
}

int	get_forks(t_sym *s, int self)
{
	int i;

	i = 0;
	pthread_mutex_lock(&s->master);
	pthread_mutex_lock(&s->forks[self]);
	pthread_mutex_lock(&s->forks[(self + 1) % s->pcount]);
	pthread_mutex_unlock(&s->master);
	return (1);
}

int	peat(t_sym *s, int self)
{
	if (!check_state(s, self))
	{
		pthread_mutex_lock(&s->master);
		s->sym_state = sym_halt;
		pthread_mutex_unlock(&s->master);
		return (0);
	}
	while (!get_forks(s, self))
	{
		if (!check_state(s, self))
		{
			pthread_mutex_lock(&s->master);
			s->sym_state = sym_halt;
			pthread_mutex_unlock(&s->master);
			return (0);
		}
	}
	s->philos[self].last_eat = get_time(s);
	printf("%ld %d is eating\n", s->philos[self].last_eat, self);
	usleep(s->teat * 1000);
	pthread_mutex_unlock(&s->forks[self]);
	pthread_mutex_unlock(&s->forks[(self + 1) % s->pcount]);
	return (1);
}

int	psleep(t_sym *s, int self)
{
	long time;
	long sleep;

	time = get_time(s);
	sleep = time - s->philos[self].last_eat + s->tsleep;
	if (sleep > s->tdie)
	{
		printf("%ld %d is sleeping for %ld\n", time, self, sleep - (sleep - s->tdie));
		usleep(sleep - (sleep - s->tdie) * 1000);
		pthread_mutex_lock(&s->master);
		s->sym_state = sym_halt;
		pthread_mutex_unlock(&s->master);
		return (0);
	}
	printf("%ld %d is sleeping for %ld\n", time, self, (long)s->tsleep);
	usleep(s->tsleep * 1000);
	return (1);
}

void	pthink(t_sym *s, int self)
{
	printf("%ld %d is thinking\n", get_time(s), self);
}
