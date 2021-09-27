/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 01:32:40 by mgueifao          #+#    #+#             */
/*   Updated: 2021/09/22 22:30:39 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	isspace(int c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r' || c == ' ');
}

int	atoi(const char *str)
{
	int	pos;
	int	sign;
	int	ret;

	sign = 1;
	ret = 0;
	pos = 0;
	while (isspace(str[pos]))
		pos++;
	if (str[pos] == '-' || str[pos] == '+')
		if (str[pos++] == '-')
			sign *= -1;
	while (str[pos] >= '0' && str[pos] <= '9')
		ret = (ret * 10) + (str[pos++] - '0') * sign;
	return (ret);
}
