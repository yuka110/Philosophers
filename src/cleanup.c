/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/03 18:00:10 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/30 20:01:55 by yitoh         ########   odam.nl         */
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

void	ft_cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->pnum)
	{
		pthread_mutex_destroy(&data->pdata[i].plock);
		pthread_mutex_destroy(&data->chopstick[i]);
		i++;
	}
	pthread_mutex_destroy(&data->writing);
	pthread_mutex_destroy(&data->start);
	pthread_mutex_destroy(&data->deadlock);
	pthread_mutex_destroy(&data->dlock);
	ft_freedata(data);
}
