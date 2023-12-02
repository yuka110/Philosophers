/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:41:05 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/02 21:21:19 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_alone(t_philo *pdata, int pnum)
{
	if (pnum > 1)
		return (0);
	pthread_mutex_lock(&pdata->data->chopstick[pdata->r_chop]);
	if (ft_selfcheck(pdata, 0) || ft_printmsg(pdata, "has taken a fork", 0))
	{
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
		return (1);
	}
	while (!ft_selfcheck(pdata, 0))
		usleep(500);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
	return (1);
}

void	*ft_routine(void *arg)
{
	t_philo	*pdata;

	pdata = (t_philo *)arg;
	pthread_mutex_lock(&pdata->data->start);
	pthread_mutex_unlock(&pdata->data->start);
	if (ft_alone(pdata, pdata->data->pnum))
		return (NULL);
	while (!ft_selfcheck(pdata, 0))
	{
		if (ft_printmsg(pdata, "is thinking", 0))
			return (NULL);
		if (ft_eating(pdata))
			break ;
		if (ft_sleeping(pdata))
			break ;
	}
	return (NULL);
}

void	*ft_monitor(void *arg)
{
	t_data	*data;
	int		i;

	i = 0;
	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->deadlock);
		if (data->dead)
		{
			pthread_mutex_unlock(&data->deadlock);
			pthread_mutex_lock(&data->writing);
			printf ("%ld %d died\n", ft_gettime(data) / 1000, data->dead);
			pthread_mutex_unlock(&data->writing);
			break ;
		}
		pthread_mutex_lock(&data->dlock);
		if (data->finished == data->pnum)
		{
			data->dead = 1;
			pthread_mutex_unlock(&data->dlock);
			pthread_mutex_unlock(&data->deadlock);
			break ;
		}
		pthread_mutex_unlock(&data->dlock);
		pthread_mutex_unlock(&data->deadlock);
	}
	return (0);
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
	pthread_mutex_lock(&data->start);
	while (id < data->pnum)
	{
		pthread_create(&data->philo[id], NULL, ft_routine, &data->pdata[id]);
		printf("\e[1;36mPhilo%d is created: right %d left %d\e[0;00m\n", id, data->pdata[id].r_chop, data->pdata[id].l_chop);
		id++;
	}
	pthread_create(&data->monitor, NULL, ft_monitor, data);
	data->s_time = ft_gettime(data);
	pthread_mutex_unlock(&data->start);

	id = 0;
	while (id < data->pnum)
	{
		pthread_join(data->philo[id], NULL);
		id++;
	}
	pthread_join(data->monitor, NULL);
	ft_cleanup(data);
	return (0);
}
