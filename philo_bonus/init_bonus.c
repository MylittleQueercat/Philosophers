/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:13:18 by hguo              #+#    #+#             */
/*   Updated: 2025/09/26 17:19:06 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	validate_args(char **av)
{
	int	v;
	int	w;

	v = 1;
	while (av[v])
	{
		if (atoiv(av[v]) <= 0)
		{
			philo_error(ERR_ARG_NONPOS);
		}
		w = 0;
		while (av[v][w])
		{
			if (ft_isdigit(av[v][w]) == 0)
			{
				philo_error(ERR_ARG_DIGIT);
			}
			w++;
		}
		v++;
	}
}

static void	init_resource(char **av, int ac, t_resource *resource)
{
	resource->num_philos = atoiv(av[1]);
	resource->t_die = atoiv(av[2]);
	resource->t_eat = atoiv(av[3]);
	resource->t_sleep = atoiv(av[4]);
	resource->start_time = get_time();
	resource->print_death = 0;
	if (resource->start_time == 1)
		philo_error(ERR_TIME);
	if (ac == 6)
		resource->must_eat = atoiv(av[5]);
	else
		resource->must_eat = -1;
}

static void	alloc_resource(t_resource *resource)
{
	int	num_of_philos;
	int	i;

	num_of_philos = resource->num_philos;
	resource->philo = malloc(sizeof(t_philo) * num_of_philos);
	if (!resource->philo)
		philo_error(ERR_ALLOC);
	resource->start_time = get_time();
	i = 0;
	while (i < num_of_philos)
	{
		resource->philo[i].id = i + 1;
		resource->philo[i].meals_eaten = 0;
		resource->philo[i].last_meal_time = get_time();
		resource->philo[i].resource = resource;
		i++;
	}
}

static void	init_all_sems(t_resource *resource)
{
	sem_unlink("/write");
	sem_unlink("/forks");
	sem_unlink("/eat");
	sem_unlink("/cmp_finish");
	sem_unlink("/stopped");
	resource->write = sem_open("/write", O_CREAT, 0644, 1);
	if (resource->write == SEM_FAILED)
		philo_error(ERR_SEM);
	resource->forks = sem_open("/forks", O_CREAT, 0644, resource->num_philos);
	if (resource->forks == SEM_FAILED)
		philo_error(ERR_SEM);
	resource->eat = sem_open("/eat", O_CREAT, 0644, 1);
	if (resource->eat == SEM_FAILED)
		philo_error(ERR_SEM);
	resource->cmp_finish = sem_open("/cmp_finish", O_CREAT, 0644, 1);
	if (resource->cmp_finish == SEM_FAILED)
		philo_error(ERR_SEM);
	resource->stopped = sem_open("/stopped", O_CREAT, 0644, 0);
	if (resource->stopped == SEM_FAILED)
		philo_error(ERR_SEM);
}

void	philo_setup(int ac, char **av, t_resource *resource)
{
	init_resource(av, ac, resource);
	alloc_resource(resource);
	init_all_sems(resource);
}
