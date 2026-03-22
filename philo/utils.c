/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:13:00 by hguo              #+#    #+#             */
/*   Updated: 2025/09/24 20:48:34 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	now_ms(void)
{
	struct timeval	tv;
	long			ms;
	long			sec;
	long			usec;

	sec = 0;
	usec = 0;
	ms = 0;
	gettimeofday(&tv, NULL);
	sec = tv.tv_sec;
	usec = tv.tv_usec;
	ms = sec * 1000 + usec / 1000;
	return (ms);
}

void	ms_sleep(long ms, t_ctx *ctx)
{
	long	start;
	long	elapsed;

	start = now_ms();
	elapsed = 0;
	while (elapsed < ms)
	{
		if (safe_get_stopped(ctx))
			return ;
		usleep(50);
		elapsed = now_ms() - start;
	}
}

int	safe_get_stopped(t_ctx *ctx)
{
	int	val;

	val = 0;
	pthread_mutex_lock(&ctx->state_mtx);
	if (ctx->stopped)
		val = 1;
	pthread_mutex_unlock(&ctx->state_mtx);
	return (val);
}

void	set_stopped(t_ctx *ctx)
{
	pthread_mutex_lock(&ctx->state_mtx);
	ctx->stopped = true;
	pthread_mutex_unlock(&ctx->state_mtx);
}

long	get_last_meal(t_philo *p)
{
	long	t;

	t = 0;
	pthread_mutex_lock(&p->ctx->state_mtx);
	t = p->last_meal_time;
	pthread_mutex_unlock(&p->ctx->state_mtx);
	return (t);
}
