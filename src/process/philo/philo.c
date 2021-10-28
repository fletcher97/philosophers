/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 23:04:25 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/28 02:46:47 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static long	get_time(t_sym *s)
{
	gettimeofday(&s->curr, NULL);
	return ((s->curr.tv_sec - s->start.tv_sec) * 1000
		+ (s->curr.tv_usec - s->start.tv_usec) / 1000);
}

static void	p_usleep(t_sym *s, int self, int t)
{
	long time;

	if (t <= 0)
		return ;
	time = get_time(s);
	if (time + t > s->philo.last_eat + s->tdie)
	{
		time = (s->philo.last_eat + s->tdie - time + 1) * 1000;
		if (time > 0)
			usleep(time);
		sem_wait(s->sem_dmsg);
		printf("%ld %d died\n", get_time(s), self + 1);
		sem_post(s->sem_died);
		while (1)
			;
	}
	usleep(t);
}

static void	p_eat(t_sym *s, int self)
{
	if ((s->philo.last_eat > 0 && s->teat * 2 >= s->tdie)
		|| (s->philo.last_eat > 0 && s->teat * 3 >= s->tdie && s->pcount == 3))
		p_usleep(s, self, s->tdie);
	sem_wait(s->sem_master);
	sem_wait(s->sem_fork);
	printf("%ld %d has taken a fork\n", get_time(s), self + 1);
	sem_wait(s->sem_fork);
	printf("%ld %d has taken a fork\n", get_time(s), self + 1);
	sem_post(s->sem_master);
	s->philo.last_eat = get_time(s);
	printf("%ld %d is eating\n", s->philo.last_eat, self + 1);
	p_usleep(s, self, s->teat * 1000);
	s->philo.eat_count++;
	if (s->philo.eat_count == s->eat_count)
		sem_post(s->sem_done);
	sem_post(s->sem_fork);
	sem_post(s->sem_fork);
}

void	philo_main(t_sym *s, int self)
{
	if (s->pcount == 1)
	{
		printf("%ld %d has taken a fork\n", get_time(s), self + 1);
		usleep(s->tdie);
		printf("%ld %d died\n", get_time(s), self + 1);
		sem_post(s->sem_died);
		while (1)
			;
	}
	while (1)
	{
		p_eat(s, self);
		printf("%ld %d is sleeping\n", get_time(s), self + 1);
		p_usleep(s, self, s->tsleep * 1000);
		printf("%ld %d is thinking\n", get_time(s), self + 1);
	}
}
