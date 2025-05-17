/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:42:14 by marvin            #+#    #+#             */
/*   Updated: 2025/05/17 17:48:36 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void killer(t_philosopher *phiolosoper)
{
	pthread_mutex_lock(&phiolosoper->loop_con->death_mutex);
	phiolosoper->loop_con->is_must_stop = 1;
	printing(phiolosoper, "is died");
	pthread_mutex_unlock(&phiolosoper->loop_con->death_mutex);
}

void printing(t_philosopher *philo, char *str)
{
	pthread_mutex_lock(&philo->loop_con->print_mutex);
	if (philo->loop_con->is_someone_dead == 0)
		printf("%llu %d %s\n", get_time() - philo->loop_con->start_time, philo->id, str);
	pthread_mutex_unlock(&philo->loop_con->print_mutex);
}

int check_meals(t_loop *loop)
{
	int i;
	int c;

	i = 0;
	c = 0;
	pthread_mutex_lock(&loop->eat_mutex);
	while (i < loop->number_of_philos)
	{
		if (loop->philos[i++].number_of_times_eaten >= loop->number_of_times_each_philosopher_must_eat)
			c++;
	}
	pthread_mutex_unlock(&loop->eat_mutex);
	return (c == loop->number_of_philos);
}

void	*is_must_stop(void *arg)
{
	int	i;

	t_loop *loop;
	loop = (t_loop *)arg;
	i = 0;


	while (i < loop->number_of_philos)
	{
		pthread_mutex_lock(&loop->eat_mutex);
		if (get_time() - loop->philos[i].last_meal_time > loop->time_to_die)
		{
			killer(&loop->philos[i]);
			pthread_mutex_unlock(&loop->eat_mutex);
			return NULL;
		}
		pthread_mutex_unlock(&loop->eat_mutex);

	}

	return NULL;
}

int check_stop(t_loop *loop)
{
	int	ret;

	pthread_mutex_lock(&loop->check_mutex);
	ret = loop->is_must_stop;
	pthread_mutex_unlock(&loop->check_mutex);
	return (ret);
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
			printing(philo, "is sleeping");
			usleep(philo->loop_con->time_to_sleep * 1000);
			printing(philo, "is thinking");
		}
	}
	return (NULL);
}

void	*loop_ctrl(void *tmp)
{
	int	i;
	t_loop	*loop;

	loop = (t_loop *)tmp;

	while (1)
	{
		i = 0;
		while (i < loop->number_of_philos)
		{
			pthread_mutex_lock(&loop->death_mutex);
			if (get_time() - loop->philos[i].last_meal_time > loop->time_to_die)
			{
				printing(&loop->philos[i], "died");
				loop->is_someone_dead = 1;
				pthread_mutex_unlock(&loop->death_mutex);
				
			}
			else
				pthread_mutex_unlock(&loop->death_mutex);
			i++;
		}
	}
}

void init_values(t_loop *loop, int i)
{
	loop->philos[i].loop_con = loop;
	loop->philos[i].id = i + 1;
	loop->philos[i].left_fork = &loop->forks[i];
	loop->philos[i].number_of_times_eaten = 0;
	loop->philos[i].last_meal_time = loop->start_time;
	if (i == loop->number_of_philos - 1)
		loop->philos[i].right_fork = &loop->forks[0];
	else
		loop->philos[i].right_fork = &loop->forks[i + 1];
}

int	create_threads(t_loop *loop)
{
    int	i;

    i = 0;
    while (i < loop->number_of_philos)
    {
        init_values(loop, i);
        if (pthread_create(&loop->philos[i].thread, NULL, start_loop, &loop->philos[i]) != 0)
		{
			printf("Thread %d başlatılamadı!\n", loop->philos[i].id);
			return (1);
		}
		i++;
    }
	usleep(100);
	if (pthread_create(&loop->control_thread, NULL, is_must_stop, loop) != 0)
	{
    	printf("Thread %d başlatılamadı!\n", loop->philos[i].id);
		return (1);
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

int	init_loop(t_loop *loop, int argc, char **argv)
{
	int	i;

	i = 0;
	if (!loop->forks)
	{
		free(loop->philos);
		return (1);
	}
	if (create_mutexes(loop))
		return (1);
	if (create_threads(loop))
		return (1);
	return (0);
}

void	end(t_loop *loop)
{
	int	i;

	i = 0;
	while (i < loop->number_of_philos)
	{
		pthread_mutex_destroy(&loop->forks[i]);
		i++;
	}
	free(loop->forks);
	free(loop->philos);
}
int init_args(t_loop *loop, char **argv, int argc)
{
	loop->number_of_philos = ft_atoi(argv[1]);
	loop->time_to_die = ft_atoi(argv[2]);
	loop->time_to_eat = ft_atoi(argv[3]);
	loop->time_to_sleep = ft_atoi(argv[4]);
	loop->start_time = get_time();
	loop->is_someone_dead = 0;
	loop->is_must_stop = 0;
	if (argc == 6)
		loop->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		loop->number_of_times_each_philosopher_must_eat = -1;
	loop->philos = malloc(sizeof(t_philosopher) * loop->number_of_philos);
		if (!loop->philos)
			return (1);
	loop->forks = malloc(sizeof(pthread_mutex_t) * loop->number_of_philos);
	if (!loop->forks)
	{
		free(loop->philos);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_loop	loop;
	int		i;

	if (arg_check(argc, argv) == 1)
		return (1);
	if (init_args(&loop, argv, argc))
		return (1);
	if (init_loop(&loop, argc, argv))
		return (1);
	i = 0;
	while (i < loop.number_of_philos)
	{
		pthread_join(loop.philos[i].thread, NULL);
		i++;
	}
	return (0);
}
