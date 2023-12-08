/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tool.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 20:22:06 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/08 18:50:24 by yitoh         ########   odam.nl         */
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

	if (ft_selfcheck(pdata, 0))
		return (1);
	pthread_mutex_lock(&pdata->data->writing);
	time = ft_gettime(pdata->data) / 1000;
	printf ("%ld %d ", time, pdata->id + 1);
	printf ("%s\n", msg);
	pthread_mutex_unlock(&pdata->data->writing);
	if (eat == 1)
	{
		pthread_mutex_lock(&pdata->l_eatlock);
		pdata->last_eat = time;
		pthread_mutex_unlock(&pdata->l_eatlock);
	}
	return (0);
}
