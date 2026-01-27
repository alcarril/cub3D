/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:44:58 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/27 02:34:29 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Selecciona la textura adecuada en función de la dirección del rayo y del 
	lado de la pared que fue golpeada.

	Detalles:
	- Cuando la dirección del rayo es positiva y el rayo ha chocado con una 
	  celda vertical, significa que la última colisión ha sido con una pared 
	  de los laterales derecho o izquierdo del mapa (límite de la cuadrícula 
	  del mapa). Según la pendiente, si la componente del vector unitario del 
	  rayo en el eje X es positiva o negativa, significa que ha sido con una 
	  pared del este o del oeste, respectivamente.
	- Cuando la dirección del rayo es negativa y el rayo ha chocado con una 
	  celda horizontal, significa que la última colisión ha sido con una pared 
	  de la parte superior o inferior del mapa (límite de la cuadrícula del 
	  mapa). Según la pendiente, si la componente del vector unitario del rayo 
	  en el eje Y es positiva o negativa, significa que ha sido con una pared 
	  del norte o del sur, respectivamente.
	- Además, si el rayo ha chocado con una pared especial (`BONUS_WALL`), se 
	  selecciona la textura correspondiente a esa pared especial. Esto se 
	  determina según el tipo de celda del mapa con la que el rayo ha colisiona
	- La variable `raydir` siempre está comprendida entre -1 y 1 porque es un 
	  vector unitario. Esto se basa en la circunferencia goniométrica. También 
	  se podría usar el ángulo del rayo, pero sería más costoso 
	  computacionalmente. Alternativamente, se podrían usar `delta_x` y 
	  `delta_y`, pero la lógica sería la misma.

	Parámetros:
	- mlx: Puntero a la estructura principal que contiene toda la información 
	  del juego.
	- ray: Puntero a la estructura del rayo que contiene información sobre la 
	  dirección del rayo y el lado golpeado.

	Retorno:
	- Puntero a la textura seleccionada.
*/
t_texture	*select_texture(t_mlx *mlx, t_ray *ray)
{
	t_texture	*texture;

	if (ray->wall_value == BONUS_WALL)
	{
		texture = &(mlx->map->textures)[4];
		return (texture);
	}
	if (ray->side_hit == VERTICAL)
	{
		if (ray->raydir[X] > 0)
			texture = &(mlx->map->textures)[E];
		else
			texture = &(mlx->map->textures)[W];
	}
	else
	{
		if (ray->raydir[Y] > 0)
			texture = &(mlx->map->textures)[N];
		else
			texture = &(mlx->map->textures)[S];
	}
	return (texture);
}

/*
	Calcula la posición horizontal relativa (proporción) donde el rayo golpea 
	la pared en la textura.

	Detalles:
	- La función utiliza las ecuaciones paramétricas de lalíneax = x0 + t * dx
	  - x0 e y0 son las coordenadas del jugador.
	  - dx y dy son las direcciones del rayo.
	  - t es la distancia a la pared (hipotenusa aproximada).
	- Calcula la posición del impacto del rayo en la pared (wall_x) como una 
	  proporción entre 0 y 1. Esto indica qué parte de la textura corresponde 
	  al punto de impacto.
	- Se realiza un ajuste para obtener solo la parte decimal de `wall_x` 
	  (wall_x - floor(wall_x)).
	- Si el rayo golpea una pared en un lado específico (vertical u horizontal), 
	  se realiza un ajuste adicional para evitar que las texturas se inviertan 
	  en ciertas caras. Esto asegura que las texturas se muestren correctamente 
	  en todas las paredes.

	Nota:
	- La variable `wall_x` es un porcentaje (0-1) que indica la posición del 
	  impacto del rayo en la base o eje X de la textura.
	- Se utiliza la distancia de la hipotenusa (`wall_dist`) en lugar de la 
	  proyección para evitar desplazamientos en las texturas debido al efecto 
	  de ojo de pez.

	Parámetros:
	- mlx: Puntero a la estructura principal que contiene toda la información 
	  del juego.
	- ray: Puntero a la estructura del rayo que contiene información sobre el 
	  impacto del rayo en la pared.

	Retorno:
	- `wall_x`: Proporción (0-1) de la posición del impacto del rayo en la base 
	  o eje X de la textura.
*/
double	calculate_wall_x(t_mlx *mlx, t_ray *ray)
{
	double	wall_x;

	if (ray->side_hit == VERTICAL)
		wall_x = mlx->player->pos_y + ray->wall_dist * ray->raydir[Y];
	else
		wall_x = mlx->player->pos_x + ray->wall_dist * ray->raydir[X];
	wall_x -= (int)(wall_x);
	if ((ray->side_hit == VERTICAL && ray->raydir[X] < 0)
		|| (ray->side_hit == HORIZONTAL && ray->raydir[Y] > 0))
	{
		wall_x = 1.0 - wall_x;
	}
	return (wall_x);
}

