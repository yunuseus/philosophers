/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:19:11 by yalp              #+#    #+#             */
/*   Updated: 2025/05/19 15:21:44 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	killer(t_philosopher *philosoper)
{
	pthread_mutex_lock(&philosoper->loop_con->death_mutex);
	philosoper->loop_con->stop = 1;
	pthread_mutex_lock(&philosoper->loop_con->print_mutex);
	printf("%llu %d %s\n", get_time() - philosoper->loop_con->start_time,
		philosoper->id, "died");
	pthread_mutex_unlock(&philosoper->loop_con->print_mutex);
	pthread_mutex_unlock(&philosoper->loop_con->death_mutex);
}

void	printing(t_philosopher *philo, char *str)
{
	if (check_stop(philo->loop_con))
		return ;
	pthread_mutex_lock(&philo->loop_con->print_mutex);
	printf("%llu %d %s\n", get_time() - philo->loop_con->start_time,
		philo->id, str);
	pthread_mutex_unlock(&philo->loop_con->print_mutex);
}

int	create_threads(t_loop *loop)
{
	int	i;

	i = 0;
	while (i < loop->number_of_philos)
	{
		init_values(loop, i);
		if (pthread_create(&loop->philos[i].thread, NULL,
				start_loop, &loop->philos[i]) != 0)
		{
			printf("Thread %d can not started!\n", loop->philos[i].id);
			return (1);
		}
		i++;
	}
	usleep(300);
	if (loop->number_of_philos != 1)
	{
		if (pthread_create(&loop->control_thread, NULL,
				is_must_stop, loop) != 0)
		{
			printf("Thread %d can not started!\n", loop->philos[i].id);
			return (1);
		}
	}
	return (0);
}

int	create_mutexes(t_loop *loop)
{
	int	i;

	i = 0;
	while (i < loop->number_of_philos)
	{
		if (pthread_mutex_init(&loop->forks[i++], NULL) != 0)
			return (1);
	}
	if (pthread_mutex_init(&loop->check_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&loop->death_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&loop->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&loop->eat_mutex, NULL) != 0)
		return (1);
	return (0);
}
