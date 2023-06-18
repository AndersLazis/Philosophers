#include "philo.h"


t_philosopher	**init_philos(t_data *data)
{	
	t_philosopher **philosophers;
	int i;

	philosophers = malloc(sizeof(t_philosopher)*(data->num_of_philosophers));
	if(!philosophers)
		return(NULL);
	i = 0;
	while (i < data->num_of_philosophers)
	{
		philosophers[i] = malloc(sizeof(t_philosopher) * 1);
		if(!philosophers[i])
			return(NULL);
		philosophers[i]->data = data;
		philosophers[i]->id = i;
		philosophers[i]->eated_dinners = 0;
		philosophers[i]->my_number_of_eat_times = data->req_number_of_eat;
		printf(" i am %d philosophers[i]->my_number_of_eat_times %d\n", philosophers[i]->id, philosophers[i]->my_number_of_eat_times);
		philosophers[i]->dead = 0;
		if(pthread_mutex_init(&philosophers[i]->data->dinner_counter_lock, NULL) != 0)
			return(NULL);
		i++;
	}
	return(philosophers);
}


pthread_mutex_t	*create_forks(t_data *data)
{
	pthread_mutex_t *forks;
	int i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * data->num_of_philosophers);
	if(!forks)
		return(NULL);
	while (i < data->num_of_philosophers)
	{
		if(pthread_mutex_init(&forks[i], 0) != 0)
			return (NULL);
		data->philosophers[i]->fork_right = i;
		
		if(i  == data->num_of_philosophers - 1)
			data->philosophers[i]->fork_left = 0;
		else
			data->philosophers[i]->fork_left = i + 1;
		//printf("i am %d my left is %d, right is %d\n", data->philosophers[i]->id, data->philosophers[i]->fork_left, data->philosophers[i]->fork_right);
		i++;
	}
	return(forks);
}



/*writes args to data struct*/
t_data	*init_data(int ac, char	**av)
{	
	t_data	*data;

	data = malloc(sizeof(t_data) * 1);
	if(!data)
		return(NULL);
	data->num_of_philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->flag_stop_sim = 0;
	if (ac == 6)
	{
		data->req_number_of_eat = ft_atoi(av[5]);
		printf("data->req_number_of_eat %d\n", data->req_number_of_eat);
	}
	else
		data->req_number_of_eat = INT_MAX;

	data->number_of_eated_philos = data->num_of_philosophers;
	printf("data->number_of_eated_philos %d\n", data->number_of_eated_philos);
	data->philosophers = init_philos(data);
	if(!data->philosophers)
		return(NULL);
	data->forks = create_forks(data);
	if(!data->forks)
		return(NULL);
	if(pthread_mutex_init(&data->flag_stop_sim_lock, NULL) != 0)
		return(NULL);
	return(data);
}

int free_data(t_data *data)
{
	int i;

	i = 0;
	while (i < data->num_of_philosophers)
	{
		free(data->philosophers[i]);
		i++;
	}
	free(data->forks);
	free(data);
	return(0);
}


long long get_real_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

int is_sim_stop(t_data	*data)
{	
	pthread_mutex_lock(&data->flag_stop_sim_lock);
	if(data->flag_stop_sim == 1)
	{	pthread_mutex_unlock(&data->flag_stop_sim_lock);
		return(1);
	}
	pthread_mutex_unlock(&data->flag_stop_sim_lock);
	return(0);
}

int am_i_dead (t_philosopher	*philosopher)
{
	if ((get_real_time() - (philosopher->last_dinner)) > (philosopher->data->time_to_die))
	{
		printf("%lld %d died\n", (get_real_time() - philosopher->data->time_start_sim), philosopher->id);
		pthread_mutex_lock(&philosopher->data->flag_stop_sim_lock);
		philosopher->data->flag_stop_sim = 1;
		pthread_mutex_unlock(&philosopher->data->flag_stop_sim_lock);
		return (1);
	}
	return (0);	
}


