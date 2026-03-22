/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_moni_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 16:01:49 by hguo              #+#    #+#             */
/*   Updated: 2025/09/28 16:11:54 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	is_stopped(t_resource *r)
{
	int	v;

	v = 0;
	sem_getvalue(r->stopped, &v);
	return (v > 0);
}

void	set_stopped(t_resource *r)
{
	int	v;

	v = 0;
	sem_getvalue(r->stopped, &v);
	if (v == 0)
		sem_post(r->stopped);
}

void	*philo_monitor(void *philos)
{
	t_philo	*philo;
	int		st;

	philo = (t_philo *)philos;
	while (!is_stopped(philo->resource))
	{
		st = check_death(philo);
		if (st == 2)
		{
			sem_wait(philo->resource->write);
			printf("[%ld]	 NO.%d     %s",
				get_time() - philo->resource->start_time, philo->id, DIE);
			fflush(stdout);
			set_stopped(philo->resource);
			return (NULL);
		}
		if (st == 1)
		{
			set_stopped(philo->resource);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}

int	get_stopped_locked(t_resource *r)
{
	int	v;

	v = 0;
	sem_getvalue(r->stopped, &v);
	return (v > 0);
}
