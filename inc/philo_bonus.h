/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 22:21:08 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/07 01:59:58 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <sys/time.h>

# define SEM_FORK "PHILO_SEM_FORK"
# define SEM_MASTER "PHILO_SEM_MASTER"
# define SEM_DEAD "PHILO_SEM_DEAD"

enum e_symstate
{
	sym_halt = 0,
	sym_running = 1
};

typedef struct s_philo
{
	pid_t		id;
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
	int				pdone;
	sem_t			*died;
	enum e_symstate	sym_state;
	sem_t			*fork;
	sem_t			*master;
	t_philo			*philos;
}				t_sym;

void	*philo_main(t_sym *s, int self);
long	get_time(t_sym *s);
int		check_state(t_sym *s, int self);
int		peat(t_sym *s, int self);
int		psleep(t_sym *s, int self);
void	pthink(t_sym *s, int self);

#endif
