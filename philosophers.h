/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: yitoh <yitoh@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 19:34:50 by yitoh         #+#    #+#                 */
/*   Updated: 2023/11/26 19:09:45 by yitoh         ########   odam.nl         */
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
# include <sys/time.h>

//Structs

typedef struct	s_data	t_data;

typedef struct s_philo
{
	int				id;
	pthread_mutex_t	plock;
	// int				status;
	long			last_eat;
	int				eatcnt;
	int				r_chop;
	int				l_chop;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				pnum;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	int				mealnum;
	int				dead;
	int				finished;
	long			s_time;
	pthread_mutex_t	dlock;
	// pthread_mutex_t	eatlock;
	pthread_mutex_t	deadlock;
	pthread_t		*philo;
	pthread_mutex_t	*chopstick;
	pthread_mutex_t	writing;
	pthread_mutex_t	start;
	t_philo			*pdata;
}	t_data;

//main
int		ft_selfcheck(t_philo *pdata);

//init
t_data	*ft_parsing(int ac, char **av);

//eat
int		ft_eating(t_philo *pdata);

// tool
void	*ft_calloc(size_t count, size_t size);
int		ft_atoiplus(const char *str);
void	ft_freedata(t_data *data);
void	ft_frechop(t_data *data);
void	ft_cleanup(t_data *data);
long	ft_gettime(t_data *data);
int		ft_printmsg(t_philo *philo, char *msg, int eat);

//test
// void	*ft_test(void *arg);

#endif