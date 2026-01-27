/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_celling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:21 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/27 03:51:13 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función para renderizar el suelo y el techo píxel a píxel.

	Detalles:
	- Calcula la línea del horizonte (origen o centro) basada en la altura de 
	  la ventana y la desviación vertical del jugador (`pitch_pix`), que 
	  representa la traslación vertical de los puntos del conjunto.
	- Itera sobre cada píxel desde la parte superior hasta la línea del 
	  horizonte y pinta el techo con el color definido en el mapa.
	- La linea del horizonte se calcula como la mitad de la altura de la ventan
	  más el pitch del jugador en píxeles, es una tralacion del origen vertical
	  de la camara para simular mirar hacia arriba o hacia abajo en vez de hace
	  una traslacion de los puntos del conjunti.
	- Luego, itera desde la línea del horizonte hasta la parte inferior de la 
	  ventana y pinta el suelo con el color definido en el mapa.
	- Divide la ventana en dos partes: techo y suelo, y las pinta con los 
	  colores especificados.

	Detalles adicionales:
	- Se utiliza en el renderizado antes de sobrescribir la imagen con las 
	  paredes, otros elementos 3D y el minimapa 2D.
	- Es más lento que `render_floor_and_ceiling_speed`, pero es más modular.
	- Es menos eficiente que renderizar el suelo y el techo directamente en el 
	  proceso de raycasting, pero mejora la modularidad del código.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Esta función es útil para dividir el renderizado en pasos más claros y 
	modulares, aunque con un mayor coste computacional.
*/
void	render_floor_and_ceiling(t_mlx *mlx)
{
	int	x;
	int	y;
	int	horizon;

	horizon = (mlx->win_height >> 1) + mlx->player->pitch_pix;
	y = -1;
	while (++y < horizon && y < mlx->win_height)
	{
		x = -1;
		while (++x < mlx->win_width)
			buffering_pixel(x, y, mlx, mlx->map->ceiling_color_hex);
	}
	while (y < mlx->win_height)
	{
		x = -1;
		while (++x < mlx->win_width)
			buffering_pixel(x, y, mlx, mlx->map->floor_color_hex);
		y++;
	}
}

/*
	Función para renderizar el suelo y el techo de forma rápida usando
	escritura por líneas completas en el framebuffer.

	Detalles:
	- En lugar de pintar píxel a píxel, la función rellena líneas completas
	  de memoria con `ft_memfillboost`, lo que reduce drásticamente el número
	  de operaciones y mejora el rendimiento.
	- Calcula la línea del horizonte en función de la altura de la ventana y
	  del desplazamiento vertical del jugador (`pitch_pix`).
	- Todas las líneas por encima del horizonte se rellenan con el color del
	  techo y todas las líneas por debajo con el color del suelo.
	- La escritura en el bitmap se realiza de forma secuencial, favoreciendo
	  el prefetch automático de caché y reduciendo fallos de caché.
	- Evita renderizar las líneas que están dentro de los límites de las 
	  paredes (entre `ceiling_limit` y `floor_limit`), previniendo la 
	  sobreescritura de píxeles que serán dibujados posteriormente por el 
	  renderizado de paredes. Esta optimización proporciona mejoras de 
	  rendimiento notables en mapas cerrados donde el horizonte apenas es 
	  visible, y será especialmente beneficiosa en futuras implementaciones 
	  de suelo y techo texturizados donde evitar cálculos redundantes será 
	  crítico para mantener un buen rendimiento.

	Organización de datos:
	- Las variables usadas dentro del bucle se agrupan en una estructura local
	  para mejorar la localidad de referencia y permitir al compilador mantener
	  valores en registros.
	- Se usan referencias locales a los datos de `mlx` para evitar accesos
	  repetidos a estructuras anidadas dentro del bucle crítico.

	Limitaciones:
	- Esta función solo es válida cuando el suelo y el techo no están
	  texturizados, ya que no se realizan cálculos por píxel ni muestreo de
	  texturas.

	Mejoras de rendimiento:
	- Reduce el coste por iteración al operar por líneas en lugar de píxeles.
	- Minimiza accesos a memoria dentro del bucle caliente (hot loop).
	- Accesos contiguos en memoria que maximizan el aprovechamiento de caché.
	- Evita renderizar píxeles que serán sobrescritos por las paredes.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene
	  el estado del frame, el framebuffer y la información del jugador.

	Uso recomendado:
	- Activar cuando no se usan texturas de suelo/techo y se prioriza el
	  rendimiento del renderizado
*/
void	render_floor_and_ceiling_speed(t_mlx *mlx)
{
	t_rf	s;
	int		ceiling_limit;
	int		floor_limit;

	load_locals(&s, mlx, &ceiling_limit, &floor_limit);
	s.offset = 0;
	s.y = 0;
	while (s.y < ceiling_limit && s.y < s.height)
	{
		ft_memfillboost(s.bitmap + s.offset, s.ceiling_color, s.line_bytes);
		s.offset += s.line_len;
		s.y++;
	}
	while (s.y <= floor_limit && s.y < s.height)
	{
		s.offset += s.line_len;
		s.y++;
	}
	while (s.y < s.height)
	{
		ft_memfillboost(s.bitmap + s.offset, s.floor_color, s.line_bytes);
		s.offset += s.line_len;
		s.y++;
	}
}

