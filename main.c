#include "philo.h"


// int	arg_is_num(char *av)
// {	
// 	int	i;
// 	i = 0;
// 	while(av[i] != '\0')
// 	{
// 		if ((av[i] > 47 && av[i] < 58) || av[i] == '+')
// 			i++;
// 		else
// 		{ 
// 			return (0);
// 		}
// 	}
// 	return(1);
// }



// //++++++++++++++++++++++++++++++++++TIME++++++++++++++++++++++++++++++++//

// long long get_current_time()	
// {
// 	struct timeval tv;
//     gettimeofday(&tv, NULL);
//     long long current_time = tv.tv_sec * 1000LL + tv.tv_usec / 1000; // Время в миллисекундах
//     //printf("Текущее время : %lld\n", milliseconds/1000);
//     return (current_time);
// }
// /***************************************************************************/

// void* routine_of_philo(void	*arg)
// {	
// 	t_philosopher *philo = (t_philosopher	*)arg;
// 	pthread_mutex_lock(philo->data_mutex);
// 	philo->start_time = get_current_time();
// 	philo->my_last_dinner = get_current_time();
// 	pthread_mutex_unlock(philo->data_mutex);

// 	if((philo->p_id)%2 == 0)
// 	{
// 		usleep(3000);
// 	}

// 	//printf("%lld %d my_last_dinner", (get_current_time()-philo->start_time), philo->p_id);
// 	while (philo->data->end == 0)
// 	{	printf("@@@@ I AM %d END IS %d\n", philo->p_id, philo->data->end);
// 		// pthread_mutex_lock(philo->data_mutex);
// 		// if(philo->is_i_alive == 0)
// 		// {
// 		// 	printf("I M DEAD!\n");
// 		// 	return(0);
// 		// }
// 		// pthread_mutex_unlock(philo->data_mutex);
// 		printf("%lld %d is thinking\n", (get_current_time()-philo->start_time), philo->p_id);
		
// 		if (!(philo->right_fork_mutex))
// 		{	printf("ololo\n");
// 			return(0);
// 		}

// 		if (philo->my_number_of_eat_times == philo->number_of_eat_times)
// 		{	
// 			pthread_mutex_lock(philo->data_mutex);
// 			philo->i_am_finished = 1;
// 			pthread_mutex_unlock(philo->data_mutex);
// 			return(0);
// 		}

// 		if((pthread_mutex_lock(philo->left_fork_mutex) == 0) && (pthread_mutex_lock(philo->right_fork_mutex) == 0))
// 		{					
// 				*philo->fork_left = 0;
// 				*philo->fork_right = 0;

// 				printf("%lld %d has taken a fork\n", (get_current_time()-philo->start_time), philo->p_id);
// 				printf("%lld %d has taken a fork\n", (get_current_time()-philo->start_time), philo->p_id);
// 				printf("%lld %d is eating\n", (get_current_time()-philo->start_time), philo->p_id);

// 				pthread_mutex_lock(philo->data_mutex);
// 				philo->my_last_dinner = get_current_time();
// 				pthread_mutex_unlock(philo->data_mutex);

// 				usleep(1000*(philo->time_to_eat));
// 				*philo->fork_left = 1;
// 				*philo->fork_right = 1;

// 				pthread_mutex_unlock(philo->left_fork_mutex);
// 				pthread_mutex_unlock(philo->right_fork_mutex);
				
// 				philo->my_number_of_eat_times++;				

// 				printf("%lld %d is sleeping\n", (get_current_time()-philo->start_time), philo->p_id);
// 				usleep(1000*(philo->time_to_sleep));
				
				
// 		}
// 		if(philo->data->end == 1)
// 			return(0);
			
// 	}
// 	return(0);
// }
 

// /*********************CHECKER****************************/
// /*********************CHECKER****************************/
// void* checker_routine(void	*arg)
// {
// 	t_data *data = (t_data	*)arg;
// 	int i = 0;
// 	// int a = 0;
// 	usleep(9000);
	
