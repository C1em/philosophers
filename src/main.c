/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coremart <coremart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 20:13:12 by coremart          #+#    #+#             */
/*   Updated: 2019/12/04 09:36:58 by coremart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "philo.h"

#include <unistd.h>

bool	philos_alive(int life[], pthread_mutex_t mutex[])
{
	int	i;

	i = 0;
	while (i < NB_PHILO)
	{
		pthread_mutex_lock(&mutex[i]);
		if (life[i] <= 0)
		{
			pthread_mutex_unlock(&mutex[i]);
			return (0);
		}
		pthread_mutex_unlock(&mutex[i]);
		i++;
	}
	return (1);
}

void	*fct(void *philosopher_info)
{
	t_philosopher	info;

	info = *(t_philosopher*)philosopher_info;
	for (int i = 0; i < 9999; i++);
	pthread_mutex_lock(&info.table->mutex_time);
	info.table->time_at_start = time(NULL);
	info.time_last_step = info.table->time_at_start;
	pthread_mutex_unlock(&info.table->mutex_time);
	while (philos_alive(info.table->life, info.table->mutex_life))
	{
		pthread_mutex_lock(&info.table->mutex_time);
		if (time(NULL) - info.table->time_at_start >= TIMEOUT)
		{
			pthread_mutex_unlock(&info.table->mutex_time);
			write(1, "TIMEOUT\n", 8);
			mutex_write(&info.table->mutex_life[info.philo_nb],
						&info.table->life[info.philo_nb], 0);
			return (NULL); // timeout ????
		}
		pthread_mutex_unlock(&info.table->mutex_time);
		pthread_mutex_lock(&info.table->mutex_chopstick[info.philo_nb]);
		pthread_mutex_lock(&info.table->mutex_chopstick[get_philo(info.philo_nb + 1)]);
		pthread_mutex_lock(&info.table->mutex_state[info.philo_nb]);
		if (info.table->chopstick[info.philo_nb] == ON_TABLE
		&& info.table->chopstick[get_philo(info.philo_nb + 1)] == ON_TABLE
		&& (info.table->state[info.philo_nb] & EAT) == 0)
		{
			philo_eat(&info);
			continue ;
		}
		pthread_mutex_unlock(&info.table->mutex_chopstick[info.philo_nb]);
		pthread_mutex_unlock(&info.table->mutex_chopstick[get_philo(info.philo_nb + 1)]);
		pthread_mutex_lock(&info.table->mutex_life[info.philo_nb]);
		if (info.table->state[info.philo_nb] & EAT
		|| (info.table->state[info.philo_nb] & THINK
		&& (info.table->life[info.philo_nb] >= REST_T
		|| info.table->life[info.philo_nb] < THINK_T)))
		{
			pthread_mutex_unlock(&info.table->mutex_life[info.philo_nb]);
			philo_rest(&info);
		}
		else
			philo_think(&info);
	}
	write(1, "DEAD\n", 5);
	return (NULL);
}

void	manage_time(t_philosopher *philosophers_info)
{
	int i;

	while (1)
	{
		usleep(SECOND);
		i = 0;
		while (i < NB_PHILO)
		{
			pthread_mutex_lock(&philosophers_info->table->mutex_life[i]);
			philosophers_info->table->life[i]--;
			if (philosophers_info->table->life[i] <= 0)
			{
				pthread_mutex_unlock(&philosophers_info->table->mutex_life[i]);
				return ;
			}
			pthread_mutex_unlock(&philosophers_info->table->mutex_life[i]);
			i++;
		}
	}
}

t_philosopher	*philosphers_create(pthread_t philosophers[], t_table *table)
{
	int 			i;
	t_philosopher	*philosophers_info;

	i = 0;
	if (!(philosophers_info = (t_philosopher*)malloc(sizeof(t_philosopher) * NB_PHILO)))
		exit(1);
	while (i < NB_PHILO)
	{
		philosophers_info[i].philo_nb = i;
		philosophers_info[i].table = (t_table*)table;
		if (pthread_create(&philosophers[i], NULL, fct, (void*)&philosophers_info[i]))
			exit(1);
		i++;
	}
	return (philosophers_info);
}

void	philosophers_del(pthread_t philosophers[], t_philosopher philosophers_info[])
{
	int	i;

	manage_time(philosophers_info);
	i = 0;
	while (i < NB_PHILO)
	{
		pthread_join(philosophers[i], NULL);
		pthread_mutex_destroy(&philosophers_info->table->mutex_chopstick[i]);
		pthread_mutex_destroy(&philosophers_info->table->mutex_state[i]);
		pthread_mutex_destroy(&philosophers_info->table->mutex_life[i]);
		i++;
	}
	pthread_mutex_destroy(&philosophers_info->table->mutex_time);
	free(philosophers_info);
}

void	init_table(t_table *table)
{
	int i;

	i = 0;
	while (i < NB_PHILO)
	{
		table->life[i] = MAX_LIFE;
		table->state[i] = SLEEP | THINK | EAT;
		table->chopstick[i] = ON_TABLE;
		if (pthread_mutex_init(&table->mutex_chopstick[i], NULL))
			exit(1);
		if (pthread_mutex_init(&table->mutex_state[i], NULL))
			exit(1);
		if (pthread_mutex_init(&table->mutex_life[i], NULL))
			exit(1);
		i++;
	}
	if (pthread_mutex_init(&table->mutex_time, NULL))
		exit(1);
}

void	*wait_for_philo_philosophers(void *param)
{
	philosophers_del(((t_param*)param)->philosophers, ((t_param*)param)->philosophers_info);
	return (NULL);
}

int		main(void)
{
	pthread_t		philosophers[NB_PHILO];
	t_table			table;
	t_philosopher	*philosophers_info;
	pthread_t		philo_del;
	t_param			param;

	//check error in macro

	init_table(&table);
	philosophers_info = philosphers_create(philosophers, &table);
	param.philosophers = philosophers;
	param.philosophers_info = philosophers_info;
	pthread_create(&philo_del, NULL, wait_for_philo_philosophers, (void*)&param);
	window(philosophers_info->table);
	pthread_join(philo_del, NULL);
	return (0);
}

// philo 3 eat all time !!!!!!!!!!!!!!!!!!!

// put life and timout counter : do numbers with images !
// add a death image
// lock mutex til philo_del is call then start simulation
// put mutex for life when write by manage_time and read by fct
// put mutex on all table var
// do mutex in window.c
