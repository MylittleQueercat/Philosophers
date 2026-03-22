/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:11:22 by hguo              #+#    #+#             */
/*   Updated: 2025/09/24 20:49:47 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_ctx *ctx)
{
	int		i;
	long	since;
	long	now;

	i = 0;
	now = now_ms();
	while (i < ctx->num_philos)
	{
		since = now - get_last_meal(&ctx->philo[i]);
		if (since > ctx->t_die)
		{
			set_stopped(ctx);
			print_died(ctx, ctx->philo[i].id);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_full(t_ctx *ctx)
{
	int	all;

	all = 0;
	if (ctx->must_eat < 1)
		return (0);
	pthread_mutex_lock(&ctx->state_mtx);
	if (ctx->full_count >= ctx->num_philos)
		all = 1;
	pthread_mutex_unlock(&ctx->state_mtx);
	if (all)
	{
		set_stopped(ctx);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_ctx	*ctx;

	ctx = (t_ctx *)arg;
	while (!safe_get_stopped(ctx))
	{
		if (check_death(ctx) || check_full(ctx))
			break ;
		usleep(50);
	}
	return (NULL);
}

int	join_threads(t_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->num_philos)
		pthread_join(ctx->philo[i++].thread, NULL);
	pthread_join(ctx->mon, NULL);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->ctx->num_philos == 1)
		return ((void *)(long)one_philo_flow(p));
	if ((p->id % 2) == 0)
		ms_sleep(5, p->ctx);
	while (!safe_get_stopped(p->ctx))
	{
		print_action(p->ctx, p->id, "is thinking 💭");
		if (!eat_block(p))
			break ;
		print_action(p->ctx, p->id, "is sleeping 😴");
		ms_sleep(p->ctx->t_sleep, p->ctx);
	}
	return (NULL);
}
