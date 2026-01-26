/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:04:35 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/26 15:42:05 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función principal para lanzar los rayos y realizar la bufferización. Realiza 
	el barrido de rayos desde la izquierda hasta la derecha del campo de visión. 
	Esto se hace de esta manera porque los ejes de la ventana en mlx están 
	invertidos respecto al minimapa. Si no se hiciera así, ocurriría un efecto 
	espejo en la visualización 3D. Es mejor hacerlo de esta forma que corregir 
	el efecto desde la bufferización.

	Mejoras de rendimiento:
	- Las conversiones a radianes se realizan desde los eventos de movimiento 
	  para evitar recalcular en cada paso del raycasting (menos ciclos, ya que 
	  son divisiones y multiplicaciones).
	- Uso de variables locales para optimizar el acceso a memoria, el uso del 
	  caché y los registros.
	- Las variables `delta_rays` y `fov_half` se precalculan en la estructura 
	  del frame y del jugador, respectivamente, para evitar recalculos en cada 
	  frame.
*/
void	throw_rays(t_mlx *mlx)
{
	float			ray_angle;
	float			rad_dif;
	float			fov_half;
	int				n_ray;
	int				win_width;

	win_width = mlx->win_width;
	// printf("Ancho de ventana: %d\n", win_width);
	fov_half = mlx->player->fov_half;
	rad_dif = mlx->frame->delta_rays;
	ray_angle = (mlx->player->rad_angle) + fov_half;
	n_ray = 0;
	while (n_ray < win_width)
	{
		cast_ray(mlx, n_ray, ray_angle);
		ray_angle -= rad_dif;
		n_ray++;
	}
}

/*
	Función auxiliar para manejar la lógica de un solo rayo. Esta función se 
	encarga de calcular la intersección del rayo con las paredes y de dibujar 
	la columna correspondiente en la pantalla.

	Flujo de la función:
	- Inicializa los parámetros del rayo, como su dirección y posición inicial 
	  en el mapa, utilizando la función `set_ray`.
	- Calcula la distancia del rayo hasta la pared más cercana utilizando el 
	  algoritmo DDA (Digital Differential Analyzer) con la función 
	  `get_distance_to_wall`.
	- Almacena la distancia perpendicular al muro en el buffer de distancias 
	  del frame actual.
	- Escala la altura de la pared en función de la distancia perpendicular 
	  utilizando la función `scale_wall_phisics`.
	- Dibuja la columna de la pared en la posición correspondiente de la 
	  pantalla. Esto puede hacerse en dos modos:
	  - **Modo simple:** Dibuja la pared con un color fijo.
	  - **Modo texturizado:** Aplica una textura a la pared. Si el modo de 
		iluminación ambiental está activado, se ajusta la textura para simular 
		efectos de luz y sombra.

	Mejoras de rendimiento:
	- El rayo (`ray`) y la pared (`wall`) se declaran en la memoria stack para 
	  optimizar el acceso a memoria y el uso del caché, evitando paginaciones 
	  de memoria lentas y la necesidad de liberar memoria después.
	- Los ifs se minimizan para reducir las ramas en el código, mejorando la 
	  predicción de saltos y el rendimiento general. Ademas el branch predictor
	  de la CPU funciona mejor con menos ramas condicionales.
*/
void	cast_ray(t_mlx *mlx, unsigned int n_ray, float ray_angle)
{
    t_ray	*ray;
    t_wall	*wall;
    
    ray = &mlx->frame->rays;
    wall = &mlx->frame->walls;
    set_ray(mlx, ray, ray_angle, n_ray);
    ray->proyected_wall_dist[n_ray] = get_distance_to_wall(mlx, ray, ray_angle, n_ray);
    mlx->frame->fov_distances[mlx->win_width - n_ray - 1] = ray->wall_dist[n_ray];
    scale_wall_phisics(wall, ray->proyected_wall_dist[n_ray], mlx, n_ray);
	// printf("holaaaa\n");
    if (mlx->frame->textures_onoff == ON)
    {
        if (mlx->frame->ambiance_onoff == OFF)
        {
            if (mlx->frame->boost == OFF)
                draw_wall_column_tex(mlx, n_ray, wall, ray);
            else
                drawwallcoltexspeed(mlx, n_ray, wall, ray);
        }
        else
            drawinglopp_tex_amb(mlx, n_ray, wall, ray);
    }
    else
        draw_wall_column(mlx, n_ray, wall, ray);
}

