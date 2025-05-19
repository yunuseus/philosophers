/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:19:11 by yalp              #+#    #+#             */
/*   Updated: 2025/05/19 15:04:51 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int is_all_philos_full(t_loop *loop)
{
	int	i;

	i = 0;
	while (i < loop->number_of_philos)
	{
		pthread_mutex_lock(&loop->eat_mutex);
		if (loop->philos[i].number_of_times_eaten < loop->number_of_times_each_philosopher_must_eat)
		{
			pthread_mutex_unlock(&loop->eat_mutex);
			return (0);
		}
		pthread_mutex_unlock(&loop->eat_mutex);
		i++;
	}
	pthread_mutex_lock(&loop->eat_mutex);
	loop->eat = 1;
	pthread_mutex_unlock(&loop->eat_mutex);
	return (1);
}
