/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 22:17:02 by mgueifao          #+#    #+#             */
/*   Updated: 2021/09/23 02:28:51 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "philo.h"
#include "common.h"

int	parse_args(int argc, const char *argv[], t_sym *s)
{
	int i;

	if (!ft_isnumber(argv[1]) || !ft_isnumber(argv[2]) || !ft_isnumber(argv[3])
		|| !ft_isnumber(argv[4]) || (argc == 6 && !ft_isnumber(argv[5])))
		return (0);
	s->eat_count = 0;
	if (argc == 6)
		s->eat_count = atoi(argv[5]);
	s->sym_state = sym_init;
	s->pdone = 0;
	s->pcount = atoi(argv[1]);
	s->tdie = atoi(argv[2]);
	s->teat = atoi(argv[3]);
	s->tsleep = atoi(argv[4]);
	s->philos = calloc(s->pcount, sizeof(t_philo));
	s->forks = calloc(s->pcount, sizeof(t_philo));
	if (!s->philos || !s->forks)
		return (0);
	i = -1;
	while (++i < s->pcount)
		pthread_mutex_init(&s->forks[i], NULL);
	pthread_mutex_init(&s->master, NULL);
	return (1);
}

int	create_philos(t_sym *s)
{
	int	i;

	i = -1;
	while (++i < s->pcount)
	{
		if (pthread_create(&(s->philos[i].id), NULL, philo_main, s))
			return (0);
	}
	return (1);
}

int	join_philos(t_sym *s)
{
	int	i;

	i = -1;
	while (1)
	{
		pthread_mutex_lock(&s->master);
		if (s->sym_state == sym_halt)
		{
			pthread_mutex_unlock(&s->master);
			break;
		}
		pthread_mutex_unlock(&s->master);
		usleep(10);
	}
	while (++i < s->pcount)
		if (pthread_join(s->philos[i].id, NULL))
			printf("Join error.\n");
	return (1);
}

void	clear(t_sym *s)
{
	int	i;

	if (!s)
		return ;
	i = -1;
	while (++i < s->pcount)
	{
		pthread_cancel(s->philos[i].id);
		pthread_mutex_destroy(&s->forks[i]);
	}
	pthread_mutex_destroy(&s->master);
	free(s->philos);
	free(s->forks);
	free(s);
}

int	main(int argc, char const *argv[])
{
	t_sym	*s;

	if (argc < 5 || argc > 6)
		return (1);
	s = calloc(1, sizeof(t_sym));
	if (!s || !parse_args(argc, argv, s) || !create_philos(s))
	{
		clear(s);
		return (1);
	}
	pthread_mutex_lock(&s->master);
	gettimeofday(&s->start, NULL);
	s->sym_state = sym_running;
	pthread_mutex_unlock(&s->master);
	join_philos(s);
	clear(s);
	return (0);
}
