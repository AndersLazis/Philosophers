#include "philo.h"

/*program parameters*/

typedef struct s_philosopher
{
	int p_id;	//my number. Start number is 1
	int thread_id;
	int	my_last_dinner;	//timestamp of my last dinner
	int my_number_of_eat_times;	// ho many times i already ate
	int number_of_eat_times; //required
	int is_i_alive; //1 if I alive
	int	time_to_eat;	//from args
	int	time_to_sleep;	//from args
	int *fork_left; // number of fork = philosopher's [i]
	int *fork_right; // number of fork = philosopher's [i]-1
	long long start_time;

}	t_philosopher;


typedef struct s_data
{
	int num_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int number_of_eat_times;
	int	*fork;
	t_philosopher *philosopher;

}	t_data;


/*writes args to data struct*/
int process_arguments(t_data *data, char **av)
{	
	data->num_of_philosophers = ft_atoi(av[1]);
	data->time_to_die = 1000 * (ft_atoi(av[2]));
	data->time_to_eat = 1000 * (ft_atoi(av[3]));
	data->time_to_sleep = 1000 * (ft_atoi(av[4]));
	if (av[5])
	{
		data->number_of_eat_times = ft_atoi(av[5]);
	}
	return(0);
}

/***********************TIME**********************/
long long get_current_time()	
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
    long long current_time = tv.tv_sec * 1000LL + tv.tv_usec / 1000; // Время в миллисекундах
    //printf("Текущее время : %lld\n", milliseconds/1000);
    return (current_time);
}


/***************************************************************************/

// void* routine_of_philo(t_philosopher *philolosopher)
void* routine_of_philo(void	*arg)
{	
	pthread_mutex_t left_fork;
	pthread_mutex_t right_fork;

	pthread_mutex_init(&left_fork, NULL);
	pthread_mutex_init(&right_fork, NULL);

	
	int i = 0;
	// for(int j = 0; j < 5; j++)
	// 	printf("again my id %d\n", philosopher[j].p_id);

	t_philosopher *philo = (t_philosopher	*)arg;
	philo->start_time = get_current_time();
	while (i < philo->number_of_eat_times)
	{			
		// printf("FORK left %d\n", *philo->fork_left);
		// printf("FORK RIGHT %d\n", *philo->fork_right);
		if((*philo->fork_left == 1) && (*philo->fork_right == 1))
		{	
			pthread_mutex_lock(&left_fork);
			pthread_mutex_lock(&right_fork);	

			*philo->fork_left = 0;	
			*philo->fork_right = 0;	
			printf("%lld %d has taken a left fork\n", (get_current_time()-philo->start_time), philo->p_id);
			printf("%lld %d has taken a right fork\n", (get_current_time()-philo->start_time), philo->p_id);
			printf("%lld %d is eating\n", (get_current_time()-philo->start_time), philo->p_id);
			philo->my_last_dinner = get_current_time();
			usleep(philo->time_to_eat);
			*philo->fork_left = 1;
			*philo->fork_right = 1;
			pthread_mutex_unlock(&left_fork);	
			pthread_mutex_unlock(&right_fork);	

			printf("%lld %d is sleeping\n", (get_current_time()-philo->start_time), philo->p_id);
			usleep(philo->time_to_sleep);
			printf("%lld %d is thinking\n", (get_current_time()-philo->start_time), philo->p_id);
		}	
		i++;
	}
	return(0);
}


int create_one_thread

int start_threads(t_data *data)
{
	int i = 0;
	pthread_t *threads = malloc(sizeof(pthread_t)*(data->num_of_philosophers));	
	printf("qty = %d\n", data->num_of_philosophers);
	while (i < data->num_of_philosophers)
	{	
		//printf("i = %d\n", i);
		pthread_create(&threads[i], NULL, routine_of_philo, &data->philosopher[i]);
		//pthread_join(threads[i], NULL);
		i++;
	}
	int j = 0;
	while (j < data->num_of_philosophers)
	{	
		pthread_join(threads[j], NULL);
		j++;
	}
	return(0);
}


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
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
		i++;
	}
	return(0);
}

/* if data->fork[i] = 1, the fork[i] is free. If 0 it's busy*/
int create_forks(t_data *data)
{	int i = 0;

	data->fork = malloc(sizeof(int)*(data->num_of_philosophers));
	while (i < data->num_of_philosophers)
	{
		data->fork[i] = 1; //1 -means the fork is available;
		i++;
	}
	i = 0;
	while (i < data->num_of_philosophers)
	{
		if(i == 0)
		{
			data->philosopher[i].fork_left = &data->fork[i];
			data->philosopher[i].fork_right = &data->fork[data->num_of_philosophers-1];
		}
		else
		{
			data->philosopher[i].fork_left = &data->fork[i];
			data->philosopher[i].fork_right = &data->fork[i-1];
		}
		 //printf("fork in data  %d = %d\n", i, data->fork[i]);
		//printf("i'm %d left fork  = %d\n", i, *data->philosopher[i].fork_left);
		//printf("i'm %d right fork = %d\n", i, *data->philosopher[i].fork_right);

		i++;
	}

	return(0);
}

int main(int ac, char	**av)
{	
	t_data data;
	if (ac != 5 && ac != 6)
		return(0);
	//printf("%lld\n", init_time());
	process_arguments(&data, av);
	create_philosophers(&data);
	create_forks(&data);
	start_threads(&data);


	return(0);
}
