/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_action.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 18:53:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/10 18:03:12 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
// make ft_hypersleep to see if the addtime makes difference


int	ft_selfcheck(t_philo *pdata, int eat)
{
	pthread_mutex_lock(&pdata->data->deadlock);
	if (pdata->data->dead)
	{
		pthread_mutex_unlock(&pdata->data->deadlock);
		return (1);
	}
	pthread_mutex_unlock(&pdata->data->deadlock);
	if (!eat)
	{
		pthread_mutex_lock(&pdata->l_eatlock);
		if (ft_gettime(pdata->data) > pdata->last_eat + pdata->data->time_die)
		{
			pthread_mutex_unlock(&pdata->l_eatlock);
			pthread_mutex_lock(&pdata->data->deadlock);
			pdata->data->dead = pdata->id + 1;
			pthread_mutex_unlock(&pdata->data->deadlock);
			return (1);
		}
		pthread_mutex_unlock(&pdata->l_eatlock);
	}
	return (0);
}

int	ft_hypersleep(t_philo *pdata, long	act_time, long start, int eat)
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

/*
func sleep_in_smaller_chunks(timetosleep)
{
	check if simulation is still ok
	endtime = currenttime + timetosleep
	while(currenttime < endtime)
	{
		check if simulation is still ok
		sleep a small chunk
	}
}
*/

int	ft_mealtime(t_philo *pdata)
{
	long	start;

	if (!pdata || ft_selfcheck(pdata, 0))
		return (1);
	if (ft_printmsg(pdata, "is eating", 0))
		return (1);
	pthread_mutex_lock(&pdata->l_eatlock);
	pdata->last_eat = ft_gettime(pdata->data);
	start = ft_gettime(pdata->data);
	pthread_mutex_unlock(&pdata->l_eatlock);
	pthread_mutex_lock(&pdata->cntlock);
	pdata->eatcnt++;
	if (pdata->data->mealnum > -1 && pdata->eatcnt == pdata->data->mealnum)
	{
		pthread_mutex_lock(&pdata->data->dlock);
		pdata->data->finished++;
		pthread_mutex_unlock(&pdata->data->dlock);
	}
	pthread_mutex_unlock(&pdata->cntlock);
	start = ft_gettime(pdata->data) - start;
	if (ft_hypersleep(pdata, pdata->data->time_eat, start, 1))
		return (1);
	return (0);
}

int	ft_starving(t_data *data, t_philo *pdata)
{
	long	waittime;
	long	start;

	if (data->pnum < 3)
		return (0);
	start = ft_gettime(pdata->data);
	pthread_mutex_lock(&pdata->cntlock);
	if (!pdata->eatcnt)
	{
		pthread_mutex_unlock(&pdata->cntlock);
		return (0);
	}
	pthread_mutex_unlock(&pdata->cntlock);
	waittime = data->time_eat;
	if (!(data->pnum % 2))
		waittime = waittime * 2;
	start = ft_gettime(pdata->data) - start;
	if (ft_hypersleep(pdata, waittime, start, 0))
		return (1);
	return (0);
}


int	ft_eating(t_philo *pdata)
{
	if (!pdata || ft_selfcheck(pdata, 0))
		return (1);
	if (ft_starving(pdata->data, pdata))
		return (1);
	pthread_mutex_lock(&pdata->data->chopstick[pdata->r_chop]);
	if (!pdata || ft_selfcheck(pdata, 0) || ft_printmsg(pdata, "has taken a fork", 0))
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
	// long	start;

	sleep = 0;
	if (!pdata || ft_selfcheck(pdata, 1))
		return (1);
	// start = ft_gettime(pdata->data);
	if (ft_printmsg(pdata, "is sleeping", 0))
		return (1);
	// start = ft_gettime(pdata->data) - start;
	if (ft_hypersleep(pdata, pdata->data->time_sleep, 0, 0))
		return (1);
	// while (sleep < pdata->data->time_sleep)
	// {
	// 	usleep(500);
	// 	sleep += 590;
	// 	if (ft_selfcheck(pdata, 0))
	// 		return (1);
	// }
	return (0);
}
