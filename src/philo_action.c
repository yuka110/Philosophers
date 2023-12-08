/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_action.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 18:53:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/08 22:45:57 by yitoh         ########   odam.nl         */
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

int	ft_mealtime(t_philo *pdata)
{
	long	eating;

	if (!pdata || ft_selfcheck(pdata, 0))
		return (1);
	if (ft_printmsg(pdata, "is eating", 0))
		return (1);
	pthread_mutex_lock(&pdata->l_eatlock);
	pdata->last_eat = ft_gettime(pdata->data);
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
	eating = 0;
	while (1)
	{
		usleep(500);
		eating += 500 + (pdata->data->pnum * 20);
		if (eating >= pdata->data->time_eat)
			break ;
		if (ft_selfcheck(pdata, 1))
			return (1);
	}
	return (0);
}

int	ft_starving(t_data *data, t_philo *pdata)
{
	int		cycle;
	long	time_left;

	if (data->pnum < 3)
		return (0);
	pthread_mutex_lock(&pdata->cntlock);
	if (!pdata->eatcnt)
	{
		pthread_mutex_unlock(&pdata->cntlock);
		return (0);
	}
	pthread_mutex_unlock(&pdata->cntlock);
	cycle = data->time_eat;
	if (data->pnum % 2)
		cycle = cycle * 2;
	pthread_mutex_lock(&pdata->l_eatlock);
	time_left = ft_gettime(data) - pdata->last_eat + data->time_die;
	pthread_mutex_unlock(&pdata->l_eatlock);
	while (time_left < cycle)
	{
		usleep (500);
		time_left += 550;
		if (ft_selfcheck(pdata, 0))
			return (1);
	}
	if (data->pnum % 2 && pdata->id % 2)
		usleep (data->time_eat / 2);
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

	sleep = 0;
	if (!pdata || ft_selfcheck(pdata, 1))
		return (1);
	if (ft_printmsg(pdata, "is sleeping", 0))
		return (1);
	while (sleep < pdata->data->time_sleep)
	{
		usleep(500);
		sleep += 590;
		if (ft_selfcheck(pdata, 0))
			return (1);
	}
	return (0);
}
