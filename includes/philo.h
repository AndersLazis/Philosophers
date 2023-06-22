#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>



typedef struct s_philosopher	t_philosopher;



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


int	ft_isdigit(int c);
int	ft_atoi(const char *str);
int	check_args(int	ac, char	**av);
void	destroy_mutexes(t_data	*data);
int	finish_simulation(t_data	*data);
int	simulation_ended(t_data *data);
int	stop_simulation(t_data *data);
int free_data(t_data *data);
t_data	*init_data(int ac, char	**av);
t_philosopher	**init_philos(t_data *data);
pthread_mutex_t	*create_forks(t_data *data);
void	*philo_routine(void *philo);
void	*one_philo_routine(t_philosopher	*philosopher);
void	*monitoring_routine(void	*input_data);
void	eat_sleep_think_routine(t_philosopher	*philosopher);
int kill_philo(t_philosopher *philosopher);
void	delay(long long start_time);
void	alarm_clock_loop(t_data *data, int sleep_time);
long long get_real_time(void);
void	write_status(t_philosopher *philo, int supervisor_flag, int status);
int start_simulation(t_data *data);
void	think_time_calc(t_philosopher *philo, int no_print_wait);

# define DIED 0
# define EATING 1
# define SLEEPING 2
# define THINKING 3
# define FORK 4
// # define FORK_1 4
// # define FORK_2 5


 #endif