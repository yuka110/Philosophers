/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/12 20:27:59 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/12 21:07:17 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_selfcheck(t_philo *pdata, int eat)
{
	pthread_mutex_lock(&pdata->data->deadlock);
	if (pdata->data->dead)
	{
		pthread_mutex_unlock(&pdata->data->deadlock);
		return (1);
	}
	pthread_mutex_unlock(&pdata->data->deadlock);
	if (!eat)
	{
		if (ft_gettime(pdata->data) > pdata->last_eat + pdata->data->time_die)
		{
			pthread_mutex_lock(&pdata->data->deadlock);
			pdata->data->dead = pdata->id + 1;
			pthread_mutex_unlock(&pdata->data->deadlock);
			return (1);
		}
	}
	return (0);
}

int	ft_monitorphilo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->pnum)
	{
		pthread_mutex_lock(&data->pdata[i].l_eatlock);
		if (ft_gettime(data) > data->pdata[i].last_eat + data->time_die)
		{
			pthread_mutex_unlock(&data->pdata[i].l_eatlock);
			pthread_mutex_lock(&data->deadlock);
			data->dead = data->pdata[i].id + 1;
			pthread_mutex_unlock(&data->deadlock);
			pthread_mutex_lock(&data->writing);
			printf ("%ld ", ft_gettime(data) / 1000);
			printf ("%d died\n", data->pdata[i].id + 1);
			pthread_mutex_unlock(&data->writing);
			return (1);
		}
		pthread_mutex_unlock(&data->pdata[i].l_eatlock);
		i++;
	}
	return (0);
}

int	ft_monitorall(t_data *data)
{
	pthread_mutex_lock(&data->deadlock);
	if (data->dead)
	{
		pthread_mutex_unlock(&data->deadlock);
		pthread_mutex_lock(&data->writing);
		printf ("%ld %d died\n", ft_gettime(data) / 1000, data->dead);
		pthread_mutex_unlock(&data->writing);
		return (1);
	}
	pthread_mutex_lock(&data->dlock);
	if (data->finished == data->pnum)
	{
		pthread_mutex_unlock(&data->dlock);
		data->dead = 1;
		pthread_mutex_unlock(&data->deadlock);
		return (1);
	}
	pthread_mutex_unlock(&data->dlock);
	pthread_mutex_unlock(&data->deadlock);
	return (0);
}

void	*ft_monitor(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	pthread_mutex_lock(&data->start);
	pthread_mutex_unlock(&data->start);
	while (1)
	{
		if (ft_monitorall(data))
			break ;
		if (ft_monitorphilo(data))
			break ;
		usleep(500);
	}
	return (0);
}
