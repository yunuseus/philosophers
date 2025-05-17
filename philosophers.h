/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:49:20 by yalp              #+#    #+#             */
/*   Updated: 2025/05/17 16:41:52 by yalp             ###   ########.fr       */
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
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					number_of_times_each_philosopher_must_eat;
	int 				is_someone_dead;
	int					is_must_stop;
	pthread_t			control_thread;
	pthread_mutex_t		*forks;
	pthread_mutex_t		check_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		eat_mutex;
}	t_loop;

int					ft_atoi(const char *str);
int					arg_check(int argc, char **argv);
int					is_all_philos_full(t_loop *loop);
void  				even_id_philo(t_philosopher *philo);
void 				odd_id_philo(t_philosopher *philo);
unsigned long long	get_time(void);
void				printing(t_philosopher *philo, char *str);
void 				eating_time(t_philosopher *philosopher);
#endif