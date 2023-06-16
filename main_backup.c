#include "philo.h"


int	arg_is_num(char *av)
{	
	int	i;
	i = 0;
	while(av[i] != '\0')
	{
		if ((av[i] > 47 && av[i] < 58) || av[i] == '+')
			i++;
		else
		{ 
			return (0);
		}
	}
	return(1);
}

/*writes args to data struct*/
int process_arguments(t_data *data, char **av)
{	
	//printf("res:%s\n", arg_is_num(av[1]));
	if((arg_is_num(av[1]) == 0) || (arg_is_num(av[2]) == 0) || (arg_is_num(av[3]) == 0) || (arg_is_num(av[4]) == 0))
	{
		printf("Error: arguments are invalid\n");
		return(1);
	}
	if (av[5])
	{
		if(arg_is_num(av[5]) == 0)
		{
			printf("Error: arguments are invalid\n");
			return(1);
		}
		else
		{
			data->number_of_eat_times = ft_atoi(av[5]);
			if(ft_atoi(av[5]) == 0)
				{
					printf("Error: arguments are invalid\n");
					return(1);
				}
		}
	}
	else
		data->number_of_eat_times = INT_MAX;

	data->num_of_philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);

	if(data->num_of_philosophers <= 0 || data->time_to_die <= 0 || data->time_to_eat < 0 || data->time_to_sleep < 0)
	{
		printf("Error: arguments are invalid\n");
		return(1);
	}


	data->number_of_eated_philos = 0;	

	return(0);
}

//++++++++++++++++++++++++++++++++++TIME++++++++++++++++++++++++++++++++//

long long get_current_time()	
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
    long long current_time = tv.tv_sec * 1000LL + tv.tv_usec / 1000; // Время в миллисекундах
    //printf("Текущее время : %lld\n", milliseconds/1000);
    return (current_time);
}
/***************************************************************************/

void* routine_of_philo(void	*arg)
{	
	t_philosopher *philo = (t_philosopher	*)arg;
	pthread_mutex_lock(philo->data_mutex);
	philo->start_time = get_current_time();
	philo->my_last_dinner = get_current_time();
	pthread_mutex_unlock(philo->data_mutex);

	if((philo->p_id)%2 == 0)
	{
		usleep(3000);
	}

	//printf("%lld %d my_last_dinner", (get_current_time()-philo->start_time), philo->p_id);
	while (philo->data->end == 0)
	{	printf("@@@@ I AM %d END IS %d\n", philo->p_id, philo->data->end);
		// pthread_mutex_lock(philo->data_mutex);
		// if(philo->is_i_alive == 0)
		// {
		// 	printf("I M DEAD!\n");
		// 	return(0);
		// }
		// pthread_mutex_unlock(philo->data_mutex);
		printf("%lld %d is thinking\n", (get_current_time()-philo->start_time), philo->p_id);
		
		if (!(philo->right_fork_mutex))
		{	printf("ololo\n");
			return(0);
		}

		if (philo->my_number_of_eat_times == philo->number_of_eat_times)
		{	
			pthread_mutex_lock(philo->data_mutex);
			philo->i_am_finished = 1;
			pthread_mutex_unlock(philo->data_mutex);
			return(0);
		}

		if((pthread_mutex_lock(philo->left_fork_mutex) == 0) && (pthread_mutex_lock(philo->right_fork_mutex) == 0))
		{					
				*philo->fork_left = 0;
				*philo->fork_right = 0;

				printf("%lld %d has taken a fork\n", (get_current_time()-philo->start_time), philo->p_id);
				printf("%lld %d has taken a fork\n", (get_current_time()-philo->start_time), philo->p_id);
				printf("%lld %d is eating\n", (get_current_time()-philo->start_time), philo->p_id);

				pthread_mutex_lock(philo->data_mutex);
				philo->my_last_dinner = get_current_time();
				pthread_mutex_unlock(philo->data_mutex);

				usleep(1000*(philo->time_to_eat));
				*philo->fork_left = 1;
				*philo->fork_right = 1;

				pthread_mutex_unlock(philo->left_fork_mutex);
				pthread_mutex_unlock(philo->right_fork_mutex);
				
				philo->my_number_of_eat_times++;				

				printf("%lld %d is sleeping\n", (get_current_time()-philo->start_time), philo->p_id);
				usleep(1000*(philo->time_to_sleep));
				
				
		}
		if(philo->data->end == 1)
			return(0);
			
	}
	return(0);
}
 

/*********************CHECKER****************************/
/*********************CHECKER****************************/
void* checker_routine(void	*arg)
{
	t_data *data = (t_data	*)arg;
	int i = 0;
	// int a = 0;
	usleep(9000);
	
	while(1)
	{	
		if((data->number_of_eated_philos == data->num_of_philosophers -1) && (data->num_of_philosophers != 1))
		{	
			printf("all philos have been eaten\n");					
			return(0);
		}

		i = 0;
		while(i < data->num_of_philosophers)
		{
			pthread_mutex_lock(&data->data_mutex);			
		
			if(((get_current_time()) - data->philosopher[i].my_last_dinner) > (data->time_to_die))
			{	
				data->philosopher[i].is_i_alive = 0;
				printf("%lld %d died\n",(get_current_time()-data->philosopher[i].start_time), i+1);	
				data->end = 1;
				pthread_mutex_unlock(&data->data_mutex);				
				return(0);
			}	
			pthread_mutex_unlock(&data->data_mutex);

			pthread_mutex_lock(&data->data_mutex);
			if(data->philosopher[i].i_am_finished == 1)
			{				
				data->philosopher[i].i_am_finished = 0;
				data->number_of_eated_philos++;			
			}	
			pthread_mutex_unlock(&data->data_mutex);		
			i++;					
		}
		i = 0;
		//pthread_mutex_unlock(&data->data_mutex);
		//break;
	}
	return(0);
}

