/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:34:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/07 18:31:58 by yitoh         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

// Headers
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <errno.h>

//Structs
typedef struct s_mutex
{
	// pthread_mutex_t	r_fork;
	// pthread_mutex_t	l_fork;
	pthread_mutex_t	lock;
	int				count;
}	t_mutex;

typedef struct s_philo
{
	pthread_mutex_t	plock;
	int				status;
	int				eatcnt;
}	t_philo;

typedef struct s_data
{
	int				pnum;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				mealnum;
	pthread_t		*philo;
	pthread_mutex_t	*chopstick;
	t_philo			*pdata;
	t_mutex			mutex;
}	t_data;

//main

//init
t_data	*ft_parsing(int ac, char **av);

// tool
void	*ft_calloc(size_t count, size_t size);
int		ft_atoiplus(const char *str);
void	ft_freedata(t_data *data);
void	ft_cleanup(t_data *data);

//test
void	*ft_test(void *arg);

#endif