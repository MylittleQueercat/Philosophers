/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:12:52 by hguo              #+#    #+#             */
/*   Updated: 2025/09/24 20:58:24 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	one_philo_flow(t_philo *p)
{
	if (safe_get_stopped(p->ctx))
		return (1);
	print_action(p->ctx, p->id, "is thinking 💭");
	pthread_mutex_lock(p->left_fork);
	print_action(p->ctx, p->id, "has taken a fork 🍴");
	ms_sleep(p->ctx->t_die, p->ctx);
	pthread_mutex_unlock(p->left_fork);
	return (1);
}

int	should_yield(t_philo *p, pthread_mutex_t *fork)
{
	long	now;
	long	self_age;
	long	left_age;
	long	right_age;
	t_ctx	*ctx;

	ctx = p->ctx;
	now = now_ms();
	pthread_mutex_lock(&ctx->state_mtx);
	self_age = now - p->last_meal_time;
	left_age = now - ctx->philo[(p->id - 1 + ctx->num_philos)
		% ctx->num_philos].last_meal_time;
	right_age = now - ctx->philo[(p->id + 1) % ctx->num_philos].last_meal_time;
	pthread_mutex_unlock(&ctx->state_mtx);
	if (self_age > ctx->t_die * 0.9)
		return (0);
	if (fork == p->left_fork && left_age - ctx->t_sleep + 10 > self_age)
		return (1);
	if (fork == p->right_fork && right_age - ctx->t_sleep + 10 > self_age)
		return (1);
	return (0);
}

int	pick_forks(t_philo *p)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	first = p->left_fork;
	second = p->right_fork;
	if ((p->id % 2) == 0)
	{
		first = p->right_fork;
		second = p->left_fork;
	}
	if (p->ctx->num_philos == 1)
		return (0);
	if (should_yield(p, first))
		ms_sleep(5, p->ctx);
	pthread_mutex_lock(first);
	print_action(p->ctx, p->id, "has taken a fork 🍴");
	if (should_yield(p, second))
		ms_sleep(5, p->ctx);
	pthread_mutex_lock(second);
	print_action(p->ctx, p->id, "has taken a fork 🍴");
	return (1);
}

void	drop_forks(t_philo *p)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	first = p->left_fork;
	second = p->right_fork;
	if ((p->id % 2) == 0)
	{
		first = p->right_fork;
		second = p->left_fork;
	}
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

int	eat_block(t_philo *p)
{
	if (pick_forks(p) == 0)
		return (0);
	pthread_mutex_lock(&p->ctx->state_mtx);
	p->last_meal_time = now_ms();
	p->meals_eaten++;
	if (p->ctx->must_eat > 0 && p->meals_eaten == p->ctx->must_eat)
		p->ctx->full_count++;
	pthread_mutex_unlock(&p->ctx->state_mtx);
	print_action(p->ctx, p->id, "is eating 🍝");
	ms_sleep(p->ctx->t_eat, p->ctx);
	drop_forks(p);
	return (1);
}
