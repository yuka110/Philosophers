/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/07 18:12:40 by yitoh         #+#    #+#                 */
/*   Updated: 2023/12/12 20:32:42 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	ft_initphilo(t_data *data)
{
	int	i;
	int	tmp;

	i = 0;
	while (i < data->pnum)
	{
		pthread_mutex_init(&data->chopstick[i], NULL);
		pthread_mutex_init(&data->pdata[i].cntlock, NULL);
		pthread_mutex_init(&data->pdata[i].l_eatlock, NULL);
		data->pdata[i].id = i;
		data->pdata[i].data = data;
		data->pdata[i].r_chop = i;
		data->pdata[i].l_chop = i + 1;
		if (i == data->pnum - 1)
			data->pdata[i].l_chop = 0;
		if (!(i % 2))
		{
			tmp = data->pdata[i].r_chop;
			data->pdata[i].r_chop = data->pdata[i].l_chop;
			data->pdata[i].l_chop = tmp;
		}
		i++;
	}
}

void	ft_initdata(t_data *data)
{
	data->philo = (pthread_t *)ft_calloc(sizeof(pthread_t), data->pnum);
	data->pdata = (t_philo *)ft_calloc(sizeof(t_philo), data->pnum);
	data->chopstick = ft_calloc(sizeof(pthread_mutex_t), data->pnum);
	if (!data->philo || !data->pdata || !data->chopstick)
	{
		ft_freedata(data);
		return ;
	}
	pthread_mutex_init(&data->deadlock, NULL);
	pthread_mutex_init(&data->writing, NULL);
	pthread_mutex_init(&data->start, NULL);
	pthread_mutex_init(&data->dlock, NULL);
}

t_data	*ft_parsing(int ac, char **av)
{
	t_data	*data;

	if (!(ac == 5 || ac == 6))
		return (NULL);
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (NULL);
	data->pnum = ft_atoiplus(av[1]);
	data->time_die = ft_atoiplus(av[2]) * 1000;
	data->time_eat = ft_atoiplus(av[3]) * 1000;
	data->time_sleep = ft_atoiplus(av[4]) * 1000;
	data->mealnum = -1;
	if (ac == 6)
		data->mealnum = ft_atoiplus(av[5]);
	if (data->pnum < 1 || data->pnum > 60000 || data->time_die < 1
		|| data->time_eat < 1 || data->time_sleep < 1
		|| (ac == 6 && data->mealnum < 1))
	{
		ft_freedata(data);
		return (NULL);
	}
	ft_initdata(data);
	ft_initphilo(data);
	return (data);
}
