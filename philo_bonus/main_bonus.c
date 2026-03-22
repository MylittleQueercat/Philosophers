/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:13:23 by hguo              #+#    #+#             */
/*   Updated: 2025/09/28 16:00:55 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	child_cleanup(t_resource *r)
{
	if (r->eat)
		sem_close(r->eat);
	if (r->write)
		sem_close(r->write);
	if (r->forks)
		sem_close(r->forks);
	if (r->cmp_finish)
		sem_close(r->cmp_finish);
	if (r->stopped)
		sem_close(r->stopped);
	if (r->philo)
	{
		free(r->philo);
		r->philo = NULL;
	}
}

void	free_resource(t_resource *resource)
{
	free(resource->philo);
	sem_close(resource->eat);
	sem_close(resource->write);
	sem_close(resource->forks);
	sem_close(resource->cmp_finish);
	sem_close(resource->stopped);
	sem_unlink("/write");
	sem_unlink("/forks");
	sem_unlink("/eat");
	sem_unlink("/cmp_finish");
	sem_unlink("/stopped");
}

int	main(int ac, char **av)
{
	t_resource	resource;
	int			i;

	if (ac == 5 || ac == 6)
	{
		validate_args(av);
		philo_setup(ac, av, &resource);
		creat_proces(&resource);
	}
	else
		philo_error(ERR_ARG_COUNT);
	sem_wait(resource.stopped);
	i = 0;
	while (i < resource.num_philos)
	{
		kill(resource.philo[i].pid, SIGKILL);
		i++;
	}
	free_resource(&resource);
}
