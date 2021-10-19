/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 02:15:15 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/19 01:16:26 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "philo_bonus.h"

static int check_args(int argc, const char *argv[], t_sym **sym)
{
	t_sym *s;

	if (!ft_isnumber(argv[1]) || !ft_isnumber(argv[2]) || !ft_isnumber(argv[3])
		|| !ft_isnumber(argv[4]) || (argc == 6 && !ft_isnumber(argv[5])))
		return (0);
	*sym = ft_calloc(sizeof(t_sym), 1);
	if (!(*sym))
		return (0);
	s = *sym;
	s->eat_count = -1;
	if (argc == 6)
		s->eat_count = atoi(argv[5]);
	s->sym_state = sym_running;
	s->pdone = 0;
	s->pcount = atoi(argv[1]);
	s->tdie = atoi(argv[2]);
	s->teat = atoi(argv[3]);
	s->tsleep = atoi(argv[4]);
	return (1);
}

int main(int argc, char const *argv[])
{
	t_sym	*s;

	if (argc < 5 || argc > 6 || !check_args(argc, argv, &s))
		return (1);
	return (0);
}
// init_coms(s);
// init_procs(s);
// join(s);
