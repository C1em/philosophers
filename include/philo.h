/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coremart <coremart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 20:14:04 by coremart          #+#    #+#             */
/*   Updated: 2020/08/23 11:53:37 by coremart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> // ????
# include <stdbool.h>
# include <time.h>
# include <GLFW/glfw3.h>
# include <pthread.h>

/*
**	variable macro
*/

# define NB_PHILO 5
# define MAX_LIFE 50
# define EAT_T 5
# define REST_T 3
# define THINK_T 7
# ifndef TIMEOUT
#  define TIMEOUT 60
# else
printf("error : timeout macro already exist\n");
exit(1);
# endif

/*
**	static macro
*/

# define EQUAL 0
# define AND 1
# define GREATER_EQ 2
# define SMALLER 3

# define SLEEP 1
# define THINK 2
# define EAT 4
# define SECOND 1000000

# define ON_TABLE 0
# define TO_RIGHT 1 // to the philo at the right from the philo pov
# define TO_LEFT 2

# define STR_TEX_TABLE "0"
# define STR_TEX_SLEEP "1"
# define STR_TEX_EAT "2"
# define STR_TEX_THINK "3"

typedef enum	e_textures
{
	tex_table = 0,
	tex_sleep,
	tex_eat,
	tex_think,
	tex_fork
}				t_textures;


// make one array of union
typedef struct	s_table
{
	pthread_mutex_t	mutex_chopstick[NB_PHILO];
	pthread_mutex_t	mutex_state[NB_PHILO];
	pthread_mutex_t	mutex_life[NB_PHILO];
	pthread_mutex_t	mutex_time;
	int				life[NB_PHILO]; // put life in last bits of state ????
	int				state[NB_PHILO];
	int				chopstick[NB_PHILO]; // chopstick[i] is the one at the left of philo[i]
	time_t			time_at_start;
}				t_table;

typedef struct	s_philospher
{
	t_table				*table;
	time_t				time_last_step;
	int					philo_nb;
}				t_philosopher;

typedef struct	s_param
{
	pthread_t			*philosophers;
	t_philosopher		*philosophers_info;
}				t_param;

/*
**	state.c
*/
void		philo_eat(t_philosopher *info);
void		philo_rest(t_philosopher *info);
void		philo_think(t_philosopher *info);
int			get_philo(int philo_nb);

/*
**	window.c
*/
void		*window(t_table *table);
GLFWwindow	*init_win(void);

/*
**	coord.c
*/
void	fill_points(GLfloat points[]);
void	fill_indices(GLuint indices[6 * (NB_PHILO + 1)]);

/*
**	fill_tex_info.c
*/
void	fill_tex_idx(int tex_idx_arr[], t_table * table);

/*
**	mutex_io.c
*/
void	mutex_write(pthread_mutex_t *mutex, int *dest, int value);

#endif
