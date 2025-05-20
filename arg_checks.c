/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_checks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:42:47 by yalp              #+#    #+#             */
/*   Updated: 2025/05/20 15:28:59 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 1;
	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			k = -1;
		i++;
	}
	while ((str[i] <= 57 && str[i] >= 48))
	{
		j = j * 10 + (str[i] - 48);
		i++;
	}
	return (k * j);
}

int	is_digit(char *argv)
{
	while (*argv)
	{
		if (*argv < '0' || *argv > '9')
			return (1);
		argv++;
	}
	return (0);
}

int	is_args_digit(int argc, char **argv)
{
	while (argc > 1)
	{
		if (is_digit(argv[argc - 1]))
		{
			printf("Invalid arguments\n");
			return (1);
		}
		argc--;
	}
	return (0);
}

int	is_int(char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
			j++;
		if (j > 10)
			return (1);
		i++;
	}
	i = 1;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) < 0)
			return (1);
		i++;
	}
	return (0);
}

int	arg_check(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Invalid numbers of arguments\n");
		return (1);
	}
	if (is_args_digit(argc, argv) == 1)
		return (1);
	if (is_int(argv) == 1)
	{
		printf("Invalid arguments\n");
		return (1);
	}
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) == 0)
			return (1);
	}
	return (0);
}
