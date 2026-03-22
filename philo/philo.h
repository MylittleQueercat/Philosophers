/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:12:39 by hguo              #+#    #+#             */
/*   Updated: 2025/09/25 15:56:20 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>

# define C_RESET   "\033[0m"
# define C_BOLD    "\033[1m"
# define C_DIM     "\033[2m"
# define C_GRAY    "\033[90m"
# define C_BRED    "\033[1;91m"
# define C_BGREEN  "\033[1;92m"
# define C_BYELLOW "\033[1;93m"
# define C_BBLUE   "\033[1;94m"
# define C_BMAG    "\033[1;95m"
# define C_BCYAN   "\033[1;96m"
# define BG_RED    "\033[41m"
# define BG_BLACK  "\033[40m"

typedef struct s_ctx	t_ctx;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_ctx			*ctx;
}	t_philo;

typedef struct s_ctx
{
	int				num_philos;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	int				must_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	state_mtx;
	bool			stopped;
	int				full_count;
	long			start_time;
	pthread_t		mon;
	t_philo			*philo;
}	t_ctx;

int		parse_args(t_ctx *ctx, int argc, char **argv);
int		alloc_ctx(t_ctx *ctx);
int		init_mutexes(t_ctx *ctx);
int		init_philos(t_ctx *ctx);
void	cleanup_ctx(t_ctx *ctx);
long	now_ms(void);
void	ms_sleep(long ms, t_ctx *ctx);
int		safe_get_stopped(t_ctx *ctx);
void	set_stopped(t_ctx *ctx);
long	get_last_meal(t_philo *p);
char	*quote_for(char *msg);
char	*color_for(char *msg);
void	print_action(t_ctx *ctx, int id, char *s);
void	print_died(t_ctx *ctx, int id);
void	*philo_routine(void *arg);
int		pick_forks(t_philo *p);
void	drop_forks(t_philo *p);
int		eat_block(t_philo *p);
int		one_philo_flow(t_philo *p);
void	*monitor_routine(void *arg);
int		start_threads(t_ctx *ctx);
int		join_threads(t_ctx *ctx);

#endif
