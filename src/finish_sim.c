#include "../includes/philo.h"


void	destroy_mutexes(t_data	*data)
	{	
		int	i;

		i = 0;
		while(i < data->num_of_philosophers)
		{
			pthread_mutex_destroy(&data->philosophers[i]->meal_count_lock);
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		pthread_mutex_destroy(&data->stdout_lock);
		pthread_mutex_destroy(&data->flag_stop_sim_lock);
	
	}


int	finish_simulation(t_data	*data)
{
	int i;

	i = 0;
	while(i < data->num_of_philosophers)
	{
		pthread_join(data->philosophers[i]->philo_thread, NULL);
		i++;
	}
	if(data->num_of_philosophers > 1)
		pthread_join(data->monitoring_thread, NULL);
	destroy_mutexes(data);
	return (0);
}


int	simulation_ended(t_data *data)
{
	int	flag_stop;

	flag_stop = 0;
	pthread_mutex_lock(&data->flag_stop_sim_lock);
	if (data->flag_stop_sim == 1)
		flag_stop = 1;
	pthread_mutex_unlock(&data->flag_stop_sim_lock);
	return (flag_stop);
}




int	stop_simulation(t_data *data)
{
	int i;
	int arg;

	arg = 1;
	i = 0;
	while (i < data->num_of_philosophers)
	{	
		pthread_mutex_lock(&data->philosophers[i]->meal_count_lock);
		if(kill_philo(data->philosophers[i]))
			return (1);
		if(data->req_eat_count != -1)
			if((data->philosophers[i]->eated_dinners < data->req_eat_count))
				arg = 0;
		pthread_mutex_unlock(&data->philosophers[i]->meal_count_lock);
		i++;
	}

	if(data->req_eat_count != -1 && arg == 1)
	{
		pthread_mutex_lock(&data->flag_stop_sim_lock);
		data->flag_stop_sim = 1;
		pthread_mutex_unlock(&data->flag_stop_sim_lock);
		return(1);
	}
	return(0);
}

int free_data(t_data *data)
{	
	int	i;
	
	i = 0;
	while(i < data->num_of_philosophers)
	{
		if(data->philosophers[i] != NULL)
			free(data->philosophers[i]);
		i++;
	}
	if(data->philosophers != NULL)
		free(data->philosophers);
	if(data->forks != NULL)
		free(data->forks);
	free(data);
	return(0);
}
