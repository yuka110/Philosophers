/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tool.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 20:22:06 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/07 18:33:39 by yitoh         ########   odam.nl         */
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
	free(data);
}

void	ft_cleanup(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->mutex.lock);
	while (i < data->pnum)
	{
		pthread_mutex_destroy(&data->chopstick[i]);
		pthread_mutex_destroy(&data->pdata[i].plock);
		i++;
	}
	ft_freedata(data);
}