/*
	Inicializa los parámetros del rayo necesarios para realizar el algoritmo de 
	raycasting. Esta función configura la dirección y posición inicial del rayo 
	en el mapa, así como las distancias necesarias para calcular las 
	intersecciones con las paredes.

	Flujo de la función:
	- Calcula la dirección del rayo en los ejes X e Y utilizando funciones 
	  trigonométricas (`cos` y `sin`) basadas en el ángulo del rayo.
	- Determina la posición inicial del rayo en el mapa, basada en la posición 
	  actual del jugador.
	- Calcula las distancias delta para cada eje (X e Y), que representan la 
	  distancia que el rayo necesita recorrer para pasar de una línea de 
	  cuadrícula a la siguiente en el eje correspondiente.
	- Maneja casos especiales donde la dirección del rayo en un eje es 0, 
	  asignando un valor muy grande (1e30) para evitar divisiones por cero.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.
	- ray: Puntero a la estructura del rayo que se está inicializando.
	- ray_angle: Ángulo del rayo en radianes, que determina su dirección.

	Esta función es fundamental para el correcto funcionamiento del algoritmo 
	DDA, ya que establece los valores iniciales necesarios para calcular las 
	intersecciones del rayo con las paredes del mapa.
*/
void	set_ray(t_mlx *mlx, t_ray *ray, float ray_angle, unsigned int n_ray)
{
    ray->raydir_x[n_ray] = cos(ray_angle);
    ray->raydir_y[n_ray] = -sin(ray_angle);
    ray->map_x[n_ray] = (int)(mlx->player->pos_x);
    ray->map_y[n_ray] = (int)(mlx->player->pos_y);
    if (ray->raydir_x[n_ray] == 0)
        ray->delta_x[n_ray] = 1e30;
    else
        ray->delta_x[n_ray] = fabs(1 / ray->raydir_x[n_ray]);
    if (ray->raydir_y[n_ray] == 0)
        ray->delta_y[n_ray] = 1e30;
    else
        ray->delta_y[n_ray] = fabs(1 / ray->raydir_y[n_ray]);
}

/*
	Escala la altura de la pared en función de la distancia perpendicular:
	- Calcula la altura de la pared en píxeles basada en la distancia 
	  perpendicular.
	- Calcula las posiciones de inicio y fin de la pared en píxeles de la 
	  matriz de la ventana.
	- Asegura que las posiciones estén dentro de los límites de la matriz de 
	  píxeles de la ventana.
	- Usa el desplazamiento vertical (pitch) para ajustar la posición vertical
	  de la pared según la inclinación (simulada) de la cámara.
	- Calcula el offset vertical de la cámara basado en su posición en z 
	  (camz) y la distancia perpendicular para simular saltos y caídas.

	NOTA:
	- `wall_start` y `wall_end` deberían llamarse al revés porque el sistema de
	  coordenadas de mlx está invertido respecto al minimapa, pero es más fácil
	  de entender así.

	Mejoras de rendimiento:
	- La altura de la ventana se almacena en una variable estática para evitar 
	  acceder repetidamente a la estructura `mlx`, mejorando el rendimiento al 
	  usar registros de la CPU y el caché de la memoria, evitando cache misses.
	- Se minimizan las operaciones dentro de la función para reducir el tiempo 
	  de ejecución, utilizando variables locales para cálculos intermedios.
	- Se usan variables locales para optimizar el acceso a memoria y el uso del
	  caché.
*/
void	scale_wall_phisics(t_wall *wall, float perp_dist, t_mlx *mlx, unsigned int n_ray)
{
    static int	win_height;
    int			wallh_half;
    int			pitch;
    float		vo;
    float		camz;

    if (win_height == 0)
        win_height = mlx->win_height;
    pitch = mlx->player->pitch_pix;
    camz = mlx->player->camz;
    mlx->player->vertical_offset = (camz * (win_height >> 1) / perp_dist);
    vo = mlx->player->vertical_offset;
    if (perp_dist <= 0)
        wall->wall_height[n_ray] = win_height;
    else
        wall->wall_height[n_ray] = (int)(win_height / perp_dist);
    wallh_half = wall->wall_height[n_ray] >> 1;
    wall->wall_start[n_ray] = (win_height >> 1) - (wallh_half) + pitch + (int)vo;
    wall->wall_end[n_ray] = (win_height >> 1) + (wallh_half) + pitch + (int)vo;
    if (wall->wall_start[n_ray] < 0)
        wall->wall_start[n_ray] = 0;
    if (wall->wall_end[n_ray] >= win_height)
        wall->wall_end[n_ray] = win_height - 1;
}

/*
	Dibuja una columna de pared en la pantalla cunado noestamos usando texturas
	- Itera desde el inicio hasta el final de la pared en la pantalla.
	- Bufferiza cada píxel de la columna con un color fijo (naranja en este 
	  caso, 0xFF8C00).
	- Se podría usar `buffering_line` para realizar el dibujado por líneas y 
	  optimizar un poco más el rendimiento, pero este enfoque es menos 
	  flexible.

	Mejoras de rendimiento:
	- Uso de variables locales para optimizar el acceso a memoria y el uso del 
	  caché.
	- Se minimizan las operaciones dentro del bucle para reducir el tiempo de 
	  ejecución.
*/
void	draw_wall_column(t_mlx *mlx, int column, t_wall *wall, t_ray *ray)
{
    int	i;
    int	wall_end;

    (void)ray;
    wall_end = wall->wall_end[column];
    i = wall->wall_start[column];
    while (i <= wall_end)
    {
        buffering_pixel(column, i, mlx, 0xFF8C00);
        i++;
    }
}
