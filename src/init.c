/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/07 18:12:40 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/13 19:26:04 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	ft_initmutex(t_data *data)
{
	int	i;

	i = 0;
	data->chopstick = (pthread_mutex_t *)ft_calloc(sizeof(pthread_mutex_t), data->pnum);
	if (!data->chopstick)
	{
		ft_freedata(data);
		return ;
	}
	pthread_mutex_init(&data->writing, NULL);
	pthread_mutex_init(&data->start, NULL);
	while (i < data->pnum)
	{
		pthread_mutex_init(&data->chopstick[i], NULL);
		pthread_mutex_init(&data->pdata[i].plock, NULL);
		data->pdata[i].id = i;
		data->pdata[i].data = data;
		i++;
	}

}

t_data	*ft_parsing(int ac, char **av)
{
	t_data	*data;

	data = (t_data *)ft_calloc(1, sizeof(t_data));
	if (!data)
		return (NULL);
	data->pnum = ft_atoiplus(av[1]);
	data->time_die = ft_atoiplus(av[2]) * 1000;
	data->time_eat = ft_atoiplus(av[3]) * 1000;
	data->time_sleep = ft_atoiplus(av[4]) * 1000;
	if (ac == 6)
		data->mealnum = ft_atoiplus(av[5]);
	data->philo = (pthread_t *)ft_calloc(sizeof(pthread_t), data->pnum);
	data->pdata = (t_philo *)ft_calloc(sizeof(t_philo), data->pnum);
	if (data->pnum < 1 || data->time_die < 1 || data->time_eat < 1 ||
		data->time_sleep < 1 || (ac == 6 && data->mealnum < 1) || !data->philo)
	{
		ft_freedata(data);
		return (NULL);
	}
	ft_initmutex(data);
	printf("\e[1;35mphilo num = %d\e[0;00m\n", data->pnum);
	printf("\e[1;35mtime die = %ld\e[0;00m\n", data->time_die);
	printf("\e[1;35mtime eat = %ld\e[0;00m\n", data->time_eat);
	printf("\e[1;35mtime sleep = %ld\e[0;00m\n", data->time_sleep);
	printf("\e[1;35mmeal num= %d\e[0;00m\n", data->mealnum);
	return (data);
}
