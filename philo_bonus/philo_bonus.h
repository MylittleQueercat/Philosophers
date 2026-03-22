/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:13:31 by hguo              #+#    #+#             */
/*   Updated: 2025/09/28 15:33:18 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>
# include <fcntl.h>
# include <semaphore.h>
# include <limits.h>
# include <errno.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdbool.h>

# define RESET    "\033[0m"
# define ROSE      "\033[38;5;218m"
# define BLEU      "\033[38;5;111m"
# define BLUE      "\033[38;5;54m"
# define PINK      "\033[38;5;224m"
# define GREEN     "\033[38;5;115m"
# define VIOLET    "\033[38;5;146m"
# define C_GRAY    "\033[90m"
# define C_BOLD    "\033[1m"
# define BG_RED    "\033[41m"

# define ERR_ARG_NONPOS "Error: All values must be positive integers [≥1].\n"
# define ERR_ARG_DIGIT "Error: Argument must be a number.\n"
# define ERR_ARG_COUNT "Error: Expected 5 or 6 arguments.\n"
# define ERR_ALLOC "Error: Memory allocation failed.\n"
# define ERR_MUTEX_INIT "Error: Failed to init mutex.\n"
# define ERR_MUTEX_DEST "Error: Failed to destroy mutex.\n"
# define ERR_CREATE "Error: Failed to create thread.\n"
# define ERR_JOIN "Error: Failed to join thread.\n"
# define ERR_TIME "Error: Failed to get time.\n"
# define ERR_SEM "Error: Failed to open semaphore.\n"
# define ERR_FORK "Error: Failed to fork process.\n"

# define EAT "is eating 🍝\n"
# define SLEEP "is sleeping 😴\n"
# define THINK "is thinking 💭\n"
# define DIE "died ☠️\n"
# define FORK "has taken a fork 🍴\n"

struct	s_resource;

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	long				last_meal_time;
	pid_t				pid;
	pthread_t			checker;
	struct s_resource	*resource;
}			t_philo;

typedef struct s_resource
{
	int				num_philos;
	long			t_eat;
	long			t_sleep;
	long			t_die;
	int				must_eat;
	long			start_time;
	int				full_count;
	int				print_death;
	t_philo			*philo;
	sem_t			*forks;
	sem_t			*write;
	sem_t			*eat;
	sem_t			*cmp_finish;
	sem_t			*stopped;
}			t_resource;

void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
int		ft_isdigit(int c);
long	atoiv(const char *str);
void	print_status(t_philo *philo, char *str);
int		philo_error(char *str);
int		get_time(void);
void	validate_args(char **argv);
void	philo_setup(int argc, char **argv, t_resource *resource);
void	creat_proces(t_resource *resource);
int		check_finish(t_philo *philo, int status);
void	*philo_monitor(void *philos);
void	routine(t_philo *philo);
void	child_cleanup(t_resource *r);
void	free_resource(t_resource *resource);
int		check_death(t_philo *philo);
int		get_stopped_locked(t_resource *r);
int		is_stopped(t_resource *r);
void	set_stopped(t_resource *r);
void	terminate_all(t_resource *r, int sig);

#endif
