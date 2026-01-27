/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_algorith.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:07:51 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/27 03:50:34 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función principal para calcular la distancia de cada rayo hasta la pared 
	utilizando el algoritmo DDA.

	- Calcula la distancia inicial a los lados hasta la primera intersección 
	  con el límite de una celda (líneas horizontales y verticales).
	- Ejecuta el bucle DDA para encontrar la intersección con una pared.
	- Calcula la distancia al muro según la configuración del frame (euclidiana 
	  o no).
	- Aplica corrección de efecto de ojo de pez si está desactivado en la 
	  configuración del frame.
	- Retorna la distancia a la pared para pintar la columna correspondiente 
	  (corregida o no).
*/
float	get_distance_to_wall(t_mlx *mlx, t_ray *ray, float ray_angle)
{
	float	walldist;

	calc_side_dist(mlx, ray);
	dda_loop(mlx, ray);
	if (mlx->frame->euclidean == false)
		ray->wall_dist = get_ray_distance(mlx, ray);
	else
		ray->wall_dist = get_ray_distance_euclidean(mlx, ray);
	if (mlx->frame->fish_eye == false)
		walldist = ray->wall_dist * cos(ray_angle - (mlx->player->rad_angle));
	else
		walldist = ray->wall_dist;
	return (walldist);
}

/*
	En este paso, se compara `sidedist[X]` y `sidedist[Y]` para determinar si 
	el rayo cruza primero un borde vertical (eje X) o un borde horizontal (eje 
	Y). Según el resultado de la comparación:
	- Si `sidedist[X]` es menor, significa que el rayo cruza un borde vertical 
	  antes que un borde horizontal. En este caso:
	  - Se suma `delta[X]` a `sidedist[X]` para avanzar al siguiente borde 
		vertical.
	  - Se actualiza la posición del rayo en el eje X (`map[X]`) sumando el 
		valor de `step[X]`, que indica la dirección del movimiento en el eje X 
		(1 para derecha, -1 para izquierda).
	  - Se marca el tipo de borde cruzado como `VERTICAL` en `side_hit`.
	- Si `sidedist[Y]` es menor o igual, significa que el rayo cruza un borde 
	  horizontal antes que un borde vertical. En este caso:
	  - Se suma `delta[Y]` a `sidedist[Y]` para avanzar al siguiente borde 
		horizontal.
	  - Se actualiza la posición del rayo en el eje Y (`map[Y]`) sumando el 
		valor de `step[Y]`, que indica la dirección del movimiento en el eje Y 
		(1 para abajo, -1 para arriba).
	  - Se marca el tipo de borde cruzado como `HORIZONTAL` en `side_hit`.
*/
void	calc_side_dist(t_mlx *mlx, t_ray *ray)
{
	t_player	*pl;

	pl = mlx->player;
	if (ray->raydir[X] < 0)
	{
		ray->step[X] = -1;
		ray->sidedist[X] = (pl->pos_x - ray->map[X]) * ray->delta[X];
	}
	else
	{
		ray->step[X] = 1;
		ray->sidedist[X] = (ray->map[X] + 1.0 - pl->pos_x) * ray->delta[X];
	}
	if (ray->raydir[Y] < 0)
	{
		ray->step[Y] = -1;
		ray->sidedist[Y] = (pl->pos_y - ray->map[Y]) * ray->delta[Y];
	}
	else
	{
		ray->step[Y] = 1;
		ray->sidedist[Y] = (ray->map[Y] + 1.0 - pl->pos_y) * ray->delta[Y];
	}
}

/*
	Una vez que hemos calculado la primera intersección con la celda, según si
	ha sido la vertical o la horizontal, sabemos que en la siguiente celda, al
	encontrarnos en un borde, la siguiente distancia será la actual más el 
	valor que tiene que tomar Alpha o Beta (según sea vertical u horizontal) 
	para cruzar toda la celda. Por eso se lo sumamos a `sidedist`.

	El valor de la celda siempre es uno porque estamos trabajando en un espacio
	euclidiano, donde las celdas tienen un tamaño uniforme de 1x1. Esto implica
	que, al cruzar un borde (AX o AY), el incremento en la distancia será igual
	a `delta[X]` o `delta[Y]`, que representan la distancia necesaria para 
	recorrer una celda completa en el eje correspondiente. Por eso, se suma 
	`delta[X]` o `delta[Y]` a `sidedist`.

	La otra celda mantiene el valor anterior porque no se ha cruzado ese borde.
	De esta forma, se van acumulando los cambios de celda en celda hasta llegar
	a una pared.

	Con este proceso conseguimos saber cuál es la siguiente celda a la que 
	vamos a entrar y si es una pared o no, sin tener que calcular la 
	intersección exacta con las ecuaciones paramétricas de la recta. Esto evita
	hacer numerosos sistemas de ecuaciones que aumentarían mucho la complejidad
	computacional en comparación con el algoritmo DDA, que es mucho más sencill
	y rápido.

	El algoritmo DDA es eficiente porque:
	- No requiere cálculos complejos como intersecciones exactas o sistemas de
	  ecuaciones.
	- Reduce la complejidad computacional al trabajar directamente con las 
	  celdas del mapa.
	- Es aplicable en cualquier espacio euclidiano, lo que lo hace versátil y
	  eficiente para este tipo de problemas.
*/
void	dda_loop(t_mlx *mlx, t_ray *ray)
{
	int	hit;

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
		if (is_wall_tile(mlx->map->map_grids[ray->map[Y]][ray->map[X]]))
		{
			hit = 1;
			ray->wall_value = mlx->map->map_grids[ray->map[Y]][ray->map[X]];
		}
	}
}

