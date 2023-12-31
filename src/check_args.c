/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 17:32:03 by aputiev           #+#    #+#             */
/*   Updated: 2023/06/23 20:54:25 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Checks if character is a digit */
int	ft_isdigit(int c)
{
	if ((c > 47 && c < 58) || c == 43 || c == 45)
		return (1);
	else
		return (0);
}

/* atoi*/
int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57 && str[i])
	{
		res = res * 10 + str[i] - 48;
		i++;
	}
	return (res * sign);
}

/* Checks arguments */
int	check_args(int ac, char	**av)
{
	int	i;
	int	j;

	i = 1;
	if (ac != 5 && ac != 6)
	{
		printf("Error: wrong arguments number.\n");
		return (0);
	}
	while (av[i])
	{
		j = 0;
		while (av[i][j] != '\0')
		{
			if ((!ft_isdigit(av[i][j])) || (ft_atoi(av[i]) < 0)
				|| (ft_atoi(av[i]) > INT_MAX) || (ft_atoi(av[1]) > 200
				|| (ft_atoi(av[2]) < 60)) || (ft_atoi(av[3]) < 60)
				|| (ft_atoi(av[4]) < 60))
			{
				printf("Error: invalid arguments.\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}
