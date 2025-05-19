/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:11:56 by yalp              #+#    #+#             */
/*   Updated: 2025/05/19 15:24:42 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_all_philos_full(t_loop *loop)
{
	int	i;

	i = 0;
	while (i < loop->number_of_philos)
	{
		pthread_mutex_lock(&loop->eat_mutex);
		if (loop->philos[i].number_of_times_eaten
			< loop->number_of_times_each_philosopher_must_eat)
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

int	check_stop(t_loop *loop)
{
	int	death;
	int	eat;

	pthread_mutex_lock(&loop->eat_mutex);
	eat = loop->eat;
	pthread_mutex_unlock(&loop->eat_mutex);
	pthread_mutex_lock(&loop->death_mutex);
	death = loop->stop;
	pthread_mutex_unlock(&loop->death_mutex);
	return (death || eat);
}

int	check_meals(t_loop *loop)
{
	int	i;
	int	c;

	i = 0;
	c = 0;
	pthread_mutex_lock(&loop->eat_mutex);
	while (i < loop->number_of_philos)
	{
		if (loop->philos[i++].number_of_times_eaten
			>= loop->number_of_times_each_philosopher_must_eat)
			c++;
	}
	pthread_mutex_unlock(&loop->eat_mutex);
	return (c == loop->number_of_philos);
}

void	*is_must_stop(void *arg)
{
	int		i;
	t_loop	*loop;

	loop = (t_loop *)arg;
	i = 0;
	while (i < loop->number_of_philos)
	{
		if (is_all_philos_full(loop))
			return (NULL);
		pthread_mutex_lock(&loop->death_mutex);
		if (get_time() - loop->philos[i].last_meal_time > loop->time_to_die)
		{
			killer(&loop->philos[i]);
			pthread_mutex_unlock(&loop->death_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&loop->death_mutex);
	}
	return (NULL);
}
