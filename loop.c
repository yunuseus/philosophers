/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:05:10 by yalp              #+#    #+#             */
/*   Updated: 2025/05/19 16:41:15 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	forks(t_philosopher *philo)
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

void	eating_time(t_philosopher *philosopher)
{
	if (check_stop(philosopher->loop_con) == 1)
		return ;
	forks(philosopher);
	printing(philosopher, "is eating");
	pthread_mutex_lock(&philosopher->loop_con->death_mutex);
	philosopher->last_meal_time = get_time();
	pthread_mutex_unlock(&philosopher->loop_con->death_mutex);
	pthread_mutex_lock(&philosopher->loop_con->eat_mutex);
	philosopher->number_of_times_eaten++;
	pthread_mutex_unlock(&philosopher->loop_con->eat_mutex);
	usleep(philosopher->loop_con->time_to_eat * 1000);
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}

void	*start_loop(void	*philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	if (philo->loop_con->number_of_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		printing(philo, "has taken a fork");
		pthread_mutex_unlock(philo->left_fork);
		usleep(philo->loop_con->time_to_die * 1000);
		printing(philo, "died");
		return (NULL);
	}
	else
	{
		while (check_stop(philo->loop_con) == 0)
		{
			eating_time(philo);
			if (check_stop(philo->loop_con) == 1)
				return (NULL);
			printing(philo, "is sleeping");
			usleep(philo->loop_con->time_to_sleep * 1000);
			printing(philo, "is thinking");
		}
	}
	return (NULL);
}

int	init_loop(t_loop *loop)
{
	int	i;

	i = 0;
	if (!loop->forks)
	{
		free(loop->philos);
		return (1);
	}
	if (create_mutexes(loop))
	{
		printf("Mutex can not be created!\n");
		return (1);
	}
	if (create_threads(loop))
	{
		printf("Thread can not be created!\n");
		return (1);
	}
	return (0);
}