/*++++++++++++++++++++++++++START+++THREADS++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int start_threads(t_data *data)
{
	int i = 0;
	pthread_t *threads = malloc(sizeof(pthread_t)*(data->num_of_philosophers));	
	data->threads = threads;
	pthread_t check_status;
	data->cheker_thread = &check_status;
	pthread_mutex_init(&data->data_mutex, NULL);

	pthread_create(&check_status, NULL, checker_routine, data);
	while (i < data->num_of_philosophers)
	{	
		data->philosopher[i].data_mutex = &data->data_mutex;
		pthread_create(&threads[i], NULL, routine_of_philo, &data->philosopher[i]);
		printf("i of thread = %d\n", i);
		//sleep (1);
		i++;
	}

	
	return(0);
}



int create_philosophers(t_data *data)
{	
	int i = 0;

	data->philosopher = malloc(sizeof(t_philosopher)*(data->num_of_philosophers));
	while (i <= data->num_of_philosophers)
	{
		data->philosopher[i].p_id = i+1;
		data->philosopher[i].my_last_dinner = 0;
		data->philosopher[i].time_to_eat = data->time_to_eat;
		data->philosopher[i].my_number_of_eat_times = 0;
		data->philosopher[i].number_of_eat_times = data->number_of_eat_times;
		data->philosopher[i].time_to_sleep = data->time_to_sleep;
		data->philosopher[i].is_i_alive = 1;
		data->philosopher[i].data = data;
		i++;
	}
	return(0);
}




/* if data->fork[i] = 1, the fork[i] is free. If 0 it's busy*/
int create_forks(t_data *data)
{	int i = 0;
	
	data->fork = malloc(sizeof(int)*(data->num_of_philosophers));
	data->fork_mutexes = malloc(sizeof(pthread_mutex_t)*(data->num_of_philosophers));
	
	while (i < data->num_of_philosophers)
	{
		data->fork[i] = 1; //1 -means the fork is available;		
		pthread_mutex_init(&data->fork_mutexes[i], NULL); // - creates mutex for each fork;
		i++;
	}
	i = 0;
	
	while (i < data->num_of_philosophers)
	{
		if(i == 0 && data->num_of_philosophers == 1)
		{
			data->philosopher[i].fork_left = &data->fork[i];
			data->philosopher[i].left_fork_mutex = &data->fork_mutexes[i];
		}

		else if(i == 0 && data->num_of_philosophers != 1)
		{	
			data->philosopher[i].fork_left = &data->fork[i];
			data->philosopher[i].left_fork_mutex = &data->fork_mutexes[i];

			data->philosopher[i].fork_right = &data->fork[data->num_of_philosophers-1];
			data->philosopher[i].right_fork_mutex = &data->fork_mutexes[data->num_of_philosophers-1];
		}
		else
		{	//printf("id fork: %d\n", i);
			data->philosopher[i].fork_left = &data->fork[i];
			data->philosopher[i].left_fork_mutex = &data->fork_mutexes[i];
			//printf("id right fork: %d\n", i-1);
			data->philosopher[i].fork_right = &data->fork[i-1];
			data->philosopher[i].right_fork_mutex = &data->fork_mutexes[i-1];

		}
		 //printf("fork in data  %d = %d\n", i, data->fork[i]);
		//printf("i'm %d left fork  = %d\n", i, *data->philosopher[i].fork_left);
		//printf("i'm %d right fork = %d\n", i, *data->philosopher[i].fork_right);
		i++;
	}	
	return(0);
}



static void	stop_simulation(t_data	*data)
{
	unsigned int	i;

	i = 0;
	while (i < data->num_of_philosophers)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	if (data->num_of_philosophers)
		pthread_join(*data->cheker_thread, NULL);

	destroy_mutexes(data);
	//free_data(data);
}



int main(int ac, char	**av)
{	int i = 0;
	t_data data;
	data.end = 0;	
	if (ac != 5 && ac != 6)
		return(0);
	//printf("%lld\n", init_time());
	if((process_arguments(&data, av)) == 1)
		return(0);
	
	create_philosophers(&data);
	create_forks(&data);
	
	start_threads(&data);
	stop_simulation(&data);

	// printf("YEHAAAAA!\n");
	// printf("%d\n", data.num_of_philosophers);



	// int j = 0;

	//  i = 0;
	// printf("HAHAHAAAAA!\n");
	// // while(i < data.num_of_philosophers)
	// // {	printf("i = %d\n", i);
	// // 	printf("res %d\n", pthread_join(data.threads[i], NULL));				
	// // 	i++;
	// // }
	// 	while(i < data.num_of_philosophers)
	// {	printf("i = %d\n", i);
	// 	printf("res %d\n", pthread_detach(data.threads[i]));				
	// 	i++;
	// }

	// 	while (j < data.num_of_philosophers)
	// {			
	// 	if(data.philosopher[0].right_fork_mutex)
	// 		pthread_mutex_destroy(data.philosopher[0].right_fork_mutex);
	// 	pthread_mutex_destroy(data.philosopher[j].left_fork_mutex);	
	//  	j++;
	// }	

	// pthread_mutex_destroy(data.fork_mutexes);
	// pthread_mutex_destroy(&data.data_mutex);

	// printf("pizdez!\n");
	// free(data.threads);	
	// free(data.fork);
	// free(&data);

	return(0);
}