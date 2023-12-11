/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:41:05 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/11 21:25:24 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_alone(t_philo *pdata, int pnum)
{
	if (pnum > 1)
		return (0);
	pthread_mutex_lock(&pdata->data->chopstick[pdata->r_chop]);
	if (ft_selfcheck(pdata, 0) || ft_printmsg(pdata, "has taken a fork"))
	{
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
		return (1);
	}
	while (!ft_selfcheck(pdata, 0))
		usleep(300);
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
	if (ft_printmsg(pdata, "is thinking"))
		return (NULL);
	if (!(pdata->id % 2))
		ft_hypersleep(pdata, (50 * pdata->data->pnum), 0, 0);
	while (!ft_selfcheck(pdata, 0))
	{
		if (ft_eating(pdata))
			break ;
		if (ft_sleeping(pdata))
			break ;
		if (ft_printmsg(pdata, "is thinking"))
			return (NULL);
	}
	return (NULL);
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
			printf ("%ld %d died\n", ft_gettime(data) / 1000, data->pdata[i].id + 1);
			pthread_mutex_unlock(&data->writing);
			return (1);
		}
		pthread_mutex_unlock(&data->pdata[i].l_eatlock);
		i++;
	}
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
		pthread_mutex_lock(&data->deadlock);
		if (data->dead)
		{
			//lock order inversion here
			pthread_mutex_lock(&data->writing);
			printf ("%ld %d died\n", ft_gettime(data) / 1000, data->dead);
			pthread_mutex_unlock(&data->writing);
			pthread_mutex_unlock(&data->deadlock);
			break ;
		}
		pthread_mutex_lock(&data->dlock);
		if (data->finished == data->pnum)
		{
			pthread_mutex_unlock(&data->dlock);
			data->dead = 1;
			pthread_mutex_unlock(&data->deadlock);
			break ;
		}
		pthread_mutex_unlock(&data->dlock);
		pthread_mutex_unlock(&data->deadlock);
		if (ft_monitorphilo(data))
			break ;
		usleep(500);
	}
	return (0);
}

//protect thread
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
