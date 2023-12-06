/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_action.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 18:53:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/06 20:51:11 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_selfcheck(t_philo *pdata, int eat)
{
	pthread_mutex_lock(&pdata->data->deadlock);
	if (pdata->data->dead)
	{
		pthread_mutex_unlock(&pdata->data->deadlock);
		return (1);
	}
	pthread_mutex_lock(&pdata->plock);
	if (!eat && ft_gettime(pdata->data) > pdata->last_eat + pdata->data->time_die)
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
	if (!pdata || ft_selfcheck(pdata, 0))
		return (1);
	pthread_mutex_lock(&pdata->plock);
	pdata->eatcnt++;
	pdata->last_eat = ft_gettime(pdata->data);
	pthread_mutex_unlock(&pdata->plock);
	if (ft_printmsg(pdata, "is eating", 0))
		return (1);
	while (1)
	{
		usleep(500);
		eat += 590;
		if (eat >= pdata->data->time_eat)
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

	if (data->pnum < 5)
		return (0);
	pthread_mutex_lock(&pdata->plock);
	if (!pdata->eatcnt)
	{
		pthread_mutex_unlock(&pdata->plock);
		return (0);
	}
	cycle = data->time_eat;
	if (data->pnum % 2)
		cycle = cycle * 2;
	time_left = data->time_die - (ft_gettime(data) - pdata->last_eat);
	pthread_mutex_unlock(&pdata->plock);
	while (time_left < cycle)
	{
		usleep (500);
		time_left += 550;
		if (ft_selfcheck(pdata, 0))
			return (1);
	}
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
	pthread_mutex_lock(&pdata->plock);
	pthread_mutex_lock(&pdata->data->dlock);
	if (pdata->eatcnt == pdata->data->mealnum)
		pdata->data->finished++;
	pthread_mutex_unlock(&pdata->data->dlock);
	pthread_mutex_unlock(&pdata->plock);
	return (0);
}

int	ft_sleeping(t_philo *pdata)
{
	long	sleep;

	sleep = 0;
	if (!pdata || ft_selfcheck(pdata, 0))
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