// 	while(1)
// 	{	
// 		if((data->number_of_eated_philos == data->num_of_philosophers -1) && (data->num_of_philosophers != 1))
// 		{	
// 			printf("all philos have been eaten\n");					
// 			return(0);
// 		}

// 		i = 0;
// 		while(i < data->num_of_philosophers)
// 		{
// 			pthread_mutex_lock(&data->data_mutex);			
		
// 			if(((get_current_time()) - data->philosopher[i].my_last_dinner) > (data->time_to_die))
// 			{	
// 				data->philosopher[i].is_i_alive = 0;
// 				printf("%lld %d died\n",(get_current_time()-data->philosopher[i].start_time), i+1);	
// 				data->end = 1;
// 				pthread_mutex_unlock(&data->data_mutex);				
// 				return(0);
// 			}	
// 			pthread_mutex_unlock(&data->data_mutex);

// 			pthread_mutex_lock(&data->data_mutex);
// 			if(data->philosopher[i].i_am_finished == 1)
// 			{				
// 				data->philosopher[i].i_am_finished = 0;
// 				data->number_of_eated_philos++;			
// 			}	
// 			pthread_mutex_unlock(&data->data_mutex);		
// 			i++;					
// 		}
// 		i = 0;
// 		//pthread_mutex_unlock(&data->data_mutex);
// 		//break;
// 	}
// 	return(0);
// }

// /*++++++++++++++++++++++++++START+++THREADS++++++++++++++++++++++++++++++*/
// /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// int start_threads(t_data *data)
// {
// 	int i = 0;
// 	pthread_t *threads = malloc(sizeof(pthread_t)*(data->num_of_philosophers));	
// 	data->threads = threads;
// 	pthread_t check_status;
// 	data->cheker_thread = &check_status;
// 	pthread_mutex_init(&data->data_mutex, NULL);

// 	pthread_create(&check_status, NULL, checker_routine, data);
// 	while (i < data->num_of_philosophers)
// 	{	
// 		data->philosopher[i].data_mutex = &data->data_mutex;
// 		pthread_create(&threads[i], NULL, routine_of_philo, &data->philosopher[i]);
// 		printf("i of thread = %d\n", i);
// 		//sleep (1);
// 		i++;
// 	}

	
// 	return(0);
// }








// /* if data->fork[i] = 1, the fork[i] is free. If 0 it's busy*/
// int create_forks(t_data *data)
// {	int i = 0;
	
// 	data->fork = malloc(sizeof(int)*(data->num_of_philosophers));
// 	data->fork_mutexes = malloc(sizeof(pthread_mutex_t)*(data->num_of_philosophers));
	
// 	while (i < data->num_of_philosophers)
// 	{
// 		data->fork[i] = 1; //1 -means the fork is available;		
// 		pthread_mutex_init(&data->fork_mutexes[i], NULL); // - creates mutex for each fork;
// 		i++;
// 	}
// 	i = 0;
	
// 	while (i < data->num_of_philosophers)
// 	{
// 		if(i == 0 && data->num_of_philosophers == 1)
// 		{
// 			data->philosopher[i].fork_left = &data->fork[i];
// 			data->philosopher[i].left_fork_mutex = &data->fork_mutexes[i];
// 		}

// 		else if(i == 0 && data->num_of_philosophers != 1)
// 		{	
// 			data->philosopher[i].fork_left = &data->fork[i];
// 			data->philosopher[i].left_fork_mutex = &data->fork_mutexes[i];

// 			data->philosopher[i].fork_right = &data->fork[data->num_of_philosophers-1];
// 			data->philosopher[i].right_fork_mutex = &data->fork_mutexes[data->num_of_philosophers-1];
// 		}
// 		else
// 		{	//printf("id fork: %d\n", i);
// 			data->philosopher[i].fork_left = &data->fork[i];
// 			data->philosopher[i].left_fork_mutex = &data->fork_mutexes[i];
// 			//printf("id right fork: %d\n", i-1);
// 			data->philosopher[i].fork_right = &data->fork[i-1];
// 			data->philosopher[i].right_fork_mutex = &data->fork_mutexes[i-1];

