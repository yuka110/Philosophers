/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:41:05 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/14 21:54:23 by yitoh         ########   odam.nl         */
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
^C

*/

int	ft_dying(t_philo *pdata)
{
	int		dead;
	t_data	*cpy;

	dead = pdata->id;
	cpy = pdata->data;
	ft_cleanup(cpy);
	printf ("%ld %d died\n", ft_gettime(cpy) / 1000, dead);
	return (1);
}

int	ft_selfcheck(t_philo *pdata)
{
	pthread_mutex_lock(&pdata->data->pdata[pdata->id].plock);
	if (ft_gettime(pdata->data) > pdata->last_eat + pdata->data->time_die)
	{
		pthread_mutex_unlock(&pdata->data->pdata[pdata->id].plock);
		return (ft_dying(pdata));
	}
	pthread_mutex_unlock(&pdata->data->pdata[pdata->id].plock);
	return (0);
}

void	ft_eating(t_philo *pdata)
{
	if (pdata->id % 2 && !ft_selfcheck(pdata))
		usleep (500);
	if (pdata->id == pdata->data->pnum - 1)
	{
		pthread_mutex_lock(&pdata->data->chopstick[pdata->id + 1]);
		if (ft_printmsg(pdata, "has taken a fork", 0))
			return ;
		pthread_mutex_lock(&pdata->data->chopstick[pdata->id]);
		pthread_mutex_lock(&pdata->data->pdata[pdata->id].plock);
		if (ft_printmsg(pdata, "is eating", 1))
			return ;
		usleep (pdata->data->time_eat);
		pdata->last_eat = ft_gettime(pdata->data);
		pdata->eatcnt++;

		pthread_mutex_unlock(&pdata->plock);
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->id]);
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->id + 1]);
	}
	else 
	{
		pthread_mutex_lock(&pdata->data->chopstick[pdata->id]);
		if (ft_printmsg(pdata, "has taken a fork", 0))
			return ;
		pthread_mutex_lock(&pdata->data->chopstick[pdata->id + 1]);
		pthread_mutex_lock(&pdata->data->pdata[pdata->id].plock);
		if (ft_printmsg(pdata, "is eating", 1))
		 	return ;
		usleep (pdata->data->time_eat);
		pdata->last_eat = ft_gettime(pdata->data);
		pdata->eatcnt++;

		pthread_mutex_unlock(&pdata->plock);
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->id + 1]);
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->id]);
	}

}

void	ft_sleeping(t_philo *pdata)
{
	long	sleep;

	sleep = 0;
	if (ft_printmsg(pdata, "is sleeping", 0))
		return ;
	while (sleep < pdata->data->time_sleep && !ft_selfcheck(pdata))
	{
		usleep(500);
		sleep += 500;
	}
}

void	*ft_routine(void *arg)
{
	t_philo	*pdata;

	pdata = (t_philo *)arg;
	pthread_mutex_lock(&pdata->data->start);
	pthread_mutex_unlock(&pdata->data->start);
	if (ft_printmsg(pdata, "is thinking", 0))
		return (NULL);
	ft_eating(pdata);
	ft_sleeping(pdata);
	return (NULL);
}

// int		ft_monitor()
// {

// }

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
		printf("\e[1;36mPhilo%d is created\e[0;00m\n", id);
		id++;
	}
	data->s_time = ft_gettime(data);
	pthread_mutex_unlock(&data->start);
	if (data)
	{
		id = 0;
		while (id < data->pnum)
		{
			pthread_join(data->philo[id], NULL);
			id++;
		}
		ft_cleanup(data);
	}
	free (data);
	return (0);
}
