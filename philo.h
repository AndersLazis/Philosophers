#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>


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

typedef struct s_philosopher	t_philosopher;

typedef struct s_data
{
	int num_of_philosophers; 
	int	time_to_die; 
	int	time_to_eat; 
	int	time_to_sleep; 
	int req_number_of_eat; 
	int number_of_eated_philos; 
	t_philosopher **philosophers;
	pthread_mutex_t	*forks;
	long long time_start_sim;


}	t_data;

typedef struct s_philosopher
{
	int id;	
	t_data *data;
	int my_number_of_eat_times;
	int eated_dinners;
	pthread_mutex_t	dinner_counter_lock;
	int	fork_left;
	int fork_right;


	
}	t_philosopher;


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