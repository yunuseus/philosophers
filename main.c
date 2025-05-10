/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:42:14 by marvin            #+#    #+#             */
/*   Updated: 2025/05/10 20:42:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	arg_checker(int argc, char **argv)
{
	while (argc > 1)
	{
		if (is_digit(argv[argc - 1]))
		{
			write(2, "Invalid arguments\n", 19);
			exit(1);
		}
		argc--;
	}
}


int	main(int argc, char **argv)
{
	arg_checker(argc, argv);
}