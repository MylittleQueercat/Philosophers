/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:13:46 by hguo              #+#    #+#             */
/*   Updated: 2025/09/28 15:41:05 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	terminate_all(t_resource *r, int sig)
{
	int	i;

	i = 0;
	while (i < r->num_philos)
	{
		if (r->philo[i].pid > 0)
		{
			kill(r->philo[i].pid, sig);
		}
		i++;
	}
}

static void	kill_and_cleanup(t_resource *r)
{
	terminate_all(r, SIGTERM);
	usleep(100000);
	terminate_all(r, SIGKILL);
	while (wait(NULL) > 0)
		;
	free_resource(r);
	philo_error(ERR_FORK);
}

void	creat_proces(t_resource *resource)
{
	int	v;

	v = 0;
	if (resource->num_philos == 1)
	{
		print_status(resource->philo, FORK);
		usleep(resource->t_die * 1000);
		print_status(resource->philo, DIE);
		child_cleanup(resource);
		exit(1);
	}
	while (v < resource->num_philos)
	{
		resource->philo[v].pid = fork();
		if (resource->philo[v].pid == 0)
			routine(&resource->philo[v]);
		else if (resource->philo[v].pid < 0)
			kill_and_cleanup(resource);
		usleep(200);
		v++;
	}
}

int	check_finish(t_philo *philo, int status)
{
	sem_wait(philo->resource->cmp_finish);
	if (status == 2)
	{
		sem_post(philo->resource->cmp_finish);
		child_cleanup(philo->resource);
		exit(1);
	}
	if (status == 1)
	{
		sem_post(philo->resource->cmp_finish);
		child_cleanup(philo->resource);
		exit(0);
	}
	sem_post(philo->resource->cmp_finish);
	return (0);
}

int	check_death(t_philo *philo)
{
	int	code;

	code = 0;
	sem_wait(philo->resource->eat);
	if (get_time() - philo->last_meal_time > philo->resource->t_die)
	{
		code = 2;
	}
	else if (philo->resource->must_eat != -1
		&& philo->meals_eaten >= philo->resource->must_eat)
	{
		code = 1;
	}
	sem_post(philo->resource->eat);
	return (code);
}
