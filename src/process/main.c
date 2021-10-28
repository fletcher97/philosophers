/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 02:15:15 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/28 02:29:25 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "philo_bonus.h"

static int	sem(t_sym *s)
{
	sem_unlink(SEM_DEAD);
	sem_unlink(SEM_MASTER);
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_DONE);
	sem_unlink(SEM_DMSG);
	s->sem_died = sem_open(SEM_DEAD, O_CREAT, 600, 0);
	s->sem_master = sem_open(SEM_MASTER, O_CREAT, 600, 1);
	s->sem_fork = sem_open(SEM_FORK, O_CREAT, 600, s->pcount);
	s->sem_done = sem_open(SEM_DONE, O_CREAT, 600, 0);
	s->sem_dmsg = sem_open(SEM_DMSG, O_CREAT, 600, 1);
	return ((s->pids = ft_calloc(sizeof(pid_t), s->pcount))
		&& (s->tr_done = ft_calloc(sizeof(pthread_t), 1))
		&& (s->tr_rip = ft_calloc(sizeof(pthread_t), 1)));
}

static int	check_args(int argc, const char *argv[], t_sym **sym)
{
	t_sym	*s;

	if (!ft_isnumber(argv[1]) || !ft_isnumber(argv[2]) || !ft_isnumber(argv[3])
		|| !ft_isnumber(argv[4]) || (argc == 6 && !ft_isnumber(argv[5])))
		return (0);
	*sym = ft_calloc(sizeof(t_sym), 1);
	s = *sym;
	if (!s)
		return (0);
	s->eat_count = -1;
	if (argc == 6)
		s->eat_count = atoi(argv[5]);
	s->pcount = atoi(argv[1]);
	s->tdie = atoi(argv[2]);
	s->teat = atoi(argv[3]);
	s->tsleep = atoi(argv[4]);
	if (s->pcount < 1 || s->tdie < 1 || s->teat < 0 || s->tsleep < 0
		|| s->eat_count < -1 || (argc == 6 && s->eat_count < 1))
		return (0);
	return (sem(s));
}

static int	clear(t_sym *s)
{
	if (!s)
		return (1);
	if (s->pids)
		free(s->pids);
	if (s->sem_died)
		sem_close(s->sem_died);
	if (s->sem_dmsg)
		sem_close(s->sem_dmsg);
	if (s->sem_done)
		sem_close(s->sem_done);
	if (s->sem_fork)
		sem_close(s->sem_fork);
	if (s->sem_master)
		sem_close(s->sem_master);
	free(s);
	return (0);
}

static void	start(t_sym *s)
{
	int	i;

	pthread_create(s->tr_rip, NULL, ripper, s);
	pthread_create(s->tr_done, NULL, completionist, s);
	i = 0;
	while (i < s->pcount)
	{
		s->pids[i] = fork();
		if (!s->pids[i])
			philo_main(s, i);
		i++;
	}
	sem_post(s->sem_died);
	sem_post(s->sem_done);
	pthread_join(*s->tr_rip, NULL);
	pthread_join(*s->tr_done, NULL);
	clear(s);
}

int	main(int ac, char const *argv[])
{
	t_sym	*s;

	s = NULL;
	if (ac < 5 || ac > 6 || !check_args(ac, argv, &s))
		return (clear(s));
	gettimeofday(&s->start, NULL);
	start(s);
	return (0);
}
