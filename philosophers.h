/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:34:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/01 21:26:30 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

// Headers
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <errno.h>

//Structs
typedef struct s_mutex
{
	pthread_mutex_t	lock;
	int				count;
}	t_mutex;

typedef struct s_data
{
	int	pnum;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	mealnum;
	pthread_t	*philo;
	t_mutex		mutex;
}	t_data;


//main
t_data	*ft_parsing(int ac, char **av);

// tool
int		ft_atoiplus(const char *str);
void	ft_freedata(t_data *data);

#endif