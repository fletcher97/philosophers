/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgueifao <mgueifao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 01:18:55 by mgueifao          #+#    #+#             */
/*   Updated: 2021/10/19 01:16:51 by mgueifao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

static int	of_s(size_t s, size_t n)
{
	size_t	m;

	if (!s)
		return (0);
	m = (size_t)(-1);
	return ((m / s) < n);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*p;

	p = malloc(nmemb * size);
	if (of_s(nmemb, size) || !p)
		return (NULL);
	ft_memset(p, 0, nmemb * size);
	return ((void *)p);
}
