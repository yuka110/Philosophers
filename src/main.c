/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:41:05 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/28 21:37:13 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"


// start lock so that everyone starts at the same time
// switch chopstick 1 and 2 for even/odd number philo
// if only one philo, then she just needs to die because there are no two chopsticks
// each philo also check their own status

/*
./philo 3 300 400 500

0 0 is thinking
0 0 has taken a fork
0 1 is thinking
0 2 is thinking
0 2 has taken a fork

T2 is trying to eat while T0 is dying

*/

// int	ft_dying(t_philo *pdata)
// {
// 	pthread_mutex_lock(&pdata->data->deadlock);
// 	pdata->data->dead = pdata->id + 1;
// 	pthread_mutex_unlock(&pdata->data->deadlock);
// 	ft_printmsg(pdata, "died", 0);
// 	return (1);
// }

int	ft_selfcheck(t_philo *pdata)
{
	pthread_mutex_lock(&pdata->data->deadlock);
	if (pdata->data->dead)
	{
		pthread_mutex_unlock(&pdata->data->deadlock);
		return (1);
	}
	pthread_mutex_lock(&pdata->data->pdata[pdata->id].plock);
	if (ft_gettime(pdata->data) > pdata->last_eat + pdata->data->time_die)
	{
		pthread_mutex_unlock(&pdata->data->pdata[pdata->id].plock);
		pdata->data->dead = pdata->id + 1;
		pthread_mutex_unlock(&pdata->data->deadlock);
		return (1);
	}
	pthread_mutex_unlock(&pdata->data->pdata[pdata->id].plock);
	pthread_mutex_unlock(&pdata->data->deadlock);
	return (0);
}

int	ft_sleeping(t_philo *pdata)
{
	long	sleep;

	sleep = 0;
	if (!pdata || ft_selfcheck(pdata))
		return (1);
	if (ft_printmsg(pdata, "is sleeping", 0))
		return (1);
	while (sleep < pdata->data->time_sleep)
	{
		if (ft_selfcheck(pdata))
			return (1);
		usleep(500);
		sleep += 500;
	}
	return (0);
}

void	*ft_routine(void *arg)
{
	t_philo	*pdata;

	pdata = (t_philo *)arg;
	pthread_mutex_lock(&pdata->data->start);
	pthread_mutex_unlock(&pdata->data->start);
	while (!ft_selfcheck(pdata))
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
	//not even initialized??
	pthread_mutex_lock(&data->deadlock);
	pthread_mutex_unlock(&data->deadlock);

	while (1)
	{
		pthread_mutex_lock(&data->deadlock);
		if (data->dead)
		{
			pthread_mutex_unlock(&data->deadlock);
			break ;
		}
		pthread_mutex_unlock(&data->deadlock);
	}
	pthread_mutex_lock(&data->writing);
	printf ("%ld %d died\n", ft_gettime(data) / 1000, data->dead - 1);
	pthread_mutex_unlock(&data->writing);
	ft_cleanup(data);
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
	pthread_create(&data->monitor, NULL, ft_monitor, &data);
	data->s_time = ft_gettime(data);
	pthread_mutex_unlock(&data->start);

	id = 0;
	while (id < data->pnum)
	{
		pthread_join(data->philo[id], NULL);
		id++;
	}
	pthread_join(data->monitor, NULL);
	usleep (1000000);
	ft_cleanup(data);
	// pthread_mutex_destroy(&data->deadlock);
	if (data->philo)
		free (data->philo);
	free (data);
	return (0);
}
