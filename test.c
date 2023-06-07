# include <stdio.h>
# include <pthread.h>
 #include <unistd.h>
  #include <stdlib.h>
  #include <sys/time.h>

int i = 0;

long long get_current_time()	
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
    long long current_time = tv.tv_sec * 1000LL + tv.tv_usec / 1000; // Время в миллисекундах
    //printf("Текущее время : %lld\n", milliseconds/1000);
    return (current_time);
}


typedef struct s_philo
{
	int p_id;	//my number. Start number is 1
	int thread_id;
	int *fork_left; // number of fork = philosopher's [i]
	int *fork_right;
    long long start_time; // number of fork = philosopher's [i]-1
	pthread_mutex_t left_fork;
	pthread_mutex_t right_fork;

}	t_philo;



// void* routine_of_philo(t_philosopher *philolosopher)
void* routine(void	*arg)
{	
	
	t_philo *philo = (t_philo	*)arg;
	philo->start_time = get_current_time();
	while (1)
	{	
		
		i++;
		if((*philo->fork_left == 1) && (*philo->fork_right == 1))
		{	
			printf("adress 1 of left fork: %p\n", philo->fork_left);
			pthread_mutex_lock(&philo->left_fork);
			pthread_mutex_lock(&philo->right_fork);
			*philo->fork_left = 0;
			*philo->fork_right = 0;
			printf("%lld  1 has taken a fork; fork = %d\n", (get_current_time()-philo->start_time), *philo->fork_left);
			printf("%lld  1 has taken a fork; fork = %d\n", (get_current_time()-philo->start_time),  *philo->fork_right);
			printf("%lld  1 is eating\n", (get_current_time()-philo->start_time));
			sleep(1);
			*philo->fork_left = 1;
			*philo->fork_right = 1;
			pthread_mutex_unlock(&philo->left_fork);	
			pthread_mutex_unlock(&philo->right_fork);	
			//printf("%d philo->my_number_of_eat_times++:%d\n", philo->p_id, philo->my_number_of_eat_times);
			printf("%lld 1 is sleeping\n", (get_current_time()-philo->start_time));
            sleep(1);
		}
	}
	return(0);
}

void* routine2(void	*arg)
{	

	t_philo *philo = (t_philo	*)arg;
	philo->start_time = get_current_time();
	while (1)
	{	
		
		i++;
		if((*philo->fork_left == 1) && (*philo->fork_right == 1))
		{	
			printf("adress 2 of left fork: %p\n", philo->fork_left);
			pthread_mutex_lock(&philo->left_fork);
			pthread_mutex_lock(&philo->right_fork);

			*philo->fork_left = 0;
			*philo->fork_right = 0;
			printf("%lld 2 has taken a fork; fork = %d\n", (get_current_time()-philo->start_time), *philo->fork_left);
			printf("%lld 2 has taken a fork; fork = %d\n", (get_current_time()-philo->start_time),  *philo->fork_right);
			printf("%lld 2 is eating\n", (get_current_time()-philo->start_time));
			sleep(1);
			*philo->fork_left = 1;
			*philo->fork_right = 1;
			pthread_mutex_unlock(&philo->left_fork);	
			pthread_mutex_unlock(&philo->right_fork);	
			//printf("%d philo->my_number_of_eat_times++:%d\n", philo->p_id, philo->my_number_of_eat_times);
			printf("%lld 2 is sleeping\n", (get_current_time()-philo->start_time));
            sleep(1);
		}
	}
	return(0);
}




int main()
{   
    t_philo philo;

    pthread_t thread1;
    pthread_t thread2;
	

	pthread_mutex_init(&philo.left_fork, NULL);
	pthread_mutex_init(&philo.right_fork, NULL);


	int a = 1;
    philo.fork_left = &a;
	philo.fork_right = &a;
	printf("hey\n");
	pthread_create(&thread1, NULL, routine, &philo);
    pthread_create(&thread2, NULL, routine2, &philo);

		//pthread_join(threads[i], NULL);


	// int j = 0;
	// while (j < philo->num_of_philosophers)
	// {	
	// 	pthread_join(threads[j], NULL);
	// 	j++;
	// }	
	// pthread_join(check_status, NULL);
	sleep(5);
	 return(0);
}
