/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 17:04:48 by aputiev           #+#    #+#             */
/*   Updated: 2023/06/23 17:04:53 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*writes args to data struct*/
t_data	*init_data(int ac, char	**av)
{
	t_data	*data;

	data = malloc(sizeof(t_data) * 1);
	if (!data)
		return (NULL);
	data->num_of_philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->req_eat_count = -1;
	if (ac == 6)
		data->req_eat_count = ft_atoi(av[5]);
	data->philosophers = init_philos(data);
	if (!data->philosophers)
		return (NULL);
	data->forks = create_forks(data);
	if (!data->forks)
		return (NULL);
	if (pthread_mutex_init(&data->flag_stop_sim_lock, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&data->stdout_lock, NULL) != 0)
		return (NULL);
	return (data);
}

/*Initializes philosophers */
t_philosopher	**init_philos(t_data *data)
{
	t_philosopher	**philosophers;
	int				i;

	philosophers = malloc(sizeof(t_philosopher) * (data->num_of_philosophers));
	if (!philosophers)
		return (NULL);
	i = 0;
	while (i < data->num_of_philosophers)
	{
		philosophers[i] = malloc(sizeof(t_philosopher) * 1);
		if (!philosophers[i])
			return (NULL);
		philosophers[i]->data = data;
		philosophers[i]->id = i;
		philosophers[i]->eated_dinners = 0;
		if (pthread_mutex_init(&philosophers[i]->meal_count_lock, NULL) != 0)
			return (NULL);
		i++;
	}
	return (philosophers);
}

/* Creates forks */
pthread_mutex_t	*create_forks(t_data *data)
{
	pthread_mutex_t	*forks;
	int				i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * data->num_of_philosophers);
	if (!forks)
		return (NULL);
	while (i < data->num_of_philosophers)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (NULL);
		data->philosophers[i]->fork_right = i;
		if (i == data->num_of_philosophers - 1)
			data->philosophers[i]->fork_left = 0;
		else
			data->philosophers[i]->fork_left = i + 1;
		i++;
	}
	return (forks);
}
