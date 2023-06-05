#include "philo.h"

/*program parameters*/

typedef struct s_philosopher
{
	int p_id;
	int thread_id;
	int	my_time_of_starving;
	int my_number_of_eat_times;	// ho many times i ate
	int number_of_eat_times; //required
	int is_i_alive;
	int	time_to_eat;
	int	time_to_sleep;

}	t_philosopher;


typedef struct s_data
{
	int num_of_philosophers;
	int *forks;
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



/***************************************************************************/

// void* routine_of_philo(t_philosopher *philolosopher)
void* routine_of_philo(void	*arg)
{
	int i = 0;
	// for(int j = 0; j < 5; j++)
	// 	printf("again my id %d\n", philosopher[j].p_id);

	t_philosopher *philo = (t_philosopher	*)arg;
	int a = (philo->p_id);
	//printf("ii =%d\n", i);
	//printf("num =%d\n", philo->number_of_eat_times);
	while (i < philo->number_of_eat_times)
	{	
		printf("%d has taken a fork\n", philo->p_id);
		printf("%d has taken a fork\n", philo->p_id);
		printf("%d is eating\n", philo->p_id);
		usleep(philo->time_to_eat);
		printf("%d is sleeping\n", philo->p_id);
		usleep(philo->time_to_sleep);
		printf("%d is thinking\n", philo->p_id);
		i++;
	}
	return(0);
}


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
		data->philosopher[i].my_time_of_starving = 0;
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
		data->fork[i] = 1;
		i++;
	}
	return(0);
}

int main(int ac, char	**av)
{	
	t_data data;
	if (ac != 5 && ac != 6)
		return(0);
	process_arguments(&data, av);
	create_philosophers(&data);
	create_forks(&data);

	// printf("again my id %d\n", data.philosopher[0].p_id);
	// printf("again my id %d\n", data.philosopher[1].p_id);
	// printf("again my id %d\n", data.philosopher[2].p_id);
	// printf("again my id %d\n", data.philosopher[3].p_id);
	// printf("again my id %d\n", data.philosopher[4].p_id);

	start_threads(&data);


	return(0);
}
