/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:05:10 by yalp              #+#    #+#             */
/*   Updated: 2025/05/17 15:46:16 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		printing(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		printing(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		printing(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		printing(philo, "has taken a fork");
	}
}

void eating_time(t_philosopher *philosopher)
{
	forks(philosopher);
	printing(philosopher, "is eating");
	pthread_mutex_lock(&philosopher->loop_con->eat_mutex);
	philosopher->last_meal_time = get_time();
	philosopher->number_of_times_eaten++;
	pthread_mutex_unlock(&philosopher->loop_con->eat_mutex);
	usleep(philosopher->loop_con->time_to_eat * 1000);
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}
