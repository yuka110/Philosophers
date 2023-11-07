/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:41:05 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/07 18:29:56 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"


// shared locks for forks (create forks for the same amount as philo)
// philo struct keeps track of the last time etc
// put them in the array in data?
// locks for each philo to check their status

void	ft_eating(t_data *data)
{
	pthread_mutex_lock(&data->mutex.lock);
	printf("Philosopher is eating\n");
	// usleep(5);
	pthread_mutex_unlock(&data->mutex.lock);
}

void	*ft_routine(void *arg)
{
	ft_eating((t_data *)arg);
	return (NULL);
}

int main(int argc, char **argv)
{
	t_data	*data;
	int		id;

	id = 0;
	if (! (argc == 5 || argc == 6))
		return (0);
	data = ft_parsing(argc, argv);
	if (!data)
		return (0);
	// pthread_mutex_init(&data->mutex.lock, NULL);
	while (id < data->pnum)
	{
		// pthread_create(&data->philo[p], NULL, ft_test, &data->mutex);
		pthread_create(&data->philo[id], NULL, ft_routine, &data);
		printf("philo%d is created\n", id);
		id++;

	}
	id = 0;
	while (id < data->pnum)
	{
		pthread_join(data->philo[id], NULL);
		id++;
	}
	printf("final count is %d\n", data->mutex.count);

	ft_cleanup(data);
	return (0);
}
