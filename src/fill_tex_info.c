/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tex_info.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coremart <coremart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 21:42:24 by coremart          #+#    #+#             */
/*   Updated: 2019/12/04 08:50:10 by coremart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fill_tex_idx(int tex_idx_arr[], t_table * table)
{
	int i;

	tex_idx_arr[0] = tex_table;
	i = 0;
	while (i++ < NB_PHILO)
	{
		pthread_mutex_lock(&table->mutex_state[i - 1]);
		if (table->state[i - 1] & SLEEP)
			tex_idx_arr[i] = tex_sleep;
		else if (table->state[i - 1] & THINK)
			tex_idx_arr[i] = tex_think;
		else if (table->state[i - 1] & EAT)
			tex_idx_arr[i] = tex_eat;
		pthread_mutex_unlock(&table->mutex_state[i - 1]);
	}
	while (i < NB_PHILO * 2 + 1)
	{
		tex_idx_arr[i] = tex_fork;
		i++;
	}

}

// void	fill_fork_pos(int fork_pos_arr[], t_table * table)
// {
// 	int i;
// 	i = 0;
// 	while (i < NB_PHILO)
// 	{

// 	}
// }
