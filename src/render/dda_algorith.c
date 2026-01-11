/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_algorith.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:07:51 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/10 15:09:04 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función principal para calcular la distancia hasta la pared utilizando el algoritmo DDA
	- Calcula la distancia inicial a los lados
	- Ejecuta el bucle DDA para encontrar la intersección con una pared
	- Calcula la distancia al muro según la configuración del frame (euclidiana o no)
	- Aplica corrección de efecto de ojo de pez si está desactivado en la configuración del frame
*/
float	get_distance_to_wall(t_mlx *mlx, t_ray *ray, float ray_angle)
{
	float	wall_dist_corrected;
	
	calc_side_dist(mlx, ray);
	dda_loop(mlx, ray);
	if (mlx->frame->euclidean == false)
		ray->wall_dist = get_ray_distance(mlx, ray);
	else
		ray->wall_dist = get_ray_distance_euclidean(mlx, ray);

	if (mlx->frame->fish_eye == false)
		wall_dist_corrected = ray->wall_dist * cos(ray_angle - (mlx->player->rad_angle));
	else
		wall_dist_corrected = ray->wall_dist;
	return (wall_dist_corrected);
}

// Determinar la dirección del paso y la distancia inicial a los lados
//falta control de errores de seno y conseno siempre es entre -1 y 1
/*
	CAlcuacmos la distancia hasta la primera interseccion con una celda del mapa
	espacio euclidiano segun ella doreccion del rayo sea negaiva o positiva en 
	cada una de la componentes del vector unitario que define su direcccion.
*/
void	calc_side_dist(t_mlx * mlx, t_ray *ray)
{
	if (ray->raydir[X] < 0)
	{
		ray->step[X] = -1;
		ray->sidedist[X] = (mlx->player->pos_x - ray->map[X]) * ray->delta[X];
	}
	else
	{
		ray->step[X] = 1;
		ray->sidedist[X] = (ray->map[X] + 1.0 - mlx->player->pos_x) * ray->delta[X];
	}
	if (ray->raydir[Y] < 0)
	{
		ray->step[Y] = -1;
		ray->sidedist[Y] = (mlx->player->pos_y - ray->map[Y]) * ray->delta[Y];
	}
	else
	{
		ray->step[Y] = 1;
		ray->sidedist[Y] = (ray->map[Y] + 1.0 - mlx->player->pos_y) * ray->delta[Y];
	}
}

// Avanzar al siguiente lado de la celda
/*
	Una vex que hemos calculado la primera interseccion con la celda
	Segun si ha dido la vertical o la horizaontal sabemosq qeu en la siguietnte
	celda al encontrarnos en unborde la siguiente distncia seria la actual mas
	el valor que tiene que tomar Alpha o Beta segun sea vertical u horizontal
	para cruzar toda la celda por eso se lo sumamos a sidedist. Como ese valor es uno
	hemos cruzado el borde AX o AY * delta x o Y valdra delta X o Y por eso se el suma el valor
	LA otras celda mantiene el valor anterior no se le suma nada porqeur no se ha cruzado ese borde
	asi se van acumulando los cambios de celda en celda hasta llegar a una pared.
*/
void	dda_loop(t_mlx * mlx, t_ray *ray)
{
	int hit;

	hit = 0;
	while (hit == 0)
	{
		if (ray->sidedist[X] < ray->sidedist[Y])
		{
			ray->sidedist[X] += ray->delta[X];
			ray->map[X] += ray->step[X];
			ray->side_hit = VERTICAL;
		}
		else
		{
			ray->sidedist[Y] += ray->delta[Y];
			ray->map[Y] += ray->step[Y];
			ray->side_hit = HORIZONTAL;
		}
		if (mlx->map->map_grids[ray->map[Y]][ray->map[X]] == WALL)
			hit = 1;
	}
}

//dda algorithm calculus
/*
	-Forma 1:
	LA distacnia ala pared se calcula con sidedist - delta dist para quitarle
	la distancia de la ultima celda que hemos cruzado y que nos ha hecho salir del bucle.
	dist = ray->sidedist[X] - ray->delta[X]; (sidedist es la hipotenusa)
	dist = ray->sidedist[Y] - ray->delta[Y]; (sidedist es la hipotenusa)
	-Forma 2:
	La distancia perpendicular se calcula segun si hemos chocado con una pared vertical u horizontal
	segun la direccion del rayo y la posicion del jugador.
	Distancia =  posicion oared - posicion jugaror + compensacion cara / direccion rayo
	La compensacion cara es (1 - step) / 2 porque si el paso es hacia atras conseguimos sumarle 1 y si es hacia adelante le sumamos 0
	Ladireccion de rayo es por los trianglos semenjantes con esto conseguimos la distancia perpendicular
*/
float get_ray_distance(t_mlx *mlx, t_ray *ray)
{
	float	dist;
	float	wall_dist;
	int		face_compensation;
	
	if (ray->side_hit == VERTICAL)
	{
		wall_dist = ray->map[X] - mlx->player->pos_x;
		face_compensation = (1 - ray->step[X]) / 2;
		dist = (wall_dist + face_compensation) / ray->raydir[X];
	}
	else
	{
		wall_dist = ray->map[Y] - mlx->player->pos_y;
		face_compensation = (1 - ray->step[Y]) / 2;
		dist = (wall_dist + face_compensation) / ray->raydir[Y];
	}
	return (dist);
}

/*
	Calcula la distancia euclidiana desde la posición del jugador hasta el punto de impacto en la pared
	usando trifonometría básica (teorema de Pitágoras). Esta función no corrige el efecto de ojo de pez.
	Hace que las apredes se vean concavas o convexas dependiendo de la posicion del jugador y el angulo del rayo.
*/
float get_ray_distance_euclidean(t_mlx *mlx, t_ray *ray)
{
	float fish_eye_dist;
	
	fish_eye_dist = sqrt(pow(ray->map[X] - mlx->player->pos_x, 2) +
							   pow(ray->map[Y] - mlx->player->pos_y, 2));
	 return fish_eye_dist;
}

