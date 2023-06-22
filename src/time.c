#include "../includes/philo.h"

void	delay(long long start_time)
{
	while (get_real_time() < start_time)
		continue ;
}

void	alarm_clock_loop(t_data *data, int sleep_time)
{
	long long	wake_up;

	wake_up = get_real_time() + sleep_time;
	while (get_real_time() < wake_up)
	{
		if (simulation_ended(data))
			break ;
		usleep(100);
	}
}

long long get_real_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	think_time_calc(t_philosopher *philo, int no_print_wait)
{
	long long	time_to_think;
	// no_print_wait = (int)no_print_wait;
	pthread_mutex_lock(&philo->meal_count_lock);
	// time_to_think = (philo->data->time_to_die - (get_real_time() - philo->last_dinner) - philo->data->time_to_eat) / 2;
	time_to_think = (philo->data->time_to_die - philo->data->time_to_eat - philo->data->time_to_sleep) / 2; // - philo->last_dinner) - philo->data->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_count_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && no_print_wait == 1)
		time_to_think = 1;
	if (time_to_think > 500)
		time_to_think = 200;
	if (no_print_wait == 0)
		write_status(philo, 0, THINKING);
	alarm_clock_loop(philo->data, time_to_think);
}
