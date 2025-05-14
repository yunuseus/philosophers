/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:42:14 by marvin            #+#    #+#             */
/*   Updated: 2025/05/14 19:15:11 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


void	*start_loop(void	*philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	while (1)
	{
		pthread_mutex_lock(&philo->loop_con->death_mutex);
		if (philo->loop_con->is_someone_dead == 1)
		{
			pthread_mutex_unlock(&philo->loop_con->death_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->loop_con->death_mutex);
		if (philo->id % 2 == 0)
			even_id_philo(philo);
		else
			odd_id_philo(philo);
		usleep(philo->loop_con->time_to_eat * 1000);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		printf("%llu Philosopher %d is sleeping\n",get_time() - philo->loop_con->start_time, philo->id);
		usleep(philo->loop_con->time_to_sleep * 1000);
		printf("%llu Philosopher %d is thinking\n",get_time() - philo->loop_con->start_time, philo->id);
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
			if (get_time() - loop->philos[i].last_meal_time > loop->time_to_die)
			{
				printf("%llu Philosopher %d is dead\n", get_time() - loop->start_time, loop->philos[i].id);
				pthread_mutex_lock(&loop->death_mutex);
				loop->is_someone_dead = 1;
				pthread_mutex_unlock(&loop->death_mutex);
				return (NULL);
			}
			i++;
		}
		
		usleep(100);
	}
	return (NULL);
}

void	init_philosophers(t_loop *loop)
{
    int	i;

    i = 0;
    loop->philos = malloc(sizeof(t_philosopher) * loop->number_of_philos);
    while (i < loop->number_of_philos)
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
        pthread_create(&loop->philos[i].thread, NULL,
            start_loop, &loop->philos[i]);
        i++;
    }
	pthread_create(&loop->control_thread, NULL, loop_ctrl, loop);
}

void	init_mutexes(t_loop *loop)
{
	int	i;

	i = 0;
	loop->forks = malloc(sizeof(pthread_mutex_t) * loop->number_of_philos);
	while (i < loop->number_of_philos)
		pthread_mutex_init(&loop->forks[i++], NULL);
	pthread_mutex_init(&loop->death_mutex, NULL);
	pthread_mutex_init(&loop->print_mutex, NULL);
	pthread_mutex_init(&loop->eat_mutex, NULL);
}

void	init_loop(t_loop *loop, int argc, char **argv)
{
	int	i;

	i = 0;
	loop->number_of_philos = ft_atoi(argv[1]);
	loop->time_to_die = ft_atoi(argv[2]);
	loop->time_to_eat = ft_atoi(argv[3]);
	loop->time_to_sleep = ft_atoi(argv[4]);
	loop->start_time = get_time();
	loop->is_someone_dead = 0;
	if (argc == 6)
		loop->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		loop->number_of_times_each_philosopher_must_eat = -1;
	init_mutexes(loop);
	init_philosophers(loop);
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

int	main(int argc, char **argv)
{
	t_loop	loop;
	int		i;

	if (arg_check(argc, argv) == 1)
		return (1);
	init_loop(&loop, argc, argv);
	i = 0;
	pthread_join(loop.control_thread, NULL);
	while (i < loop.number_of_philos)
	{
		pthread_join(loop.philos[i].thread, NULL);
		i++;
	}
	return (0);
}
