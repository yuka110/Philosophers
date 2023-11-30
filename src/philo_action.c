/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_action.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 18:53:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/30 20:58:34 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_selfcheck(t_philo *pdata)
{
	pthread_mutex_lock(&pdata->data->deadlock);
	if (pdata->data->dead)
	{
		pthread_mutex_unlock(&pdata->data->deadlock);
		return (1);
	}
	pthread_mutex_lock(&pdata->plock);
	if (ft_gettime(pdata->data) > pdata->last_eat + pdata->data->time_die)
	{
		pthread_mutex_unlock(&pdata->plock);
		pdata->data->dead = pdata->id + 1;
		pthread_mutex_unlock(&pdata->data->deadlock);
		return (1);
	}
	pthread_mutex_unlock(&pdata->plock);
	pthread_mutex_unlock(&pdata->data->deadlock);
	return (0);
}

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
	}
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
	pthread_mutex_lock(&pdata->data->dlock);
	if (pdata->eatcnt == pdata->data->mealnum)
		pdata->data->finished++;
	pthread_mutex_unlock(&pdata->data->dlock);
	pthread_mutex_unlock(&pdata->plock);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->l_chop]);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
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