/*
	Calcula los parámetros necesarios para renderizar la textura:

	- **tex_x:** Coordenada X en la matriz de píxeles de la textura. Representa 
	  la posición horizontal en la textura que corresponde al punto de impacto 
	  del rayo en la pared. Se calcula multiplicando la proporción de la 
	  posición del impacto en la pared (`wall_x`) por el ancho de la textura. 
	  Esto escala la posición relativa (0-1) al rango de píxeles de la textura.
	- **text_v_step:** Proporción entre la altura de la textura y la altura de 
	  la pared en píxeles. Esto determina cuánto se debe avanzar en la textura 
	  por cada píxel dibujado en la pantalla, asegurando que la textura no se 
	  deforme.
	- **tex_pos:** Posición inicial en la textura en el eje Y (vertical) desde 
	  donde se comenzará a muestrear la textura. Esto se calcula en función de 
	  la posición de inicio de la pared en la pantalla (`wall_start`), el 
	  centro de la pantalla (`horizon`), y el desplazamiento vertical del 
	  jugador (`pitch` y `vertical_offset`). Este cálculo asegura que la 
	  textura esté alineada correctamente con la pared renderizada en la 
	  pantalla.

	Variables:
	- **horizon:** Representa el centro de la pantalla ajustado por el pitch 
	  (desplazamiento vertical del jugador) y el desplazamiento vertical 
	  adicional (`vertical_offset`). Es el punto de referencia para alinear la 
	  textura.
	- **ws_offset:** Diferencia entre el inicio de la pared (`wall_start`) y el 
	  horizonte (`horizon`). Indica cuánto se debe desplazar la textura 
	  verticalmente para alinearla con la pared.
	- **wall_height_half:** Mitad de la altura de la pared en píxeles. Se usa 
	  para centrar la textura en la pared.

	Parámetros:
	- wall: Estructura que contiene información sobre la pared.
	- texture: Puntero a la textura.
	- win_height: Altura de la ventana.
	- player: Puntero a la estructura del jugador.

	NOTA:
	- `wall_start` está invertido debido al sistema de coordenadas de la 
	  librería gráfica utilizada.
	- Se pueden optimizar algunos cálculos precalculando valores constantes 
	  como el horizonte en eventos de cambio de ventana o ajustes de `pitch`.
*/
void	calculate_tex(t_wall *wall, t_texture *texture, int winh, t_player *pl)
{
	int	horizon;
	int	ws_offset;
	int	wall_height_half;

	wall->tex_x = (int)(wall->wall_x * (double)texture->width);
	wall->text_v_step = (float)texture->height / (float)wall->wall_height;
	horizon = (winh >> 1) + pl->pitch_pix + (int)wall->vertical_offset;
	ws_offset = wall->wall_start - horizon;
	wall_height_half = wall->wall_height >> 1;
	wall->tex_pos = (ws_offset + wall_height_half) * wall->text_v_step;
}

/*
	Extrae el color de un píxel de una textura en base a las coordenadas 
	`tex_x` y `tex_y`.

	Detalles:
	1. Valida las coordenadas `tex_x` y `tex_y` para asegurarse de que están 
	   dentro de los límites de la textura.
	2. Calcula la dirección del píxel en la memoria de la textura utilizando 
	   `tex_x`, `tex_y`, `line_length` y `bits_per_pixel`.
	3. Lee el color desde la dirección calculada en la memoria.
	4. Devuelve el color como un entero (`unsigned int`).

	Parámetros:
	- texture: Puntero a la textura de la que se extraerá el color.
	- tex_x, tex_y: Coordenadas del píxel en la textura.

	Retorno:
	- El color del píxel como un entero (`unsigned int`).

	Optimizaciones de microprocesador (pendiente):
	- Se evita la división utilizando un desplazamiento de bits para calcular 
	  el tamaño del píxel en bytes. Se sustituye 
	  `texture->bits_per_pixel / 8` por `texture->bits_per_pixel >> 3`.
	- No se pueden usar variables locales estáticas porque la imagen de las 
	  texturas puede cambiar en tiempo de ejecución si se cargan nuevas 
	  texturas o se liberan de memoria.
*/
unsigned int	extract_color(t_texture *texture, int tex_x, int tex_y)
{
	unsigned int	color;
	char			*pixel_address;

	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= texture->height)
		tex_y = texture->height - 1;
	pixel_address = texture->addr + (tex_y * texture->line_length)
		+ (tex_x * (texture->bits_per_pixel >> 3));
	color = *(unsigned int *)pixel_address;
	return (color);
}
