/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:04:35 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 18:55:51 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"


/*
	- Función principal para lanzar los rayos y realizar la bufferización
	- Hace el barrido de rayos desde la izquierda hasta la derecha del campo de visión.
	 Lo hace asi porque los ejes de la ventana en mlx estan invertidos respecto al minimapa.
	 Sino ocurriria un efecto espejo en la visualización 3D. y es mejor hacerlo asi que corregir desde
	 la bufferización..
	 
	Mejoras de rendimiento:
	- La conversiones a radianes se hacen desde los eventos de movientos para no tener que recalcular en cada paso del
	  raycasting (menos cilos ya que son diviones y multiplicaciones).
	- Uso de variables locales para optimizar el acceso a memoria y el uso del caché y registros.
	- Lass variables de delta_rays y fov_half se precalculan en la estructura del frame y jugador respectivamente
	  para evitar recalculos en cada frame.
*/
void	throw_rays(t_mlx *mlx)
{
	float			ray_angle;
	float			rad_dif;
	float			fov_half;
	unsigned int	n_ray;

	fov_half = mlx->player->fov_half;
	rad_dif = mlx->frame->delta_rays;
	ray_angle = (mlx->player->rad_angle) + fov_half;
	n_ray = 0;
	while ((int)n_ray < mlx->win_width)
	{
		cast_ray(mlx,n_ray, ray_angle);
		ray_angle -= rad_dif;
		n_ray++;
	}
}

/*
	Función auxiliar para manejar la lógica de un solo rayo
	- Inicializa los parámetros del rayo
	- Calcula la distancia hasta la pared utilizando el algoritmo DDA
	- Calcula la altura de la pared dentro de la matriz de pixeles de la ventana y
	  ajusta las posiciones de inicio y fin
	- Dibuja la columna de la pared en la posición correspondiente de la pantalla en dos modos:
	  - Modo simple (color fijo)
	  - Modo texturizado (textura aplicada)
	- Declaramos el rayo y el wall en memoria stack para optimizar el acceso a memoria y el uso del caché.
	 y evitar paginaciones de memoria lentas y que se tengan que liberar despues
*/
void	cast_ray(t_mlx *mlx, unsigned int n_ray, float ray_angle)
{
	t_ray	ray;
	t_wall	wall;
	
	set_ray(mlx, &ray, ray_angle);
	ray.proyected_wall_dist = get_distance_to_wall(mlx, &ray, ray_angle);
	mlx->frame->fov_distances[mlx->win_width - n_ray - 1] = ray.wall_dist;
	// scale_wall(&wall, ray.proyected_wall_dist, mlx->win_height, mlx->player->pitch_pix);
	scale_wall_phisics(&wall, ray.proyected_wall_dist, mlx);
	if (mlx->frame->textures_onoff == ON)
		draw_wall_column_tex(mlx, n_ray, &wall, &ray);
	else
		draw_wall_column(mlx, n_ray, &wall, &ray);
}

/*
	Inicializar los parámetros del rayo:
	- Dirección del rayo en los ejes X e Y
	- Ajuste de norte-sur en el seno
	- Posición del mapa basada en la posición del jugador
	- Ajuste de delta distancia para evitar divisiones por cero
	- Uso de valores muy grandes (1e30) para representar distancias infinitas
*/
void	set_ray(t_mlx *mlx, t_ray *ray, float ray_angle)
{
	ray->raydir[X] = cos(ray_angle);
	ray->raydir[Y] = -sin(ray_angle);
	ray->map[X] = (int)(mlx->player->pos_x);
	ray->map[Y] = (int)(mlx->player->pos_y);
	if (ray->raydir[X] == 0)
		ray->delta[X] = 1e30;
	else
		ray->delta[X] = fabs(1 / ray->raydir[X]);
	if (ray->raydir[Y] == 0)
		ray->delta[Y] = 1e30;
	else
		ray->delta[Y] = fabs(1 / ray->raydir[Y]);
}

/*
	Escala la altura de la pared en función de la distancia perpendicular:
	- Calcula la altura de la pared en píxeles basada en la distancia perpendicular
	- Calcula las posiciones de inicio y fin de la pared en pixeles de la matriz de la ventana
	- Asegura que las posiciones estén dentro de los límites de la matriz de pixeles de la ventana
	- Usa desplazamiento vertical (pitch) para ajustar la posición vertical de la pared a la traslacion
	 del conjunto de puntos del mapa segun la inclinacion (simulada) de la camara a traves de este metdodo
	NOTA:
	- wall star y wall end se tendrian que llamar alreves porque el sistema de coordenadas de mlx es invertido
	  respecto al minimapa, pero es mas facil de entender asi
	- 
*/
void scale_wall_phisics(t_wall *wall, float perpendicular_distance, t_mlx *mlx)
{
	static int	win_height;
	int			wallh_half;
	int			pitch;
	float		vertical_offset;
	float		camz;

	//valores estaticos para eficiencia y memeria local
	if (win_height == 0)
		win_height = mlx->win_height;
	wallh_half = wall->wall_height >> 1;
	
	//traslaciones y movientos del mapa con persespetivas para fisicas y simulaicon de eje y
	pitch = mlx->player->pitch_pix;
	camz = mlx->player->camz;
	mlx->player->vertical_offset = (camz * (win_height >> 1) / perpendicular_distance);
	vertical_offset = mlx->player->vertical_offset;

	//calculo de distancia
	if (perpendicular_distance <= 0)
		wall->wall_height = win_height;
	else
		wall->wall_height = (int)(win_height / perpendicular_distance);

	wall->wall_start = (win_height >> 1) - (wallh_half) + pitch + (int)vertical_offset;
	wall->wall_end = (win_height >> 1) + (wallh_half) + pitch + (int)vertical_offset;

	//control de limites
	if (wall->wall_start < 0)
		wall->wall_start = 0;
	if (wall->wall_end >= win_height)
		wall->wall_end = win_height - 1;
}


/*
	Dibuja una columna de pared en la pantalla:
	- Itera desde el inicio hasta el final de la pared en la pantalla
	- Bufferiza cada píxel de la columna con un color fijo (rojo en este caso)
	- Se podria usar buffering line para hacer lel dibujado por lineas y optimizar
	  un poco mas el rendimiento pero es menos flexible
*/
void	draw_wall_column(t_mlx *mlx, int column, t_wall *wall, t_ray *ray)
{
	int i;
	
	(void)ray;
	i = wall->wall_start;
	while (i <= wall->wall_end)
	{
		buffering_pixel(column, i, mlx, 0xFF8C00);
		i++;
	}
}
