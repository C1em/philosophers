/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coremart <coremart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 17:29:07 by coremart          #+#    #+#             */
/*   Updated: 2019/12/04 09:32:29 by coremart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int		get_philo(int philo_nb)
{
	if (philo_nb == -1)
		return (NB_PHILO - 1);
	return (philo_nb % NB_PHILO);
}

void				philo_eat(t_philosopher *info)
{
	info->table->state[info->philo_nb] = EAT;
	pthread_mutex_unlock(&info->table->mutex_state[info->philo_nb]);
	info->table->chopstick[info->philo_nb] = TO_RIGHT;
	info->table->chopstick[get_philo(info->philo_nb + 1)] = TO_LEFT;
	pthread_mutex_unlock(&info->table->mutex_chopstick[info->philo_nb]);
	pthread_mutex_unlock(&info->table->mutex_chopstick[get_philo(info->philo_nb + 1)]);
	mutex_write(&info->table->mutex_life[info->philo_nb],
				&info->table->life[info->philo_nb], MAX_LIFE);
	usleep(((EAT_T * SECOND) - (time(NULL) - info->time_last_step)) > 0
	? (EAT_T * SECOND) - (time(NULL) - info->time_last_step)
	: 0);
	info->time_last_step = time(NULL);
	pthread_mutex_lock(&info->table->mutex_chopstick[info->philo_nb]);
	pthread_mutex_lock(&info->table->mutex_chopstick[get_philo(info->philo_nb + 1)]);
	info->table->chopstick[info->philo_nb] = ON_TABLE;
	info->table->chopstick[get_philo(info->philo_nb + 1)] = ON_TABLE;
	pthread_mutex_unlock(&info->table->mutex_chopstick[info->philo_nb]);
	pthread_mutex_unlock(&info->table->mutex_chopstick[get_philo(info->philo_nb + 1)]);
}

void			philo_rest(t_philosopher *info)
{
	info->table->state[info->philo_nb] = SLEEP;
	pthread_mutex_unlock(&info->table->mutex_state[info->philo_nb]);
	usleep(((REST_T * SECOND) - (time(NULL) - info->time_last_step)) > 0
	? (REST_T * SECOND) - (time(NULL) - info->time_last_step)
	: 0);
	info->time_last_step = time(NULL);
}

void			philo_think(t_philosopher *info)
{
	pthread_mutex_unlock(&info->table->mutex_life[info->philo_nb]);
	pthread_mutex_lock(&info->table->mutex_state[get_philo(info->philo_nb - 1)]);
	pthread_mutex_lock(&info->table->mutex_state[get_philo(info->philo_nb + 1)]);
	pthread_mutex_lock(&info->table->mutex_chopstick[info->philo_nb]);
	pthread_mutex_lock(&info->table->mutex_chopstick[get_philo(info->philo_nb + 1)]);
	if (info->table->state[get_philo(info->philo_nb - 1)] & THINK)
	{
		info->table->chopstick[info->philo_nb] = TO_RIGHT;
		info->table->state[info->philo_nb] = THINK;
	}
	else if (info->table->state[get_philo(info->philo_nb + 1)] & THINK)
	{
		info->table->chopstick[get_philo(info->philo_nb + 1)] = TO_LEFT;
		info->table->state[info->philo_nb] = THINK;
	}
	else if (info->table->chopstick[info->philo_nb] == ON_TABLE)
	{
		info->table->chopstick[info->philo_nb] = TO_RIGHT;
		info->table->state[info->philo_nb] = THINK;
	}
	else if (info->table->state[get_philo(info->philo_nb + 1)] == ON_TABLE)
	{
		info->table->chopstick[get_philo(info->philo_nb + 1)] = TO_LEFT;
		info->table->state[info->philo_nb] = THINK;
	}
	else
	{
		pthread_mutex_unlock(&info->table->mutex_state[get_philo(info->philo_nb - 1)]);
		pthread_mutex_unlock(&info->table->mutex_state[get_philo(info->philo_nb + 1)]);
		pthread_mutex_unlock(&info->table->mutex_chopstick[info->philo_nb]);
		pthread_mutex_unlock(&info->table->mutex_chopstick[get_philo(info->philo_nb + 1)]);
		pthread_mutex_unlock(&info->table->mutex_state[info->philo_nb]);
		return ;
	}
	pthread_mutex_unlock(&info->table->mutex_state[get_philo(info->philo_nb - 1)]);
	pthread_mutex_unlock(&info->table->mutex_state[get_philo(info->philo_nb + 1)]);
	pthread_mutex_unlock(&info->table->mutex_chopstick[info->philo_nb]);
	pthread_mutex_unlock(&info->table->mutex_chopstick[get_philo(info->philo_nb + 1)]);
	pthread_mutex_unlock(&info->table->mutex_state[info->philo_nb]);
	usleep(((THINK_T * SECOND) - (time(NULL) - info->time_last_step)) > 0
	? (THINK_T * SECOND) - (time(NULL) - info->time_last_step)
	: 0);
	info->time_last_step = time(NULL);
	pthread_mutex_lock(&info->table->mutex_chopstick[info->philo_nb]);
	if (info->table->chopstick[info->philo_nb] == TO_RIGHT)
		info->table->chopstick[info->philo_nb] = ON_TABLE;
	pthread_mutex_unlock(&info->table->mutex_chopstick[info->philo_nb]);
	pthread_mutex_lock(&info->table->mutex_chopstick[get_philo(info->philo_nb + 1)]);
	if (info->table->chopstick[get_philo(info->philo_nb + 1)] == TO_LEFT)
		info->table->chopstick[get_philo(info->philo_nb + 1)] = ON_TABLE;
	pthread_mutex_unlock(&info->table->mutex_chopstick[get_philo(info->philo_nb + 1)]);
}
