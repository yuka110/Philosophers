/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_action.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 18:53:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/12 21:17:30 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_hypersleep(t_philo *pdata, long act_time, long start, int eat)
{
	int	endtime;

	endtime = ft_gettime(pdata->data) + act_time - start;
	while (ft_gettime(pdata->data) < endtime)
	{
		if (ft_selfcheck(pdata, eat))
			return (1);
		usleep(300);
	}
	return (0);
}

int	ft_mealtime(t_philo *pdata)
{
	long	start;

	if (ft_selfcheck(pdata, 0))
		return (1);
	if (ft_printmsg(pdata, "is eating"))
		return (1);
	start = ft_gettime(pdata->data);
	pthread_mutex_lock(&pdata->l_eatlock);
	pdata->last_eat = ft_gettime(pdata->data);
	pthread_mutex_unlock(&pdata->l_eatlock);
	pthread_mutex_lock(&pdata->cntlock);
	pdata->eatcnt++;
	pthread_mutex_unlock(&pdata->cntlock);
	if (pdata->data->mealnum > -1 && pdata->eatcnt == pdata->data->mealnum)
	{
		pthread_mutex_lock(&pdata->data->dlock);
		pdata->data->finished++;
		pthread_mutex_unlock(&pdata->data->dlock);
	}
	start = ft_gettime(pdata->data) - start;
	if (ft_hypersleep(pdata, pdata->data->time_eat, start, 1))
		return (1);
	return (0);
}

int	ft_starving(t_data *data, t_philo *pdata)
{
	long	waittime;
	long	start;

	if (data->pnum < 3 || !pdata->eatcnt)
		return (0);
	start = ft_gettime(pdata->data);
	if (data->time_die - (start - pdata->last_eat) < data->time_eat)
		return (0);
	waittime = data->time_eat / 2;
	if (data->pnum % 2)
		waittime = waittime * 2;
	if (ft_hypersleep(pdata, waittime, 0, 0))
		return (1);
	return (0);
}

int	ft_eating(t_philo *pdata)
{
	if (ft_starving(pdata->data, pdata))
		return (1);
	pthread_mutex_lock(&pdata->data->chopstick[pdata->r_chop]);
	if (ft_selfcheck(pdata, 0) || ft_printmsg(pdata, "has taken a fork"))
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
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->l_chop]);
	pthread_mutex_unlock(&pdata->data->chopstick[pdata->r_chop]);
	return (0);
}

int	ft_sleeping(t_philo *pdata)
{
	long	sleep;

	sleep = 0;
	if (ft_selfcheck(pdata, 1) || ft_printmsg(pdata, "is sleeping"))
		return (1);
	if (ft_hypersleep(pdata, pdata->data->time_sleep, 0, 0))
		return (1);
	return (0);
}
