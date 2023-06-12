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

pthread_mutex_t left_fork;
pthread_mutex_t right_fork;

typedef struct s_data
{
	int p_id;	//my number. Start number is 1
	int thread_id;
	int fork_left; // number of fork = datasopher's [i]
	int fork_right;
    long long start_time; // number of fork = datasopher's [i]-1
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;

}	t_data;



// void* routine_of_data(t_datasopher *datalosopher)
void* routine(void	*arg)
{	
	int i = 0;
	t_data *data = (t_data	*)arg;
	printf("=============HEEey===============\n");
	pthread_mutex_lock(data->left_fork);
	printf("+++++++++++HEEey+++++++++++\n");
	data->start_time = get_current_time();
	pthread_mutex_unlock(data->left_fork);

	while ( i < 3)
	{	
		pthread_mutex_lock(data->left_fork);
		i++;
		pthread_mutex_unlock(data->left_fork);
			
			printf("data->left_fork %d\n", data->fork_left);
			
		if((data->fork_left == 1) && (data->fork_right == 1))
		{	
			
			pthread_mutex_lock(data->left_fork);
			pthread_mutex_lock(data->right_fork);
			printf("adress 1 of left fork: %d\n", data->fork_left);
			data->fork_left = 0;
			data->fork_right = 0;
			printf("%lld  1 has taken a fork; fork = %d\n", (get_current_time()-data->start_time), data->fork_left);
			printf("%lld  1 has taken a fork; fork = %d\n", (get_current_time()-data->start_time),  data->fork_right);
			printf("%lld  1 is eating\n", (get_current_time()-data->start_time));
			sleep(1);
			data->fork_left = 1;
			data->fork_right = 1;
			printf("%lld 1 is sleeping\n", (get_current_time()-data->start_time));
			pthread_mutex_unlock(data->left_fork);	
			pthread_mutex_unlock(data->right_fork);	
			//printf("%d data->my_number_of_eat_times++:%d\n", data->p_id, data->my_number_of_eat_times);
			
            sleep(1);
		}
		i++;
	}
	return(0);
}

void* routine2(void	*arg)
{	
	int i = 0;
	t_data *data = (t_data	*)arg;
	pthread_mutex_lock(data->left_fork);
	data->start_time = get_current_time();
	pthread_mutex_unlock(data->left_fork);
	while (i<3)
	{	
		pthread_mutex_lock(data->left_fork);
		i++;
		pthread_mutex_unlock(data->left_fork);
		if((data->fork_left == 1) && (data->fork_right == 1))
		{	
			
			pthread_mutex_lock(data->left_fork);
			pthread_mutex_lock(data->right_fork);
			printf("adress 2 of left fork: %d\n", data->fork_left);

			data->fork_left = 1;
			data->fork_right = 1;
			printf("%lld 2 has taken a fork; fork = %d\n", (get_current_time()-data->start_time), data->fork_left);
			printf("%lld 2 has taken a fork; fork = %d\n", (get_current_time()-data->start_time),  data->fork_right);
			printf("%lld 2 is eating\n", (get_current_time()-data->start_time));
			sleep(1);
			data->fork_left = 1;
			data->fork_right = 1;
			printf("%lld 2 is sleeping\n", (get_current_time()-data->start_time));
			pthread_mutex_unlock(data->left_fork);	
			pthread_mutex_unlock(data->right_fork);	
			//printf("%d data->my_number_of_eat_times++:%d\n", data->p_id, data->my_number_of_eat_times);
			
            sleep(1);
		}
		i++;
	}
	return(0);
}




int main()
{   
    t_data data;

    pthread_t thread1;
    pthread_t thread2;
	
	
	
	pthread_mutex_init(&left_fork, NULL);
	pthread_mutex_init(&right_fork, NULL);

	data.left_fork = &left_fork;
	data.right_fork = &right_fork;

    data.fork_left = 1;
	data.fork_right = 1;

	pthread_create(&thread1, NULL, routine, &data);
    pthread_create(&thread2, NULL, routine2, &data);




	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_mutex_destroy(&left_fork);
	pthread_mutex_destroy(&right_fork);


	// int j = 0;
	// while (j < data->num_of_datasophers)
	// {	
	// 	pthread_join(threads[j], NULL);
	// 	j++;
	// }	
	// pthread_join(check_status, NULL);

	sleep(3);
	 return(0);
}
