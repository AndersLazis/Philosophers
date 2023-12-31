/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 16:42:01 by aputiev           #+#    #+#             */
/*   Updated: 2023/06/23 16:45:16 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Delay before simulation starts */
void	delay(long long start_time)
{
	while (get_real_time() < start_time)
		continue ;
}

/* Sleep function loop. If simulation flag_stop_sim_lock == 1,*/
/* then loop stops in any time. */
void	alarm_loop(t_data *data, int sleep_time)
{
	long long	wake_up;

	wake_up = get_real_time() + sleep_time;
	while (get_real_time() < wake_up)
	{
		if (is_simulation_ended(data))
			break ;
		usleep(100);
	}
}

/* Gets real time in milliseconds */
long long	get_real_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

/* Calculates time to think. If flag == 1,*/
/* then message "X is thinking" will not be printed */
void	think_time_calc(t_philosopher *philo, int no_print_flag)
{
	long long	time_to_think;

	pthread_mutex_lock(&philo->meal_count_lock);
	time_to_think = (philo->data->time_to_die - (get_real_time()
				- philo->last_dinner) - philo->data->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_count_lock);
	if (no_print_flag == 0)
		write_status(philo, 0, THINKING);
	alarm_loop(philo->data, time_to_think);
}
