/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/03 18:00:10 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/12 20:57:01 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	ft_freedata(t_data *data)
{
	if (data->philo)
		free (data->philo);
	if (data->pdata)
		free (data->pdata);
	if (data->chopstick)
		free (data->chopstick);
	free(data);
}

void	ft_cleanthread(t_data *data, int failed)
{
	pthread_mutex_unlock(&data->start);
	ft_cleanup(data, failed);
}

void	ft_cleanup(t_data *data, int i)
{
	if (i < 0)
		i = data->pnum - 1;
	while (i >= 0)
	{
		pthread_mutex_destroy(&data->pdata[i].l_eatlock);
		pthread_mutex_destroy(&data->pdata[i].cntlock);
		pthread_mutex_destroy(&data->chopstick[i]);
		i--;
	}
	pthread_mutex_destroy(&data->writing);
	pthread_mutex_destroy(&data->start);
	pthread_mutex_destroy(&data->deadlock);
	pthread_mutex_destroy(&data->dlock);
	ft_freedata(data);
}
