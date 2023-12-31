/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 16:52:03 by aputiev           #+#    #+#             */
/*   Updated: 2023/06/23 18:56:40 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Starts philosopher routine */ 
void	*philo_routine(void *philo)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)philo;
	if (philosopher->data->req_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philosopher->meal_count_lock);
	philosopher->last_dinner = philosopher->data->time_start_sim;
	pthread_mutex_unlock(&philosopher->meal_count_lock);
	delay(philosopher->data->time_start_sim);
	if (philosopher->data->num_of_philosophers == 1)
		return (one_philo_routine(philosopher));
	write_status(philo, 0, THINKING);
	if (philosopher->id % 2)
	{
		usleep((philosopher->data->time_to_eat) * 500);
	}
	while (is_simulation_ended(philosopher->data) == 0)
	{
		eat_sleep_think_routine(philosopher);
		think_time_calc(philosopher, 0);
	}
	return (NULL);
}

/* Routine for single philosopher case */
void	*one_philo_routine(t_philosopher	*philosopher)
{
	pthread_mutex_lock(&philosopher->data->forks[philosopher->fork_right]);
	write_status(philosopher, 0, FORK);
	alarm_loop(philosopher->data, philosopher->data->time_to_die);
	write_status(philosopher, 0, DIED);
	pthread_mutex_unlock(&philosopher->data->forks[philosopher->fork_right]);
	return (NULL);
}

/* Philosopher's loop cycle routine */ 
void	eat_sleep_think_routine(t_philosopher	*philosopher)
{
	if ((!pthread_mutex_lock(&philosopher->data->forks[philosopher->fork_left]))
		&& (!pthread_mutex_lock(&philosopher->data->forks[philosopher
					->fork_right])))
	{
		write_status(philosopher, 0, FORK);
		write_status(philosopher, 0, FORK);
		write_status(philosopher, 0, EATING);
		pthread_mutex_lock(&philosopher->meal_count_lock);
		philosopher->last_dinner = get_real_time();
		pthread_mutex_unlock(&philosopher->meal_count_lock);
		alarm_loop(philosopher->data, philosopher->data->time_to_eat);
		if (is_simulation_ended(philosopher->data) == 0)
		{
			pthread_mutex_lock(&philosopher->meal_count_lock);
			philosopher->eated_dinners += 1;
			pthread_mutex_unlock(&philosopher->meal_count_lock);
		}
		write_status(philosopher, 0, SLEEPING);
		pthread_mutex_unlock(&philosopher->data->forks[philosopher->fork_left]);
		pthread_mutex_unlock(&philosopher->data
			->forks[philosopher->fork_right]);
		alarm_loop(philosopher->data, philosopher->data->time_to_sleep);
	}
}

/* Checks if philosopher died from starvation */
int	is_philo_dead(t_philosopher *philosopher)
{
	long long	time;

	time = get_real_time();
	if ((time - (philosopher->last_dinner)) >= (philosopher->data->time_to_die))
	{
		pthread_mutex_lock(&philosopher->data->flag_stop_sim_lock);
		philosopher->data->flag_stop_sim = 1;
		pthread_mutex_unlock(&philosopher->data->flag_stop_sim_lock);
		write_status(philosopher, 1, DIED);
		pthread_mutex_unlock(&philosopher->meal_count_lock);
		return (1);
	}
	return (0);
}

/* Finishes simulation when it needed */
void	*monitoring_routine(void	*input_data)
{
	t_data	*data;

	data = (t_data *)input_data;
	if (data->req_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&data->flag_stop_sim_lock);
	data->flag_stop_sim = 0;
	pthread_mutex_unlock(&data->flag_stop_sim_lock);
	delay(data->time_start_sim);
	while (1)
	{
		if (should_i_stop_simulation(data) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