void	*eat_sleep_think_routine(t_philosopher	*philosopher)
{	
	// if(am_i_dead(philosopher) == 1 || (philosopher->data->flag_stop_sim == 1))
	// 	return (NULL);
	


	if (((pthread_mutex_lock(&philosopher->data->forks[philosopher->fork_left])) == 0) &&  (pthread_mutex_lock(&philosopher->data->forks[philosopher->fork_right])) == 0)
	{	
		printf("%lld %d has taken a fork\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
		printf("%lld %d has taken a fork\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
		philosopher->last_dinner = get_real_time();
		printf("%lld %d is eating\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
		philosopher->my_number_of_eat_times--;
		usleep((philosopher->data->time_to_eat)*1000);
		pthread_mutex_unlock(&philosopher->data->forks[philosopher->fork_right]);
		pthread_mutex_unlock(&philosopher->data->forks[philosopher->fork_left]);

		printf("%lld %d is sleeping\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
		usleep((philosopher->data->time_to_sleep)*1000);
	}

	if(philosopher->my_number_of_eat_times == 0)
	{
		pthread_mutex_lock(&philosopher->data->dinner_counter_lock);
		philosopher->data->number_of_eated_philos--;
		pthread_mutex_unlock(&philosopher->data->dinner_counter_lock);
		return(NULL);
	}

	printf("%lld %d is thinking\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
	return (NULL);
}







void	*philo_routine(void *philo)
{
	t_philosopher *philosopher;
	philosopher = (t_philosopher *)philo;
	philosopher->last_dinner = philosopher->data->time_start_sim;
	printf("%lld %d is thinking\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
	if(philosopher->id % 2 != 0)
		usleep((philosopher->data->time_to_eat/2)*1000);
	else
		usleep(10000);
	while(philosopher->my_number_of_eat_times > 0)
	{	
			if(philosopher->data->flag_stop_sim == 0)
				eat_sleep_think_routine(philosopher);
			else
				return(NULL);

	}
	return(NULL);
}


void	*monitoring_routine(void	*input_data)
{
	t_data *data;
	data = (t_data*)input_data;

	while(1)
	{	//printf("data->num_of_philosophers %d\n", data->num_of_philosophers);
		//printf("data->number_of_eated_philos %d\n", data->number_of_eated_philos);
		
		pthread_mutex_lock(&data->flag_stop_sim_lock);
		if(data->flag_stop_sim == 1)
		{	
			pthread_mutex_unlock(&data->flag_stop_sim_lock);
			stop_simulation(data);
			return(NULL);
		}
		pthread_mutex_unlock(&data->flag_stop_sim_lock);				
		
		
		pthread_mutex_lock(&data->dinner_counter_lock);
		if (data->number_of_eated_philos == 0)
		{	
			pthread_mutex_lock(&data->flag_stop_sim_lock);
			data->flag_stop_sim = 1;
			pthread_mutex_unlock(&data->flag_stop_sim_lock);
			return(NULL);
		}
		pthread_mutex_unlock(&data->dinner_counter_lock);
		
		usleep(10000);	
	}

	return (NULL);
}

int start_simulation(t_data *data)
{	
	int i;
	
	i = 0;
	data->time_start_sim = get_real_time();
	while(i < data->num_of_philosophers)
	{
		if(pthread_create(&data->philosophers[i]->philo_thread, NULL, &philo_routine, data->philosophers[i]) != 0)
			return (0);
		i++;
	}
	if(pthread_create(&data->monitoring_thread, NULL, &monitoring_routine, data) != 0)
		return (0);
	pthread_join(data->monitoring_thread, NULL);

 return (1);
}

int	stop_simulation(t_data *data)
{
	int	i;

	i = 0;
	//printf("HEY\n");
	while (i < data->num_of_philosophers)
	{	
		//printf("i am %d res of my join thread: %d\n", i, (pthread_join(data->philosophers[i]->philo_thread, NULL)));
		if(pthread_join(data->philosophers[i]->philo_thread, NULL) != 0)
			return (0);
		i++;
	}

	return (1);
}




int main(int ac, char	**av)
{	
	printf("==============ok===============\n==============ok===============\n==============ok===============\n");
	t_data *data;
		
//	+check args
	data = init_data(ac, av);
	if(!data)
		return(1);
	if(!start_simulation(data))
		return (1);

	if(!stop_simulation(data))
	  	return (1);
	free_data(data);
	return(0);
}