/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 17:01:43 by aputiev           #+#    #+#             */
/*   Updated: 2023/06/23 17:01:45 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Prints line with timestamp and status */
static void	print_status(t_philosopher *philosopher, char *str)
{
	printf("%lld %d %s\n", get_real_time() - philosopher->data->time_start_sim,
		philosopher->id + 1, str);
}

/* General message printing function.*/
/* If flag == 1 message will be printed even if simulation stops */
void	write_status(t_philosopher *philosopher,
	int supervisor_flag, int status)
{
	pthread_mutex_lock(&philosopher->data->stdout_lock);
	if (is_simulation_ended(philosopher->data) == 1 && supervisor_flag == 0)
	{
		pthread_mutex_unlock(&philosopher->data->stdout_lock);
		return ;
	}
	if (status == DIED)
		print_status(philosopher, "died");
	else if (status == EATING)
		print_status(philosopher, "is eating");
	else if (status == SLEEPING)
		print_status(philosopher, "is sleeping");
	else if (status == THINKING)
		print_status(philosopher, "is thinking");
	else if (status == FORK)
		print_status(philosopher, "has taken a fork");
	pthread_mutex_unlock(&philosopher->data->stdout_lock);
}

/* Runs simulation */
int	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	data->time_start_sim = get_real_time() + (data->num_of_philosophers * 2);
	while (i < data->num_of_philosophers)
	{
		if (pthread_create(&data->philosophers[i]->philo_thread,
				NULL, &philo_routine, data->philosophers[i]) != 0)
			return (0);
		i++;
	}
	if (data->num_of_philosophers > 1)
	{
		if (pthread_create(&data->monitoring_thread, NULL,
				&monitoring_routine, data) != 0)
			return (0);
	}
	return (1);
}

/* Main Function */
int	main(int ac, char	**av)
{
	t_data	*data;

	if (!check_args(ac, av))
		return (1);
	data = init_data(ac, av);
	if (!data)
		return (1);
	if (!start_simulation(data))
		return (1);
	finish_simulation(data);
	free_data(data);
	return (0);
}
