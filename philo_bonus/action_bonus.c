/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:13:12 by hguo              #+#    #+#             */
/*   Updated: 2025/09/28 16:12:22 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	take_forks(t_philo *philo)
{
	sem_wait(philo->resource->forks);
	print_status(philo, PINK FORK RESET);
	sem_wait(philo->resource->forks);
	print_status(philo, PINK FORK RESET);
}

static void	drop_forks(t_philo *philo)
{
	sem_post(philo->resource->forks);
	sem_post(philo->resource->forks);
}

static void	to_eat(t_philo *philo)
{
	sem_wait(philo->resource->eat);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	sem_post(philo->resource->eat);
	print_status(philo, GREEN EAT RESET);
	usleep(philo->resource->t_eat * 1000);
}

static void	to_sleep(t_philo *philo)
{
	print_status(philo, BLEU SLEEP RESET);
	usleep(philo->resource->t_sleep * 1000);
	if (philo->resource->num_philos % 2 == 1)
		usleep(200);
}

void	routine(t_philo *philo)
{
	int	code;

	if (pthread_create(&philo->checker, NULL, philo_monitor, philo) != 0)
		philo_error("pthread_create error\n");
	print_status(philo, BLUE THINK RESET);
	if (philo->id % 2 == 0)
		usleep(philo->resource->t_eat * 1000);
	while (get_stopped_locked(philo->resource) == 0)
	{
		take_forks(philo);
		to_eat(philo);
		drop_forks(philo);
		to_sleep(philo);
		print_status(philo, BLUE THINK RESET);
	}
	pthread_join(philo->checker, NULL);
	sem_wait(philo->resource->cmp_finish);
	code = 0;
	if (is_stopped(philo->resource))
		code = 1;
	sem_post(philo->resource->cmp_finish);
	child_cleanup(philo->resource);
	exit(code);
}
