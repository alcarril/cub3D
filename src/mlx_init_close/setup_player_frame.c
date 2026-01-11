/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_player_frame.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 11:42:33 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/11 13:37:58 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	NOTA:Falta meter los valores del mapa que lo hace carbon
	Inicializamos los componentes del juego:
	- Ponemso todos las variables de ca da elemento del array de texturas
	  a cero, no es estrcitamente necesario sumneta complejidad pero por 
	  robustez
	- Cargamos las texturas del mapa en caso de error liberamos los componentes
	  de la mlx y devolvemos false
	- Inicializamos los colores del suelo y el techo en formato hexadecimal
	  a partir de los valores RGB almacenados en el mapa
	- Inicializamos los datos del jugador llamando a la funcion setup_player
	- Inicializamos los datos del frame llamando a la funcion init_frame_data
	- Crear le fichero de log
*/
bool	setup_game(t_mlx *mlx, t_player *player, t_map *map, t_frame *frame)
{
	mlx->map = map;//quizas se quite
	ft_bzero((void *)mlx->map->textures, sizeof(mlx->map->textures));
	if (load_textures(mlx) == false)
	{
		destroy_mlx_componets(mlx_destroy_image, mlx_destroy_window, 
			mlx_destroy_display, mlx);
		return (false);
	}
	// init_floor_and_ceiling_colors(&map);
	map->floor_color_hex = rgb_to_hex(map->floor_color[0], map->floor_color[1], map->floor_color[2]);
	map->ceiling_color_hex = rgb_to_hex(map->ceiling_color[0], map->ceiling_color[1], map->ceiling_color[2]);
	mlx->player = player;
	setup_player_mouse(mlx);
	mlx->frame = frame;
	mlx->log_fd = create_fps_logfile();
	if (init_frame_data(mlx) == false || mlx->log_fd < 0)
	{
		free_game(mlx);
		return (false);
	}
	return (true);
}

/*
	Inicializar los datos del jugador: 
	- Inicializamos la posicion iniclial del jugador (parser val)
	- La orientacion inicla del jugador (parser val)
	- La velocidad de moviento (multiplicador del diferencial de cada componenete de los vectore de moviento)
	- El campo de vision del jugador (fov)
	- El volumen del jugador (radio de colision)
	- Los booleanos para los movientos, rotaciones y sprint del jugador
	Esta funcion va a tener que tener mas argumentos uno para la posicion iniclal metida en 
	un array de dos valores y otro mas para la orien
	tacion inicial del jugador (N,S,E,W).-> estos dato se obtienen en el parser.
	Iniiclaizamos los datos de los booleanos del moviento del jugador, el multiplicadpr para
	calculñar los diferenciales de los movientos
*/
void setup_player_mouse(t_mlx *mlx)
{
	int	middle[2];//esto se va a borrar
	
	middle[X] = mlx->map->max_columns / 2; //esto se va a borrar
	middle[Y] = mlx->map->max_rows / 2; //esto se va a borrar
	
	init_player_orientation_pos(mlx->player, 'N', middle);
	mlx->player->speed = 0.033f;
	mlx->player->fov = 60.0f;
	mlx->player->rad_fov = 60.0f * (PI / 180.0f);
	mlx->player->volume = EPSILON;
	ft_bzero((void *)&(mlx->player->keys), sizeof(mlx->player->keys));
	mlx->player->mouse.pos_x = mlx->win_width / 2;
	mlx->player->mouse.pos_y = mlx->win_height / 2;
	mlx->player->mouse.spin_pix = MOUSE_SENS;
	mlx->player->mouse.axis_x = mlx->win_width / 2;
	mlx->player->mouse.axis_y = mlx->win_height / 2;
	mlx->player->mouse.onoff = ON;
}

/*
	Inicializar los datos del jugador:
	- Inicializamos el angulo del jugador en funcion de la cardinalidad
	- Inicializamos la posicion del jugador en funcion de la posicion del mapa
*/
void	init_player_orientation_pos(t_player *pl, char cardinal, int pos[2])
{
	if (cardinal == 'N')
		pl->angle = 90.0f;
	if (cardinal == 'S')
		pl->angle = 270.0f;
	if (cardinal == 'E')
		pl->angle = 0.0f;
	if (cardinal == 'W')
		pl->angle = 180.0f;
	pl->rad_angle = pl->angle * (PI / 180.0f);
	pl->pos_x = (float)pos[X] + 0.5f;
	pl->pos_y = (float)pos[Y] + 0.5f;
}

/*
	Inicializar los datos del frame:
	- Inicializamos el minimapa como apagado
	- Inicializamos la opcion de mostrar los rayos en el minimapa como apagado
	- Inicializamos el procentaje del ancho y largo de la ventana que se usa para el minimapa
	- Inicializamos la escala del minimapa en funcion del procentaje del ancho y largo de la ventana
	  y del numero de filas y columnas del mapa original
	- Inicializamos el efecto fish eye como apagado
	- Inicializamos el efecto euclidean como apagado
*/
bool	init_frame_data( t_mlx *mlx)
{
	mlx->frame->minimap_onoff = false;
	mlx->frame->minimap_showrays = false;
	mlx->frame->mm_height= mlx->win_height / MINI_HEIGHT;
	mlx->frame->mm_widht = mlx->win_width / MINI_WIDTH;
	mlx->frame->raycasting_onoff = true;
	mlx->frame->fish_eye = false;
	mlx->frame->euclidean = false;
	mlx->frame->draw_walls = draw_wall_column_tex;
	mlx->frame->floor_celling = render_floor_and_ceiling;
	get_minimapscale(mlx, mlx->frame->mm_scale);
	mlx->frame->fov_distances = NULL;
	mlx->frame->fov_distances = (float *)malloc(sizeof(float) * mlx->win_width);
	if (!mlx->frame->fov_distances)
	{
		perror("Error allocating memory for fov_distances\n");
		return (false);
	}
	return (true);
}

/*
	Esta funcion sirve para poder sacar la escala del minimapa en funcion de:
	- El numero de filas y columnas del mapa original
	- El procentaje del ancho y largo de la ventana que se usa para el minimapa
*/
void	get_minimapscale(t_mlx *mlx, float *scale)
{
	scale[X] = (float)(mlx->frame->mm_widht) / mlx->map->max_columns;
	scale[Y] = (float)(mlx->frame->mm_height) / mlx->map->max_rows;
}