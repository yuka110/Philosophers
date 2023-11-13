/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:41:05 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/13 19:41:19 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"


// start lock so that everyone starts at the same time
// switch chopstick 1 and 2 for even/odd number philo
// if only one philo, then she just needs to die because there are no two chopsticks
// each philo also check their own status

void	ft_eating(t_philo *pdata)
{
	if (pdata->id % 2)
		usleep (500);
	pthread_mutex_lock(&pdata->data->chopstick[pdata->id]);
	pthread_mutex_lock(&pdata->data->chopstick[pdata->id + 1]);
	pthread_mutex_lock(&pdata->data->pdata[pdata->id].plock);
	ft_printmsg(pdata, "is eating", 1);
	usleep (pdata->data->time_eat);
	pdata->last_eat = ft_gettime(pdata->data);
	pdata->eatcnt++;

	pthread_mutex_unlock(&pdata->plock);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->id + 1]);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->id]);

}

void	ft_sleeping(t_philo *pdata)
{
	ft_printmsg(pdata, "is sleeping", 0);
	usleep(pdata->data->time_sleep);
}

void	*ft_routine(void *arg)
{
	t_philo	*pdata;

	pdata = (t_philo *)arg;
	pthread_mutex_lock(&pdata->data->start);
	pthread_mutex_unlock(&pdata->data->start);
	ft_printmsg(pdata, "is thinking", 0);
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
	id = 0;
	while (id < data->pnum)
	{
		pthread_join(data->philo[id], NULL);
		id++;
	}
	ft_cleanup(data);
	return (0);
}
