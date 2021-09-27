/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 22:21:08 by mgueifao          #+#    #+#             */
/*   Updated: 2021/09/25 02:09:33 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

// enum e_symstate
// {
// 	sym_halt = 0,
// 	sym_init = 1,
// 	sym_running = 2
// };

// typedef struct s_philo
// {
// 	pthread_t		id;
// 	long			last_eat;
// 	int				eat_count;
// }				t_philo;

// typedef struct s_sym{
// 	int				pcount;
// 	int				tdie;
// 	int				teat;
// 	int				tsleep;
// 	int				eat_count;
// 	int				died;
// 	struct timeval	start;
// 	struct timeval	curr;
// 	long			currt;
// 	int				pdone;
// 	enum e_symstate	sym_state;
// 	t_philo			*philos;
// 	pthread_mutex_t	*forks;
// 	pthread_mutex_t	master;
// }				t_sym;

typedef struct s_sym
{
	int		pcount;
	int		tdie;
	int		tsleep;
	int		teat;
	int		eat_count;
	long	currt;
}				t_sym;


void	*philo_main(void *arg);
int		peat(t_sym *s, int self);
int		psleep(t_sym *s, int self);
void	pthink(t_sym *s, int self);

#endif
