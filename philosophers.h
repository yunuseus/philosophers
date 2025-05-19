/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:49:20 by yalp              #+#    #+#             */
/*   Updated: 2025/05/19 15:34:49 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H

# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct philosopher_s
{
	int					id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	unsigned long long	last_meal_time;
	int					number_of_times_eaten;
	pthread_t			thread;
	struct loop_s		*loop_con;
	int					is_eating;
}	t_philosopher;

typedef struct loop_s
{
	t_philosopher		*philos;
	int					number_of_philos;
	unsigned long long	start_time;
	unsigned long long	time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					number_of_times_each_philosopher_must_eat;
	int					stop;
	int					eat;
	pthread_t			control_thread;
	pthread_mutex_t		*forks;
	pthread_mutex_t		check_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		eat_mutex;
}	t_loop;

void				killer(t_philosopher *philosoper);
void				init_values(t_loop *loop, int i);
int					create_threads(t_loop *loop);
int					create_mutexes(t_loop *loop);
void				*start_loop(void	*philosopher);
int					init_loop(t_loop *loop);
void				*start_loop(void	*philosopher);
void				*is_must_stop(void *arg);
int					ft_atoi(const char *str);
int					arg_check(int argc, char **argv);
int					is_all_philos_full(t_loop *loop);
unsigned long long	get_time(void);
void				printing(t_philosopher *philo, char *str);
void				eating_time(t_philosopher *philosopher);
int					check_stop(t_loop *loop);
#endif