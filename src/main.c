/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:41:05 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/12 21:05:51 by yitoh         ########   odam.nl         */
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

void	ft_threadjoin(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->pnum)
	{
		if (pthread_join(data->philo[i], NULL))
			return ;
		i++;
	}
	pthread_join(data->monitor, NULL);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;

	i = 0;
	data = ft_parsing(argc, argv);
	if (!data)
		return (0);
	pthread_mutex_lock(&data->start);
	while (i < data->pnum)
	{
		if (pthread_create(&data->philo[i], NULL, ft_routine, &data->pdata[i]))
			return (ft_cleanthread(data, i), 0);
		i++;
	}
	if (pthread_create(&data->monitor, NULL, ft_monitor, data))
		return (ft_cleanthread(data, i - 1), 0);
	data->s_time = ft_gettime(data);
	pthread_mutex_unlock(&data->start);
	ft_threadjoin(data);
	ft_cleanup(data, data->pnum);
	return (0);
}
