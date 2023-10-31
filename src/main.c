/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:41:05 by yitoh         #+#    #+#                 */
/*   Updated: 2023/10/31 21:04:14 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

t_data	*ft_parsing(int ac, char **av)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->pnum = ft_atoiplus(av[1]);
	data->time_die = ft_atoiplus(av[2]);
	data->time_eat = ft_atoiplus(av[3]);
	data->time_sleep = ft_atoiplus(av[4]);
	data->mealnum = 0;
	if (ac == 6)
		data->mealnum = ft_atoiplus(av[5]);
	if (data->pnum < 1 || data->time_die < 1 || data->time_eat < 1 ||
		data->time_sleep < 1 || (ac == 6 && data->mealnum < 1))
	{
		ft_freedata(data);
		return (NULL);
	}
	printf("\e[1;35mphilo num = %d\e[0;00m\n", data->pnum);
	printf("\e[1;35mtime die = %d\e[0;00m\n", data->time_die);
	printf("\e[1;35mtime eat = %d\e[0;00m\n", data->time_eat);
	printf("\e[1;35mtime sleep = %d\e[0;00m\n", data->time_sleep);
	printf("\e[1;35mmeal num= %d\e[0;00m\n", data->mealnum);
	return (data);
}

//create 5 threads and see 5000000sec


int main(int argc, char **argv)
{
	t_data	*data;

	if (! (argc == 5 || argc == 6))
		return (0);
	data = ft_parsing(argc, argv);
	if (!data)
		return (0);
	ft_freedata(data);
	return (0);

	// pthread_t	p1;

	// pthread_create(&p1, NULL, ft_eat, &var);

}