/*
	Función auxiliar para cargar variables locales necesarias para el
	renderizado rápido del suelo y el techo.

	Parámetros:
	- s: Puntero a la estructura local `t_rf` donde se almacenarán las
	  variables.
	- mlx: Puntero a la estructura principal del motor gráfico que contiene
	  toda la información del juego.
	- ceiling_limit: Puntero a un entero donde se almacenará el límite
	  inferior del techo.
	- floor_limit: Puntero a un entero donde se almacenará el límite
	  superior del suelo.

	Funcionalidad:
	- Extrae y almacena en `s` las dimensiones de la ventana, la dirección
	  del bitmap, la longitud de línea, los bytes por píxel y los colores
	  del techo y el suelo.
	- Calcula los límites de dibujo para el techo y el suelo basándose en
	  los datos del frame actual.
	- Asegura que los límites no excedan las dimensiones de la ventana.

	Esta función prepara todas las variables necesarias para optimizar el
	renderizado rápido del suelo y el techo.
*/
void	load_locals(t_rf *s, t_mlx *mlx, int *ceiling_limit, int *floor_limit)
{
	s->width = mlx->win_width;
	s->height = mlx->win_height;
	s->bitmap = mlx->bit_map_address;
	s->line_len = mlx->line_length;
	s->bpp = mlx->bits_per_pixel >> 3;
	s->ceiling_color = mlx->map->ceiling_color_hex;
	s->floor_color = mlx->map->floor_color_hex;
	s->line_bytes = s->width * s->bpp;
	*ceiling_limit = mlx->frame->wall_start_min;
	*floor_limit = mlx->frame->wall_end_max;
	if (*ceiling_limit < 0)
		*ceiling_limit = 0;
	if (*ceiling_limit >= s->height)
		*ceiling_limit = s->height - 1;
	if (*floor_limit < 0)
		*floor_limit = 0;
	if (*floor_limit >= s->height)
		*floor_limit = s->height - 1;
}

/*
	Función para renderizar el suelo y el techo con efectos de ambiente.

	Detalles:
	- Similar a `render_floor_and_ceiling_speed`, pero aplica efectos de 
	  sombreado y niebla basados en la distancia desde la cámara.
	- Calcula un factor de distancia (`dist_factor`) para cada línea, que 
	  determina la intensidad de los efectos de ambiente.
	- Aplica sombreado y niebla al color del suelo y el techo antes de 
	  dibujarlos, creando una sensación de profundidad y atmósfera en la escena 
	  3D.
	- El sombreado y la niebla se aplican utilizando funciones específicas como 
	  `applyamb_ceiling` y `applyamb_floor`, que ajustan los colores según el 
	  factor de distancia.

	Mejoras de rendimiento:
	- Reduce la sobrecarga de llamadas a funciones al pintar líneas completas 
	  en lugar de píxeles individuales.
	- Minimiza las indirecciones dentro del bucle, permitiendo un acceso más 
	  eficiente a las variables (usando registros en lugar de registro -> 
	  caché -> RAM).
	- Utiliza referencias locales para evitar accesos repetidos a estructuras 
	  anidadas, mejorando el rendimiento dentro de las restricciones de la 
	  norma de 42.
	- Evita el uso de condiciones dentro del bucle para aplicar los efectos de 
	  ambiente, eliminando saltos de instrucción que penalizan el rendimiento 
	  del procesador en hoot loops.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Esta función mejora significativamente la calidad visual al añadir efectos 
	de profundidad y atmósfera, aunque con un mayor coste computacional debido 
	a los cálculos adicionales por píxel.
*/
void	render_floor_and_ceiling_amb(t_mlx *mlx)
{
	int				y;
	int				refs[3];
	float			df;
	unsigned int	colors[2];

	colors[0] = mlx->map->ceiling_color_hex;
	colors[1] = mlx->map->floor_color_hex;
	refs[HEIG] = mlx->win_height;
	refs[WIDT] = mlx->win_width;
	refs[HOR] = (refs[0] >> 1) + mlx->player->pitch_pix;
	y = 0;
	while (y < refs[HOR] && y < refs[HEIG])
	{
		df = dist_factor_ceiling(y, refs[HOR], mlx->amb.ambiance);
		colors[0] = apllyamb_ceiling(&(mlx->amb), df, colors[0]);
		buffering_line(y, colors[0], mlx, refs[WIDT]);
		y++;
	}
	while (y < refs[HEIG])
	{
		df = dist_factor_floor(refs[HEIG], y, refs[HOR], mlx->amb.ambiance);
		colors[1] = apllyamb_ceiling(&(mlx->amb), df, colors[1]);
		buffering_line(y, colors[1], mlx, refs[WIDT]);
		y++;
	}
}
