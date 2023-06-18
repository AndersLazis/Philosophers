#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philosopher	t_philosopher;

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


typedef struct s_data
{
	int num_of_philosophers; 
	int	time_to_die; 
	int	time_to_eat; 
	int	time_to_sleep; 
	int req_number_of_eat; 
	int number_of_eated_philos;  //--
	t_philosopher **philosophers;
	pthread_mutex_t	*forks;
	long long time_start_sim;
	pthread_t monitoring_thread;
	int flag_stop_sim;
	pthread_mutex_t	stdout_lock;
	pthread_mutex_t	flag_stop_sim_lock;
	pthread_mutex_t	dinner_counter_lock;
	pthread_mutex_t	dead_check_lock;
	int one_dead;
	pthread_t dead_check_thread;

}	t_data;

typedef struct s_philosopher
{
	int id;	
	t_data *data;
	int my_number_of_eat_times;
	int eated_dinners; //--	
	int	fork_left;
	int fork_right;
	pthread_t	philo_thread;
	long long last_dinner;
	int	dead;

	
}	t_philosopher;

int	stop_simulation(t_data *data);
// void	destroy_mutexes(t_data *data)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (i < data->num_of_philosophers)
// 	{
// 		pthread_mutex_destroy(data->philosopher[i].left_fork_mutex);
// 		i++;
// 	}
// 	pthread_mutex_destroy(&data->data_mutex);
// }

 #endif