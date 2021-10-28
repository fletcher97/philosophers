/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 22:21:08 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/28 02:53:36 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdlib.h>
# include <stdio.h>
# include <semaphore.h>
# include <signal.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/types.h>
# include <fcntl.h>
# include <pthread.h>

# define SEM_FORK "PHILO_SEM_FORK"
# define SEM_MASTER "PHILO_SEM_MASTER"
# define SEM_DEAD "PHILO_SEM_DEAD"
# define SEM_DONE "PHILO_SEM_DONE"
# define SEM_DMSG "PHILO_SEM_DMSG"

typedef struct s_philo
{
	int			id;
	pid_t		parent;
	long		last_eat;
	int			eat_count;
}				t_philo;

typedef struct s_sym
{
	int				pcount;
	int				tdie;
	int				teat;
	int				tsleep;
	int				eat_count;
	struct timeval	start;
	struct timeval	curr;
	long			currt;
	sem_t			*sem_died;
	sem_t			*sem_dmsg;
	sem_t			*sem_fork;
	sem_t			*sem_done;
	sem_t			*sem_master;
	pid_t			*pids;
	pthread_t		*tr_rip;
	pthread_t		*tr_done;
	int				tr_me;
	t_philo			philo;
}				t_sym;

void	*ripper(void *sym);
void	*completionist(void *sym);

void	philo_main(t_sym *s, int self);

#endif
