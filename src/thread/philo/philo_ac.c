/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_ac.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 00:50:44 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/28 03:04:23 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

#include "philo.h"

int	tr_usleep(t_sym *s, int self, long t)
{
	long	start;
	long	curr;
	long	sleep;

	start = get_time(s);
	curr = start;
	sleep = 100;
	while ((start + t) > curr)
	{
		if (!check_state(s, self))
			break ;
		curr = get_time(s);
		(((curr + 50) < (start + t)) && (sleep = 50));
		if ((curr + sleep) < (start + t)
			&& curr + sleep - s->philos[self].last_eat < s->tdie)
			usleep(sleep);
		else
			sleep -= 10;
		curr = get_time(s);
	}
	return (1);
}

static int	can_wait(t_sym *s, int self)
{
	if (s->philos[self].last_eat > 0 && s->teat * 2 > s->tdie)
		return (0);
	if (s->philos[self].last_eat > 0 && s->teat * 3 > s->tdie && s->pcount == 3)
		return (0);
	return (1);
}

static int	get_forks(t_sym *s, int self)
{
	int		i;
	int		bin;

	(void)bin;
	if (!can_wait(s, self) && tr_usleep(s, self, s->tdie))
		return (0);
	bin = (((self % 2)
				&& (i = self)) || (i = (self + 1) % s->pcount));
	pthread_mutex_lock(&s->forks[i]);
	if (!check_state(s, self))
		return (pthread_mutex_unlock(&s->forks[i]) && 0);
	printf("%ld %d has taken a fork\n", get_time(s), self + 1);
	bin = (((self % 2) && ((i = (self + 1) % s->pcount) || 1)) || (i = self));
	pthread_mutex_lock(&s->forks[i]);
	if (!check_state(s, self))
	{
		pthread_mutex_unlock(&s->forks[self]);
		pthread_mutex_unlock(&s->forks[(self + 1) % s->pcount]);
		return (0);
	}
	printf("%ld %d has taken a fork\n", get_time(s), self + 1);
	return (1);
}

int	peat(t_sym *s, int self)
{
	if (!get_forks(s, self))
		return (0);
	pthread_mutex_lock(&s->master);
	(!s->start_parity) && (((self % 2) && (s->start_parity = 1))
		|| (s->start_parity = 2));
	pthread_mutex_unlock(&s->master);
	s->philos[self].last_eat = get_time(s);
	printf("%ld %d is eating\n", s->philos[self].last_eat, self + 1);
	tr_usleep(s, self, s->teat);
	if (s->eat_count != -1 && ++s->philos[self].eat_count == s->eat_count)
	{
		pthread_mutex_lock(&s->master);
		s->pdone++;
		pthread_mutex_unlock(&s->master);
	}
	pthread_mutex_unlock(&s->forks[self]);
	pthread_mutex_unlock(&s->forks[(self + 1) % s->pcount]);
	if (!check_state(s, self))
		return (0);
	return (1);
}

int	psleep(t_sym *s, int self)
{
	long	time;

	time = get_time(s);
	printf("%ld %d is sleeping\n", time, self + 1);
	tr_usleep(s, self, s->tsleep);
	if (!check_state(s, self))
		return (0);
	return (1);
}
