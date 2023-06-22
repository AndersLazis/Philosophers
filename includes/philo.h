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
	// printf("ATOI %d\n", res * sign);
	return (res * sign);
}


typedef struct s_data
{
	int num_of_philosophers; 
	int	time_to_die; 
	int	time_to_eat; 
	int	time_to_sleep; 
	int req_eat_count;
	t_philosopher **philosophers;
	pthread_mutex_t	*forks;
	long long time_start_sim;
	pthread_t monitoring_thread;
	int flag_stop_sim;
	pthread_mutex_t	stdout_lock;
	pthread_mutex_t	flag_stop_sim_lock;

}	t_data;

typedef struct s_philosopher
{
	int id;	
	t_data *data;

	int eated_dinners; //
	int	fork_left;
	int fork_right;
	pthread_t	philo_thread;
	long long last_dinner;
	int	dead;
	pthread_mutex_t	meal_count_lock;
	
}	t_philosopher;

int	stop_simulation(t_data *data);
void	alarm_clock_loop(t_data *data, int sleep_time);
static void	print_status(t_philosopher *philo, char *str);
void	write_status(t_philosopher *philo, int supervisor_flag, int status);



// MACROS

# define PROG_NAME	"philo"
# define ERROR_ARGS	"%s invalid input: enter a string of digits only \
./philo <num_of_philos> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_meals]\n"
# define ERROR_DIGIT	"%s invalid input: %s: \
 string must consist of digits\n"
# define ERROR_MAX_INT	"%s invalid input: %s: \
 string must not exceed unsigned INT_MAX\n"
# define ERROR_PHILO	"%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define ERROR_TTD	"%s invalid input: \
<time_to_die> has to be between %s ms.\n"
# define ERROR_THREAD	"Error: thread creation failed\n"
# define ERROR_MALLOC	"Error: malloc failed.\n"
# define ERROR_MUTEX	"Error: mutex creation failed.\n"
# define DIED 0
# define EATING 1
# define SLEEPING 2
# define THINKING 3
# define FORK_1 4
# define FORK_2 5




 #endif