/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:19:11 by yalp              #+#    #+#             */
/*   Updated: 2025/05/17 14:56:33 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	is_all_philos_full(t_loop *loop)
{
	int	i;

	i = 0;
	while (i < loop->number_of_philos)
	{
		if (loop->philos[i].number_of_times_eaten < loop->number_of_times_each_philosopher_must_eat)
			return (0);
		i++;
	}
	return (1);
}

void  even_id_philo(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	printing(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	printing(philo, "has taken a fork");
	printing(philo, "is eating");
	philo->last_meal_time = get_time();
	usleep(philo->loop_con->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	philo->number_of_times_eaten++;
	printing(philo, "is sleeping");
	usleep(philo->loop_con->time_to_sleep * 1000);
	printing(philo, "is thinking");
}


void  odd_id_philo(t_philosopher *philo)
{
	pthread_mutex_lock(philo->right_fork);
	printing(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	printing(philo, "has taken a fork");
	printing(philo, "is eating");
	philo->last_meal_time = get_time();
	usleep(philo->loop_con->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	philo->number_of_times_eaten++;
	printing(philo, "is sleeping");
	usleep(philo->loop_con->time_to_sleep * 1000);
	printing(philo, "is thinking");
}
