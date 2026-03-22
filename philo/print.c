/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:12:45 by hguo              #+#    #+#             */
/*   Updated: 2025/09/25 16:44:33 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*quote_for(char *msg)
{
	if (strstr(msg, "eating") != 0)
		return ("Tell me what you eat, and I will tell you what you are.");
	else if (strstr(msg, "sleeping") != 0)
		return ("Even a soul in sleep works and helps make the world");
	else if (strstr(msg, "thinking") != 0)
		return ("I think, therefore I am.");
	else if (strstr(msg, "fork") != 0)
		return ("Without bread, all is misery.");
	return ("");
}

char	*color_for(char *msg)
{
	if (!msg)
		return (C_RESET);
	if (msg[0] == 'i')
	{
		if (msg[3] == 'e')
			return (C_BGREEN);
		if (msg[3] == 's')
			return (C_BYELLOW);
		if (msg[3] == 't')
			return (C_BBLUE);
	}
	if (msg[0] == 'h')
		return (C_BCYAN);
	if (msg[0] == 'd')
		return (C_BOLD BG_RED);
	return (C_BMAG);
}

void	print_action(t_ctx *ctx, int id, char *s)
{
	long	t;

	t = now_ms() - ctx->start_time;
	if (safe_get_stopped(ctx))
		return ;
	pthread_mutex_lock(&ctx->print_mtx);
	if (!safe_get_stopped(ctx))
	{
		printf("[%ld] \tNO.%d \t%s%s%s\n", t, id + 1, color_for(s), s, C_RESET);
		printf("\t\t%s%s%s\n", C_GRAY, quote_for(s), C_RESET);
	}
	pthread_mutex_unlock(&ctx->print_mtx);
}

void	print_died(t_ctx *ctx, int id)
{
	long	t;
	char	*s;

	s = "died ☠️";
	t = now_ms() - ctx->start_time;
	pthread_mutex_lock(&ctx->print_mtx);
	printf("[%ld] \tNO.%d \t%s%s%s\n", t, id + 1, color_for(s), s, C_RESET);
	pthread_mutex_unlock(&ctx->print_mtx);
}
