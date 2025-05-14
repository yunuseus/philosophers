/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:19:11 by yalp              #+#    #+#             */
/*   Updated: 2025/05/14 18:08:53 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long		get_time(void)
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
	printf("%ld Philosopher %d has taken the left fork\n",get_time() - philo->loop_con->start_time, philo->id);
	pthread_mutex_lock(philo->right_fork);
	printf("%ld Philosopher %d has taken the right fork\n",get_time() - philo->loop_con->start_time, philo->id);
	printf("%ld Philosopher %d is eating\n",get_time() - philo->loop_con->start_time, philo->id);
		philo->number_of_times_eaten++;
}


void  odd_id_philo(t_philosopher *philo)
{
	pthread_mutex_lock(philo->right_fork);
	printf("%ld Philosopher %d has taken the right fork\n",get_time() - philo->loop_con->start_time, philo->id);
	pthread_mutex_lock(philo->left_fork);
	printf("%ld Philosopher %d has taken the left fork\n",get_time() - philo->loop_con->start_time, philo->id);
	printf("%ld Philosopher %d is eating\n",get_time() - philo->loop_con->start_time, philo->id);
		philo->number_of_times_eaten++;
}
