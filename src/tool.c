/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tool.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 20:22:06 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/23 17:43:40 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;
	size_t	i;

	i = 0;
	p = malloc(count * size);
	if (p == NULL)
		return (NULL);
	memset(p, '\0', count * size);
	return (p);
}

int	ft_atoiplus(const char *str)
{
	int	i;
	int	num;

	i = 0;
	num = 0;
	while (str[i] == ' ' || str[i] == '\n'
		|| str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		++i;
	if (str[i] == '0' || str[i] == '+')
		++i;
	if ('0' > str[i] || str[i] > '9')
		return (-1);
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		num = 10 * num + (str[i] - 48);
		++i;
	}
	if (num > 2147483647)
		return (-1);
	return (num);
}

void	ft_freedata(t_data *data)
{
	if (data->philo)
		free (data->philo);
	if (data->pdata)
		free (data->pdata);
	if (data->chopstick)
		free (data->chopstick);
	// free(data);
}

void	ft_nophilo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->pnum)
	{
		pthread_mutex_lock(&data->pdata[i].plock);
		pthread_mutex_unlock(&data->pdata[i].plock);
		pthread_mutex_destroy(&data->pdata[i].plock);
		i++;
	}
}

void	ft_cleanup(t_data *data)
{
	int	i;

	i = 0;
	// pthread_mutex_lock(&data->dlock);
	ft_nophilo(data);
	pthread_mutex_lock(&data->writing);
	pthread_mutex_unlock(&data->writing);
	pthread_mutex_destroy(&data->writing);
	pthread_mutex_lock(&data->start);
	pthread_mutex_unlock(&data->start);
	pthread_mutex_destroy(&data->start);
	while (i < data->pnum)
	{
		pthread_mutex_lock(&data->chopstick[i]);
		pthread_mutex_unlock(&data->chopstick[i]);
		pthread_mutex_destroy(&data->chopstick[i]);
		i++;
	}
	ft_freedata(data);
	// pthread_mutex_unlock(&data->dlock);
}

long	ft_gettime(t_data *data)
{
	struct timeval	time;
	long			microsec;

	if (gettimeofday(&time, NULL) < 0)
		return (-1);
	microsec = time.tv_sec * 1000000 + time.tv_usec - data->s_time;
	return (microsec);
}

int	ft_printmsg(t_philo *pdata, char *msg, int eat)
{
	unsigned long	time;

	if (ft_selfcheck(pdata))
		return (1);
	pthread_mutex_lock(&pdata->data->writing);
	time = ft_gettime(pdata->data) / 1000;
	printf ("%ld %d ", time, pdata->id);
	printf ("%s\n", msg);
	if (eat == 1)
		pdata->last_eat = time;
	pthread_mutex_unlock(&pdata->data->writing);
	return (0);
}
