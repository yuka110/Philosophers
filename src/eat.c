/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   eat.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 18:53:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/22 19:28:54 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"


int	ft_mealtime(t_philo *pdata)
{
	long	eat;

	eat = 0;
	if (ft_printmsg(pdata, "is eating", 0))
		return (1);
	while (!pdata->data->dead && eat < pdata->data->time_eat )
	{
		if (ft_selfcheck(pdata))
			return (1);
		usleep(500);
		eat += 500;
	}
	return (0);
}

void	ft_eating(t_philo *pdata)
{
	if (!(pdata->id % 2) && !ft_selfcheck(pdata))
		usleep (1000 * pdata->data->pnum);
	if (pdata->id == pdata->data->pnum - 1)
	{
		pthread_mutex_lock(&pdata->data->chopstick[0]);
		if (ft_printmsg(pdata, "last has taken a fork", 0))
			return ;
		pthread_mutex_lock(&pdata->data->chopstick[pdata->id]);
		if (ft_mealtime(pdata))
			return ;
		pthread_mutex_lock(&pdata->plock);
		pdata->last_eat = ft_gettime(pdata->data);
		pdata->eatcnt++;
		pthread_mutex_unlock(&pdata->plock);
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->id]);
		pthread_mutex_unlock(&pdata->data->chopstick[0]);
	}
	else 
	{
		pthread_mutex_lock(&pdata->data->chopstick[pdata->id]);
		if (ft_printmsg(pdata, "has taken a fork", 0))
			return ;
		pthread_mutex_lock(&pdata->data->chopstick[pdata->id + 1]);
		if (ft_mealtime(pdata))
			return ;
		pthread_mutex_lock(&pdata->plock);
		pdata->last_eat = ft_gettime(pdata->data);
		pdata->eatcnt++;
		pthread_mutex_unlock(&pdata->plock);
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->id + 1]);
		pthread_mutex_unlock(&pdata->data->chopstick[pdata->id]);
	}

}
