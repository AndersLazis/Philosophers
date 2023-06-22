#include "philo.h"


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
		//printf(" i am %d philosophers[i]->my_number_of_eat_times %d\n", philosophers[i]->id, philosophers[i]->my_number_of_eat_times);
		if(pthread_mutex_init(&philosophers[i]->meal_count_lock, NULL) != 0)
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
	// data->one_dead = 0;
	data->req_eat_count = -1;
	printf("data->req_eat_count %d\n", data->req_eat_count);
	if (ac == 6)
	{
		data->req_eat_count = ft_atoi(av[5]);
		printf("data->req_eat_count %d\n", data->req_eat_count);
	}		

	data->philosophers = init_philos(data);
	if(!data->philosophers)
		return(NULL);
	data->forks = create_forks(data);
	if(!data->forks)
		return(NULL);
	if(pthread_mutex_init(&data->flag_stop_sim_lock, NULL) != 0)
		return(NULL);
	if(pthread_mutex_init(&data->stdout_lock, NULL) != 0)
		return(NULL);
	return(data);
}


long long get_real_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}


void	eat_sleep_think_routine(t_philosopher	*philosopher)
{	
	
		pthread_mutex_lock(&philosopher->data->forks[philosopher->fork_left]);
		write_status(philosopher, 0, FORK_1);
		pthread_mutex_lock(&philosopher->data->forks[philosopher->fork_right]);
		write_status(philosopher, 0, FORK_2);
		write_status(philosopher, 0, EATING);

		pthread_mutex_lock(&philosopher->meal_count_lock);
		philosopher->last_dinner = get_real_time();
		pthread_mutex_unlock(&philosopher->meal_count_lock);

		alarm_clock_loop(philosopher->data, philosopher->data->time_to_eat);
		if(simulation_ended(philosopher->data) == 0)
		{
			pthread_mutex_lock(&philosopher->meal_count_lock);
			philosopher->eated_dinners+=1;
			//printf("%d philosopher->eated_dinners-- %d\n", philosopher->id, philosopher->eated_dinners);
			pthread_mutex_unlock(&philosopher->meal_count_lock);
		}
		write_status(philosopher, 0, SLEEPING);
		pthread_mutex_unlock(&philosopher->data->forks[philosopher->fork_left]);
		pthread_mutex_unlock(&philosopher->data->forks[philosopher->fork_right]);	
		alarm_clock_loop(philosopher->data, philosopher->data->time_to_sleep);
}

void	*one_philo_routine(t_philosopher	*philosopher)
{	
	
		pthread_mutex_lock(&philosopher->data->forks[philosopher->fork_right]);
		write_status(philosopher, 0, FORK_2);
		alarm_clock_loop(philosopher->data, philosopher->data->time_to_die);
		write_status(philosopher, 0, DIED);
		pthread_mutex_unlock(&philosopher->data->forks[philosopher->fork_right]);	
		return(NULL);
}



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


