/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 22:21:08 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/27 23:28:39 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

enum e_symstate
{
	sym_halt = 0,
	sym_running = 1
};

typedef struct s_philo
{
	pthread_t	id;
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
	int				claim;
	int				start_parity;
	struct timeval	start;
	struct timeval	curr;
	long			currt;
	int				pdone;
	int				died;
	enum e_symstate	sym_state;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	master;
}				t_sym;

void	*philo_main(void *arg);
long	get_time(t_sym *s);
int		check_state(t_sym *s, int self);
int		peat(t_sym *s, int self);
int		psleep(t_sym *s, int self);
void	pthink(t_sym *s, int self);
int		tr_usleep(t_sym *s, int self, long t);

#endif
