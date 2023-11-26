/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   eat.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 18:53:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/26 11:24:03 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"


int	ft_mealtime(t_philo *pdata)
{
	long	eat;

	eat = 0;
	if (ft_printmsg(pdata, "is eating", 0))
		return (1);
	while (!pdata->data->dead)
	{
		if (ft_selfcheck(pdata))
			return (1);
		usleep(500);
		eat += 500;
		pthread_mutex_lock(&pdata->data->dlock);
		if (eat >= pdata->data->time_eat)
			break ;
		pthread_mutex_unlock(&pdata->data->dlock);
		// stuck here
		// ft_printmsg(pdata, "loooooooooooooooooop", 0);
	}
	pthread_mutex_unlock(&pdata->data->dlock);
	printf ("GOOD\n");
	return (0);
}

void	ft_eating(t_philo *pdata)
{
	if (pdata->eatcnt || (!(pdata->id % 2) && !ft_selfcheck(pdata)))
		usleep (300 * pdata->data->pnum);
	pthread_mutex_lock(&pdata->data->chopstick[pdata->r_chop]);
	if (ft_printmsg(pdata, "has taken a fork", 0))
		return ;
	pthread_mutex_lock(&pdata->data->chopstick[pdata->l_chop]);
	if (ft_mealtime(pdata))
	{
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->l_chop]);
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
		return ;
	}
	ft_printmsg(pdata, "ahhhh", 0);
	pthread_mutex_lock(&pdata->plock);
	pdata->last_eat = ft_gettime(pdata->data);
	pdata->eatcnt++;
	pthread_mutex_unlock(&pdata->plock);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->l_chop]);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
}
