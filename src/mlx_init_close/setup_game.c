/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_game.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 11:42:33 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 18:37:11 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

void	setup_default_phisics(t_phisics *phisics);

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
	- Retirna falso si hay algun error y libera recursos reservados (fds y memeria)
	- Retirn true si tidi va bien
*/
bool	setup_game(t_mlx *mlx, t_player *player, t_map *map, t_frame *frame)
{
	mlx->map = map;//quizas se quite
	mlx->map->n_textures = 5;
	if (load_textures(mlx) == false)
	{
		destroy_mlx_componets(mlx_destroy_image, mlx_destroy_window, 
			mlx_destroy_display, mlx);
		return (false);
	}
	init_floor_and_ceiling_colors(mlx->map);
	map->max_distance = sqrtf((mlx->map->max_columns * mlx->map->max_columns) + 
		(mlx->map->max_rows * mlx->map->max_rows));
	setup_default_ambiance(mlx->map, &(mlx->amb));
	setup_default_phisics(&(mlx->phisics));
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
	Inicializar las caracteristicas del jugador: 
	- Inicializamos la posicion iniclial del jugador (parser val)
	- La orientacion inicla del jugador (parser val)
	- La velocidad de moviento (multiplicador del diferencial de cada componenete de los vectore de moviento)
	- El campo de vision del jugador (fov)
	- El volumen del jugador (radio de colision)
	- Inicializamos el pitch_pixels (numero de pixeles de traslacion que simula la inclinacion de la camara en el eje y)
	- El pitch_factor que sirve el factor que se multiplica por max_pitch para savar el numero de pixeles de incliacion
	  de la camara del jugador (variable)
	- El max pitch que es el numero maximo de pixeles que puede trasladarse el mapa para simular la inclinaicon
	Inicializar los boleanos para el moviento del jugador con bezero (ver strcut player keys)
*/
void setup_player_mouse(t_mlx *mlx)
{
	int	middle[2];//esto se va a borrar
	t_player	*pl;
	
	// middle[X] = mlx->map->max_columns / 2; //esto se va a borrar
	// middle[Y] = mlx->map->max_rows / 2; //esto se va a borrar

	middle[X] = 1; //esto se va a borrar
	middle[Y] = 1; //esto se va a borra
	
	pl = mlx->player;
	init_player_orientation_pos(mlx->player, 'N', middle);//esta quizas se quita
	pl->speed = SPEED_DIGITAL;
	pl->fov = 60.0f;
	pl->rad_fov = pl->fov * (PI / 180.0f);
	pl->fov_half = pl->rad_fov / 2.0f;
	ft_bzero((void *)(pl->diff), sizeof(pl->diff));
	ft_bzero((void *)(pl->v_move), sizeof(pl->v_move));
	pl->volume = EPSILON;
	pl->pitch_pix = 0;
	pl->pitch_factor = PITCH_FACTOR;
	pl->max_pitch_pix = mlx->win_height * MAX_PIXELS_PITCH;
	init_player_mouse(pl, mlx);
	init_player_phisics(pl, &(mlx->phisics));
	ft_bzero((void *)&(pl->keys), sizeof(pl->keys));
}

/*
	Inicializar los datos del jugador:
	- Inicializamos el angulo del jugador en funcion de la cardinalidad
	- Inicilizamos el angulo en formato radianes
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
	- Inicializamos el procentaje del ancho y largo de la ventana que se usa para el minimapa
	- Inicializamos la escala del minimapa en funcion del procentaje del ancho y largo de la ventana
	  y del numero de filas y columnas del mapa original
	- Inicializamos el minimapa como apagado
	- Inicializamos la opcion de mostrar los rayos en el minimapa como apagado
	- Inicializamos el efecto fish eye como apagado
	- Inicializamos el efecto euclidean como apagado
	- Inicializamos el pintado de muros texturizado o no
	- Inicilaizamos el modo de renderizado el suelo y techo
	- Inicializamos el delta de angulo entre rayos en funcion del fov del jugador y el ancho de la ventana:
	  delta_rays = fov_rad / win_width (nos sorve para no tener que calcular el angulo de cada rayo en cada frame)
	- Inicializamos el modo de fisica como apagado
	- Inicializamos el modo dukedoom como apagado
	- Inicilaiciomos el array de distancias de cada rayo a la pared
*/
bool	init_frame_data( t_mlx *mlx)
{
	t_frame	*f;

	f = mlx->frame;
	f->mm_height= mlx->win_height / MINI_HEIGHT;
	f->mm_widht = mlx->win_width / MINI_WIDTH;
	f->mm_scale[X] = (float)(f->mm_widht) / mlx->map->max_columns;
	f->mm_scale[Y] = (float)(f->mm_height) / mlx->map->max_rows;
	f->mm_zoom_factor = MINI_ZOOM_FACTOR;
	f->minimap_onoff = OFF;
	f->minimap_showrays = false;
	f->raycasting_onoff = ON;
	f->fish_eye = OFF;
	f->euclidean = OFF;
	f->boost = OFF;
	f->textures_onoff = ON;
	f->ambiance_onoff = OFF;
	f->delta_rays = (mlx->player->rad_fov) / mlx->win_width;
	f->phisics_onoff = OFF;
	f->dukedoom_mode = OFF;
	f->fov_distances = NULL;
	f->fov_distances = (float *)malloc(sizeof(float) * mlx->win_width);
	if (!mlx->frame->fov_distances)
	{
		perror("Error allocating memory for fov_distances\n");
		return (false);
	}
	return (true);
}

/*
	Create log file: Este archivo esta creado para registrar
	los fps por segundo que mantiene el juego. Con el podemos
	apreciar como adecta a la tasa de fps las diferentes configuraciones
	del motor grafico que puedes hace enuestro programa. El archivo se 
	siutla en el drectorio /log que se crea cuando se hace make para compilar
	el programa. En cada ejecucucion del juego el archivo se borra y se vueleve
	a escribir en el.
	Retorna el file descriptor del archivo o -1 en caso de error en la creacion
*/
int	create_fps_logfile(void)
{
	int	log_fd;

	log_fd = open("log/cub3d_log.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (log_fd < 0)
	{
		perror("File error ");
		return (-1);
	}
	else
		return (log_fd);
}