static void	think_time_calc(t_philosopher *philo, int no_print_wait)
{
	long long	time_to_think;

	pthread_mutex_lock(&philo->meal_count_lock);
	time_to_think = (philo->data->time_to_die - (get_real_time() - philo->last_dinner) - philo->data->time_to_eat) / 2;
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

void	*philo_routine(void *philo)
{
	t_philosopher *philosopher;

	philosopher = (t_philosopher *)philo;

	if (philosopher->data->req_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philosopher->meal_count_lock);
	philosopher->last_dinner = philosopher->data->time_start_sim;
	pthread_mutex_unlock(&philosopher->meal_count_lock);
	delay(philosopher->data->time_start_sim);
	//printf("%d\n", philosopher->eated_dinners);
	if (philosopher->data->time_to_die == 0)
		return (NULL);
	if(philosopher->data->num_of_philosophers == 1)
		return(one_philo_routine(philosopher));
	if (philosopher->id % 2)
		think_time_calc(philosopher, 1);

	while(simulation_ended(philosopher->data) == 0)
	{	
		eat_sleep_think_routine(philosopher);
		think_time_calc(philosopher, 0);
	}
	return(NULL);	
}


int kill_philo(t_philosopher *philosopher)
{
	long long time;

	time = get_real_time();
	// printf("time: %lld\n", time);
	// printf("philosopher->last_dinner: %lld\n", philosopher->last_dinner);
	// printf("philosopher->data->time_to_die: %d\n", philosopher->data->time_to_die);
	if((time - (philosopher->last_dinner)) >= (philosopher->data->time_to_die))
	{	
		pthread_mutex_lock(&philosopher->data->flag_stop_sim_lock);
		philosopher->data->flag_stop_sim = 1;
		pthread_mutex_unlock(&philosopher->data->flag_stop_sim_lock);
		write_status(philosopher, 1, DIED);
		pthread_mutex_unlock(&philosopher->meal_count_lock);
		return(1);
	}
	return(0);
}


static void	print_status(t_philosopher *philo, char *str)
{
	printf("%lld %d %s\n", get_real_time() - philo->data->time_start_sim,
		philo->id + 1, str);
}


void	write_status(t_philosopher *philo, int supervisor_flag, int status)
{
	pthread_mutex_lock(&philo->data->stdout_lock);
	if (simulation_ended(philo->data) == 1 && supervisor_flag == 0)
	{
		pthread_mutex_unlock(&philo->data->stdout_lock);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == FORK_1 || status == FORK_2)
		print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->data->stdout_lock);
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


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void	*monitoring_routine(void	*input_data)
{
	t_data *data;
	data = (t_data*)input_data;

	if(data->req_eat_count == 0)
		return(NULL);

	pthread_mutex_lock(&data->flag_stop_sim_lock);
	data->flag_stop_sim = 0;
	pthread_mutex_unlock(&data->flag_stop_sim_lock);
	delay(data->time_start_sim);
	while(1)
	{		
		if (stop_simulation(data) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}


int start_simulation(t_data *data)
{	
	int i;
	
	i = 0;
	data->time_start_sim = get_real_time() + (data->num_of_philosophers * 2);
	while(i < data->num_of_philosophers)
	{
		if(pthread_create(&data->philosophers[i]->philo_thread, NULL, &philo_routine, data->philosophers[i]) != 0)
			return (0);
		i++;
	}
	if(data->num_of_philosophers > 1)
	{
		if(pthread_create(&data->monitoring_thread, NULL, &monitoring_routine, data) != 0)
			return (0);	
	}
 	return (1);
}

int	ft_isdigit(int c)
{
	if ((c > 47 && c < 58) ||  c == 43 || c == 45)
		return (1);
	else
		return (0);
}

int	check_args(int	ac, char	**av)
{
	int i;
	int j;

	i = 1;
	
	if(ac != 5 && ac != 6)
		printf("Error: wrong arguments number.\n");
	while(av[i])
	{
		j = 0;
		while (av[i][j]!= '\0')
		{
			if((!ft_isdigit(av[i][j])) || (ft_atoi(av[i]) < 0) || (ft_atoi(av[i]) > INT_MAX) || (ft_atoi(av[1]) >200 || (ft_atoi(av[2]) < 60)) || (ft_atoi(av[3]) < 60) || (ft_atoi(av[4]) < 60))
			{
				printf("Error: invalid arguments.\n");
				return(0);
			}
			// printf("%c\n", av[i][j]);
			j++;
		}	
		i++;
	}
	return(1);
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

int main(int ac, char	**av)
{	
	printf("===========================ok==========================\n======================ok=========================\n==============ok=======================\n");
	t_data *data;
		
	if(!check_args(ac, av))
		return(1);
	data = init_data(ac, av);
	if(!data)
		return(1);
	if(!start_simulation(data))
		return (1);
	finish_simulation(data);
	free_data(data);
	return(0);
}