// 		}
// 		 //printf("fork in data  %d = %d\n", i, data->fork[i]);
// 		//printf("i'm %d left fork  = %d\n", i, *data->philosopher[i].fork_left);
// 		//printf("i'm %d right fork = %d\n", i, *data->philosopher[i].fork_right);
// 		i++;
// 	}	
// 	return(0);
// }



// static void	stop_simulation(t_data	*data)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (i < data->num_of_philosophers)
// 	{
// 		pthread_join(data->threads[i], NULL);
// 		i++;
// 	}
// 	if (data->num_of_philosophers)
// 		pthread_join(*data->cheker_thread, NULL);

// 	destroy_mutexes(data);
// 	//free_data(data);
// }




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
		philosophers[i]->my_number_of_eat_times = 0;
		if(pthread_mutex_init(&philosophers[i]->dinner_counter_lock, NULL) != 0)
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
		data->req_number_of_eat = ft_atoi(av[5]);
	else
		data->req_number_of_eat = INT_MAX;

	data->number_of_eated_philos = 0;
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

void	eat_sleep_think_routine(t_philosopher	*philosopher)
{	//printf(" I AM %d MY FORKS ARE %d AND %d\n", philosopher->id, philosopher->fork_left, philosopher->fork_right);
	if (((pthread_mutex_lock(&philosopher->data->forks[philosopher->fork_left])) == 0) &&  (pthread_mutex_lock(&philosopher->data->forks[philosopher->fork_right])) == 0)
	{	
		printf("%lld %d has taken a fork\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
		printf("%lld %d has taken a fork\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
		printf("%lld %d is eating\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
		usleep((philosopher->data->time_to_eat)*1000);
		pthread_mutex_unlock(&philosopher->data->forks[philosopher->fork_right]);
		pthread_mutex_unlock(&philosopher->data->forks[philosopher->fork_left]);

		pthread_mutex_lock(&philosopher->dinner_counter_lock);
		philosopher->my_number_of_eat_times++;
		pthread_mutex_unlock(&philosopher->dinner_counter_lock);

		printf("%lld %d is sleeping\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
		usleep((philosopher->data->time_to_sleep)*1000);
	}
	// pthread_mutex_lock(&philosopher->data->flag_stop_sim_lock);
	// philosopher->data->flag_stop_sim = 1;
	// pthread_mutex_unlock(&philosopher->data->flag_stop_sim_lock);
	printf("%lld %d is thinking\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
}


void	*philo_routine(void *philo)
{
	t_philosopher *philosopher;
	philosopher = (t_philosopher *)philo;
	philosopher->last_dinner = philosopher->data->time_start_sim;
	printf("%lld %d is thinking\n", (get_real_time() - philosopher->data->time_start_sim), (philosopher->id));
	if(philosopher->id % 2 != 0)
		usleep((philosopher->data->time_to_eat/2)*1000);
	while(is_sim_stop(philosopher->data) == 0)
	{	
		if(philosopher->data->req_number_of_eat > philosopher->my_number_of_eat_times)
			eat_sleep_think_routine(philosopher);
		else
			return(NULL);
	}

	return(NULL);
}


void	*monitoring_routine(void	*data)
{
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

 return (1);
}

int	stop_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philosophers)
	{
		if(pthread_join(data->philosophers[i]->philo_thread, NULL) != 0)
			return (0);
		i++;
	}
	if(pthread_join(data->monitoring_thread, NULL) != 0)
			return (0);
	return (1);
}




int main(int ac, char	**av)
{	
	printf("==============ok===============\n==============ok===============\n==============ok===============\n");
	int i = 0;
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