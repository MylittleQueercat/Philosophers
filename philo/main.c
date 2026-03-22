/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:11:08 by hguo              #+#    #+#             */
/*   Updated: 2025/09/24 21:01:08 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_ctx(t_ctx *ctx)
{
	int	i;

	i = 0;
	if (ctx->forks)
	{
		while (i < ctx->num_philos)
			pthread_mutex_destroy(&ctx->forks[i++]);
		pthread_mutex_destroy(&ctx->print_mtx);
		pthread_mutex_destroy(&ctx->state_mtx);
		free(ctx->forks);
	}
	if (ctx->philo)
		free(ctx->philo);
}

int	start_threads(t_ctx *ctx)
{
	int			i;
	int			ok;

	i = 0;
	ok = 0;
	if (pthread_create(&ctx->mon, NULL, monitor_routine, ctx) != 0)
		ok = 1;
	while (i < ctx->num_philos && ok == 0)
	{
		if (pthread_create(&ctx->philo[i].thread, NULL, philo_routine,
				&ctx->philo[i]) != 0)
			ok = 1;
		i++;
	}
	if (ok != 0)
		set_stopped(ctx);
	return (ok);
}

int	start_simulation(t_ctx *ctx)
{
	int	i;

	i = 0;
	ctx->start_time = now_ms();
	while (i < ctx->num_philos)
		ctx->philo[i++].last_meal_time = ctx->start_time;
	if (start_threads(ctx) != 0)
		return (1);
	if (join_threads(ctx) != 0)
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_ctx	ctx;

	ctx.philo = NULL;
	ctx.forks = NULL;
	if (parse_args(&ctx, ac, av) != 0)
		return (1);
	if (alloc_ctx(&ctx) != 0)
		return (cleanup_ctx(&ctx), 1);
	if (init_mutexes(&ctx) != 0)
		return (cleanup_ctx(&ctx), 1);
	if (init_philos(&ctx) != 0)
		return (cleanup_ctx(&ctx), 1);
	if (start_simulation(&ctx) != 0)
		return (cleanup_ctx(&ctx), 1);
	cleanup_ctx(&ctx);
	return (0);
}
