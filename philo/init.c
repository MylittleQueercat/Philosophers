/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:10:58 by hguo              #+#    #+#             */
/*   Updated: 2025/09/24 19:11:00 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(const char *nptr)
{
	int		sign;
	long	num;

	sign = 1;
	num = 0;
	while (*nptr && *nptr == ' ')
		nptr ++;
	if (*nptr && (*nptr == '-' || *nptr == '+'))
	{
		if (*nptr == '-')
			sign *= -1;
		nptr ++;
	}
	while (*nptr && *nptr >= '0' && *nptr <= '9')
	{
		num = num * 10 + (*nptr - '0');
		nptr ++;
	}
	return (sign * num);
}

int	parse_args(t_ctx *ctx, int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (1);
	ctx->num_philos = (int)ft_atol(av[1]);
	ctx->t_die = ft_atol(av[2]);
	ctx->t_eat = ft_atol(av[3]);
	ctx->t_sleep = ft_atol(av[4]);
	ctx->must_eat = -1;
	if (ac == 6)
		ctx->must_eat = (int)ft_atol(av[5]);
	if (ctx->num_philos < 1 || ctx->t_die < 1 || ctx->t_eat < 0
		|| ctx->t_sleep < 0)
		return (1);
	if (ac == 6 && ctx->must_eat < 1)
		return (1);
	return (0);
}

int	alloc_ctx(t_ctx *ctx)
{
	ctx->forks = NULL;
	ctx->philo = NULL;
	ctx->stopped = false;
	ctx->full_count = 0;
	ctx->mon = 0;
	ctx->forks = malloc(sizeof(pthread_mutex_t) * ctx->num_philos);
	if (!ctx->forks)
		return (1);
	ctx->philo = malloc(sizeof(t_philo) * ctx->num_philos);
	if (!ctx->philo)
		return (1);
	return (0);
}

int	init_mutexes(t_ctx *ctx)
{
	int	i;
	int	ok;

	i = 0;
	ok = 0;
	if (pthread_mutex_init(&ctx->print_mtx, NULL) != 0)
		ok = 1;
	if (pthread_mutex_init(&ctx->state_mtx, NULL) != 0)
		ok = 1;
	while (i < ctx->num_philos && ok == 0)
		if (pthread_mutex_init(&ctx->forks[i++], NULL) != 0)
			ok = 1;
	if (ok != 0)
		return (1);
	return (0);
}

int	init_philos(t_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->num_philos)
	{
		ctx->philo[i].id = i;
		ctx->philo[i].meals_eaten = 0;
		ctx->philo[i].thread = 0;
		ctx->philo[i].ctx = ctx;
		ctx->philo[i].left_fork = &ctx->forks[i];
		ctx->philo[i].right_fork = &ctx->forks[(i + 1) % ctx->num_philos];
		ctx->philo[i].last_meal_time = 0;
		i++;
	}
	return (0);
}