/*
	Calcula la distancia perpendicular desde la posición del jugador hasta la 
	pared con la que el rayo ha colisionado. Este cálculo es necesario para 
	evitar el efecto de distorsión conocido como "efecto ojo de pez" y para 
	obtener una representación correcta de las paredes en el plano 3D.

	Existen dos formas principales de calcular la distancia a la pared:

	**Forma 1:**
	- La distancia a la pared se calcula restando `delta[X]` o `delta[Y]` a 
	  `sidedist[X]` o `sidedist[Y]`, respectivamente. Esto elimina la distancia
	  de la última celda que el rayo cruzó antes de salir del bucle DDA.
	  - Fórmula para eje X: `dist = ray->sidedist[X] - ray->delta[X]`.
	  - Fórmula para eje Y: `dist = ray->sidedist[Y] - ray->delta[Y]`.
	- En este caso, `sidedist` representa la hipotenusa del triángulo formado 
	  por el rayo y las celdas del mapa.

	**Forma 2:**
	- La distancia perpendicular se calcula según si el rayo ha chocado con una
	  pared vertical u horizontal. Esto se determina en función de la dirección
	  del rayo y la posición del jugador.
	- Fórmula general: 
	  `dist = (wall_pos - player_pos + face_compensation) / ray_direction`.
	- La compensación de la cara (`face_compensation`) se calcula como 
	  `(1 - step) / 2`. Esto asegura que, si el paso (`step`) es hacia atrás, 
	  se suma 1, y si es hacia adelante, se suma 0.
	- La dirección del rayo se utiliza para calcular la distancia perpendicular 
	  mediante triángulos semejantes. Esto permite obtener una distancia 
	  precisa y corregir el efecto de ojo de pez.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.
	- ray: Puntero a la estructura del rayo que contiene los datos calculados 
	  durante el algoritmo DDA.

	Retorno:
	- La distancia perpendicular desde el jugador hasta la pared con la que el 
	  rayo ha colisionado.
*/
float	get_ray_distance(t_mlx *mlx, t_ray *ray)
{
	float	dist;
	float	wall_dist;
	int		face_compensation;

	if (ray->side_hit == VERTICAL)
	{
		wall_dist = ray->map[X] - mlx->player->pos_x;
		face_compensation = ((1 - ray->step[X]) >> 1);
		dist = (wall_dist + face_compensation) / ray->raydir[X];
	}
	else
	{
		wall_dist = ray->map[Y] - mlx->player->pos_y;
		face_compensation = ((1 - ray->step[Y]) >> 1);
		dist = (wall_dist + face_compensation) / ray->raydir[Y];
	}
	return (dist);
}

/*
	Calcula la distancia euclidiana desde la posición del jugador hasta el 
	punto de impacto en la pared utilizando trigonometría básica (teorema de 
	Pitágoras). A diferencia de otras funciones de cálculo de distancia, esta 
	función no corrige el efecto de ojo de pez.

	Detalles de implementación:
	- La distancia euclidiana se calcula como la raíz cuadrada de la suma de 
	  los cuadrados de las diferencias entre las coordenadas del jugador y las 
	  coordenadas del punto de impacto del rayo en la pared.
	- Fórmula utilizada: 
	  `dist = sqrt((map[X] - pos_x)^2 + (map[Y] - pos_y)^2)`.

	Nota:
	- Este método no aplica corrección para el efecto de ojo de pez, lo que 
	  significa que las paredes pueden aparecer distorsionadas (cóncavas o 
	  convexas) dependiendo de la posición del jugador y el ángulo del rayo.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.
	- ray: Puntero a la estructura del rayo que contiene los datos calculados 
	  durante el algoritmo DDA.

	Retorno:
	- La distancia euclidiana desde el jugador hasta el punto de impacto en la 
	  pared.
*/
float	get_ray_distance_euclidean(t_mlx *mlx, t_ray *ray)
{
	float	fish_eye_dist;

	fish_eye_dist = sqrt(pow(ray->map[X] - mlx->player->pos_x, 2)
			+ pow(ray->map[Y] - mlx->player->pos_y, 2));
	return (fish_eye_dist);
}
