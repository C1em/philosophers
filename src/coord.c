/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coord.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coremart <coremart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 20:33:13 by coremart          #+#    #+#             */
/*   Updated: 2019/12/01 03:03:13 by coremart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <math.h>
#include <GLFW/glfw3.h>

#define PHILO_SIZE 0.18
#define TABLE_SIZE 0.6
#define CHOP_SIZE 0.1

void	fill_indices(GLuint indices[])
{
	int i;

	i = 0;
	while (i < NB_PHILO * 2 + 1)
	{
		indices[i * 6] = 0 + 4.0 * i;
		indices[i * 6 + 1] = 1 + 4.0 * i;
		indices[i * 6 + 2] = 2 + 4.0 * i;
		indices[i * 6 + 3] = 3 + 4.0 * i;
		indices[i * 6 + 4] = 1 + 4.0 * i;
		indices[i * 6 + 5] = 0 + 4.0 * i;
		i++;
	}
}


void	add_philo(GLfloat points[], int i)
{
	GLfloat x;
	GLfloat y;
	GLfloat teta;

	teta = i * 2.0 * M_PI / (GLfloat)NB_PHILO;
	x = 0.78 * cos(teta);
	y = 0.78 * sin(teta);
	points[0] = x + PHILO_SIZE * -1.0 * (cos(teta) - sin(teta));
	points[1] = y + PHILO_SIZE * -1.0 * (sin(teta) + cos(teta));
	points[2] = (GLfloat)i;

	points[3] = 0.0;
	points[4] = 0.0;


	points[5] = x + PHILO_SIZE * 1.0 * (cos(teta) - sin(teta));
	points[6] = y + PHILO_SIZE * 1.0 * (sin(teta) + cos(teta));
	points[7] = (GLfloat)i;

	points[8] = 1.0;
	points[9] = 1.0;


	points[10] = x + PHILO_SIZE * -1.0 * (cos(teta) + sin(teta));
	points[11] = y + PHILO_SIZE * 1.0 * (cos(teta) - sin(teta));
	points[12] = (GLfloat)i;

	points[13] = 0.0;
	points[14] = 1.0;


	points[15] = x + PHILO_SIZE * 1.0 * (cos(teta) + sin(teta));
	points[16] = y + PHILO_SIZE * -1.0 * (cos(teta) - sin(teta));
	points[17] = (GLfloat)i;

	points[18] = 1.0;
	points[19] = 0.0;
}

void	add_chopstick(GLfloat points[], int i)
{
	GLfloat x;
	GLfloat y;
	GLfloat teta;

	teta = ((2.0 * i - 1) * M_PI) / (GLfloat)NB_PHILO;
	x = 0.43 * cos(teta);
	y = 0.43 * sin(teta);
	points[0] = x + CHOP_SIZE * -1.0 * (cos(teta) - sin(teta));
	points[1] = y + CHOP_SIZE * -1.0 * (sin(teta) + cos(teta));
	points[2] = (GLfloat)i;

	points[3] = 0.0;
	points[4] = 0.0;


	points[5] = x + CHOP_SIZE * 1.0 * (cos(teta) - sin(teta));
	points[6] = y + CHOP_SIZE * 1.0 * (sin(teta) + cos(teta));
	points[7] = (GLfloat)i;

	points[8] = 1.0;
	points[9] = 1.0;


	points[10] = x + CHOP_SIZE * -1.0 * (cos(teta) + sin(teta));
	points[11] = y + CHOP_SIZE * 1.0 * (cos(teta) - sin(teta));
	points[12] = (GLfloat)i;

	points[13] = 0.0;
	points[14] = 1.0;


	points[15] = x + CHOP_SIZE * 1.0 * (cos(teta) + sin(teta));
	points[16] = y + CHOP_SIZE * -1.0 * (cos(teta) - sin(teta));
	points[17] = (GLfloat)i;

	points[18] = 1.0;
	points[19] = 0.0;
}

static inline void	fill_table(GLfloat points[])
{
	points[0] = TABLE_SIZE * -1.0;
	points[1] = TABLE_SIZE * -1.0;
	points[2] = 0.0;
	points[3] = 0.0;
	points[4] = 0.0;
	points[5] = TABLE_SIZE * 1.0;
	points[6] = TABLE_SIZE * 1.0;
	points[7] = 0.0;
	points[8] = 1.0;
	points[9] = 1.0;
	points[10] = TABLE_SIZE * -1.0;
	points[11] = TABLE_SIZE * 1.0;
	points[12] = 0.0;
	points[13] = 0.0;
	points[14] = 1.0;
	points[15] = TABLE_SIZE * 1.0;
	points[16] = TABLE_SIZE * -1.0;
	points[17] = 0.0;
	points[18] = 1.0;
	points[19] = 0.0;
}

void	fill_points(GLfloat points[])
{
	int i;

	fill_table(points);
	i = 0;
	while (i < NB_PHILO)
	{
		i++;
		add_philo(&points[i * 20], i);
	}
	while (i++ < NB_PHILO * 2)
		add_chopstick(&points[i * 20], i);
}
