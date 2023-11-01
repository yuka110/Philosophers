/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tool.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 20:22:06 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/01 21:33:31 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

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
	// free(data->pnum);
	// free(data->time_die);
	// free(data->time_eat);
	// free(data->time_sleep);
	// free(data->mealnum);
	// if (data->mutex)
	// 	free (data->mutex);
	if (data->philo)
		free (data->philo);
	free(data);
}
