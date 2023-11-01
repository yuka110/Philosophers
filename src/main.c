/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:41:05 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/01 21:35:15 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

t_data	*ft_parsing(int ac, char **av)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->pnum = ft_atoiplus(av[1]);
	data->time_die = ft_atoiplus(av[2]);
	data->time_eat = ft_atoiplus(av[3]);
	data->time_sleep = ft_atoiplus(av[4]);
	data->mealnum = 0;
	if (ac == 6)
		data->mealnum = ft_atoiplus(av[5]);
	data->philo = (pthread_t *)malloc(sizeof(pthread_t) * (data->pnum));
	if (data->pnum < 1 || data->time_die < 1 || data->time_eat < 1 ||
		data->time_sleep < 1 || (ac == 6 && data->mealnum < 1))
	{
		ft_freedata(data);
		return (NULL);
	}
	data->mutex.count = 0;
	printf("\e[1;35mphilo num = %d\e[0;00m\n", data->pnum);
	printf("\e[1;35mtime die = %d\e[0;00m\n", data->time_die);
	printf("\e[1;35mtime eat = %d\e[0;00m\n", data->time_eat);
	printf("\e[1;35mtime sleep = %d\e[0;00m\n", data->time_sleep);
	printf("\e[1;35mmeal num= %d\e[0;00m\n", data->mealnum);
	return (data);
}

void	*ft_test(void *arg)
{
	t_mutex	*mutex;
	int		i;

	i = 0;
	mutex = (t_mutex *)arg;
	while (i < 100)
	{
		pthread_mutex_lock(&mutex->lock);
		mutex->count++;
		i++;
		pthread_mutex_unlock(&mutex->lock);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	t_data	*data;
	int		p;

	p = 0;
	if (! (argc == 5 || argc == 6))
		return (0);
	data = ft_parsing(argc, argv);
	if (!data)
		return (0);
	while (p < data->pnum)
	{
		pthread_mutex_init(&data->mutex.lock, NULL);
		pthread_create(&data->philo[p], NULL, ft_test, &data->mutex);
		printf("philo%d is created\n", p);
		p++;

	}
	p = 0;
	while (p < data->pnum)
	{
		pthread_join(data->philo[p], NULL);
		p++;
	}
	printf("final count is %d\n", data->mutex.count);

	p = 0;
	while (p < data->pnum)
	{
		pthread_mutex_destroy(&data->mutex.lock);
		p++;
	}
	ft_freedata(data);
	return (0);
}
