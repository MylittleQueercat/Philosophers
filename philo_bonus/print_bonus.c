/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:13:37 by hguo              #+#    #+#             */
/*   Updated: 2025/09/25 19:10:49 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

void	print_status(t_philo *philo, char *s)
{
	long	time;

	sem_wait(philo->resource->write);
	time = get_time() - philo->resource->start_time;
	ft_putstr_fd("[", 1);
	ft_putnbr_fd(time, 1);
	ft_putstr_fd("]    NO.", 1);
	ft_putnbr_fd(philo->id, 1);
	ft_putstr_fd("	  ", 1);
	printf("%s%s%s", C_BOLD, s, RESET);
	printf("\t\t  %s%s%s\n", C_GRAY, quote_for(s), RESET);
	sem_post(philo->resource->write);
}

int	philo_error(char *s)
{
	ft_putstr_fd(s, 1);
	exit (1);
}

int	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
