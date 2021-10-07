/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_ac.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 00:50:44 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/07 01:09:40 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

#include "philo.h"

static int	get_forks(t_sym *s, int self)
{
	long	time;

	if (pthread_mutex_trylock(&s->forks[self]))
		return (0);
	if (!pthread_mutex_trylock(&s->forks[(self + 1) % s->pcount]))
	{
		time = get_time(s);
		printf("%ld %d has taken a fork\n", time, self + 1);
		printf("%ld %d has taken a fork\n", time, self + 1);
		return (1);
	}
	pthread_mutex_unlock(&s->forks[self]);
	return (0);
}

int	peat(t_sym *s, int self)
{
	while (!get_forks(s, self))
		if (!check_state(s, self))
			return (0);
	s->philos[self].last_eat = get_time(s);
	printf("%ld %d is eating\n", s->philos[self].last_eat, self + 1);
	usleep(s->teat * 1000);
	if (s->eat_count != -1 && ++s->philos[self].eat_count == s->eat_count)
	{
		pthread_mutex_lock(&s->master);
		s->pdone++;
		pthread_mutex_unlock(&s->master);
	}
	pthread_mutex_unlock(&s->forks[self]);
	pthread_mutex_unlock(&s->forks[(self + 1) % s->pcount]);
	return (1);
}

int	psleep(t_sym *s, int self)
{
	long	time;

	time = get_time(s);
	printf("%ld %d is sleeping\n", time, self + 1);
	if (time + s->tsleep > s->philos[self].last_eat + s->tdie)
	{
		usleep((s->philos[self].last_eat + s->tdie - time + 1) * 1000);
		check_state(s, self);
		return (0);
	}
	usleep(s->tsleep * 1000);
	return (1);
}

void	pthink(t_sym *s, int self)
{
	printf("%ld %d is thinking\n", get_time(s), self + 1);
}
