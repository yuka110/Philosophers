/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   eat.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 18:53:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/27 20:57:03 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"


int	ft_mealtime(t_philo *pdata)
{
	long	eat;

	eat = 0;
	if (ft_printmsg(pdata, "is eating", 0))
		return (1);
	while (1)
	{
		if (ft_selfcheck(pdata))
			return (1);
		usleep(500);
		eat += 500;
		if (eat >= pdata->data->time_eat)
			break ;
		// stuck here
		// ft_printmsg(pdata, "loooooooooooooooooop", 0);
	}
	printf ("GOOD\n");
	return (0);
}

int	ft_eating(t_philo *pdata)
{
	if (ft_selfcheck(pdata))
		return (1);
	if (pdata->eatcnt || (!(pdata->id % 2)))
		usleep (300 * pdata->data->pnum);
	if (!pdata || ft_selfcheck(pdata))
		return (1);
	pthread_mutex_lock(&pdata->data->chopstick[pdata->r_chop]);
	if (ft_printmsg(pdata, "has taken a fork", 0) || !pdata || ft_selfcheck(pdata))
	{
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
		return (1);
	}
	pthread_mutex_lock(&pdata->data->chopstick[pdata->l_chop]);
	if (ft_mealtime(pdata))
	{
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->l_chop]);
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
		return (1);
	}
	pthread_mutex_lock(&pdata->plock);
	pdata->last_eat = ft_gettime(pdata->data);
	pdata->eatcnt++;
	pthread_mutex_unlock(&pdata->plock);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->l_chop]);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
	return (0);
}
