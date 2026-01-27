/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_game.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 11:42:33 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/26 00:21:47 by carbon-m         ###   ########.fr       */
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
	- Retirna falso si hay algun error y libera recursos reservados (fds y 
	  memeria)
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
	map->max_distance = sqrtf((mlx->map->max_columns * mlx->map->max_columns)
			+ (mlx->map->max_rows * mlx->map->max_rows));
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
	Inicializa las características del jugador y su interacción con el ratón.

	Detalles:
	- Inicializa la posición inicial del jugador en el mapa (valores del parser
	- Configura la orientación inicial del jugador según la dirección cardinal 
	  (N, S, E, W) proporcionada por el parser.
	- Establece la velocidad de movimiento del jugador como un multiplicador 
	  del diferencial de cada componente de los vectores de movimiento.
	- Configura el campo de visión (FOV) del jugador en grados y lo convierte 
	  a radianes para cálculos trigonométricos.
	- Calcula la mitad del FOV en radianes para optimizar los cálculos de los 
	  rayos en el renderizado.
	- Inicializa los vectores de movimiento (`diff` y `v_move`) a cero usando 
	  `ft_bzero`.
	- Configura el volumen del jugador (`volume`), que representa el radio de 
	  colisión.
	- Inicializa el desplazamiento vertical del jugador (`pitch_pix`) para 
	  simular la inclinación de la cámara en el eje Y.
	- Configura el factor de inclinación (`pitch_factor`) y el máximo 
	  desplazamiento vertical (`max_pitch_pix`) en función de la altura de la 
	  ventana.
	- Inicializa los datos del ratón del jugador llamando a `init_player_mouse`
	- Configura las físicas del jugador llamando a `init_player_phisics`.
	- Inicializa los valores de las teclas de movimiento del jugador a `false` 
	  utilizando `ft_bzero`.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	NOTA:
	- Actualmente, se utilizan valores de prueba para la posición inicial del 
	  jugador (`middle[X]` y `middle[Y]`), que serán reemplazados por los 
	  valores reales del parser.
*/
void	setup_player_mouse(t_mlx *mlx)
{
	t_player	*pl;
	// int			middle[2];//esto se va a borrar
	// middle[X] = mlx->map->max_columns / 2; //esto se va a borrar
	// middle[Y] = mlx->map->max_rows / 2; //esto se va a borrar
	// middle[X] = 1; //esto se va a borrar
	// middle[Y] = 1; //esto se va a borra
	pl = mlx->player;
	// init_player_orientation_pos(mlx->player, 'N', middle);//esta quizas se quita
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
	Inicializa los datos de orientación y posición del jugador.

	Detalles:
	- Configura el ángulo inicial del jugador en función de la dirección 
	  cardinal (N, S, E, W) proporcionada por el parser.
	- Convierte el ángulo inicial del jugador de grados a radianes para 
	  facilitar los cálculos trigonométricos en el motor gráfico.
	- Establece la posición inicial del jugador en el mapa, ajustando las 
	  coordenadas para que el jugador se posicione en el centro de la celda 
	  correspondiente. Esto se logra sumando 0.5 a las coordenadas enteras 
	  proporcionadas por el parser.

	Parámetros:
	- pl: Puntero a la estructura del jugador que contiene los datos a 
	  inicializar.
	- cardinal: Carácter que indica la dirección inicial del jugador ('N', 'S'
	  'E', 'W').
	- pos: Array de dos enteros que contiene las coordenadas iniciales del 
	  jugador en el mapa.

	NOTA:
	- Esta función asume que las coordenadas proporcionadas por el parser son 
	  válidas y están dentro de los límites del mapa.
	- La orientación inicial del jugador es crucial para el correcto 
	  funcionamiento del raycasting y el movimiento en el juego.
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
	Inicializa los datos del frame para el renderizado y las configuraciones 
	del juego.

	Detalles:
	- Calcula el porcentaje del ancho y alto de la ventana que se usará para el 
	  minimapa.
	- Inicializa la escala del minimapa en función del porcentaje del ancho y 
	  alto de la ventana y del número de filas y columnas del mapa original.
	- Configura el minimapa como apagado por defecto.
	- Configura la opción de mostrar los rayos en el minimapa como apagado.
	- Desactiva el efecto de "fish eye" (ojo de pez) por defecto.
	- Desactiva el cálculo de distancias euclidianas por defecto.
	- Configura el renderizado de muros texturizados como activado.
	- Configura el modo de renderizado del suelo y techo.
	- Calcula el delta de ángulo entre rayos en función del FOV del jugador y 
	  el ancho de la ventana:
	  - `delta_rays = fov_rad / win_width` (esto evita calcular el ángulo de 
		cada rayo en cada frame).
	- Desactiva el modo de física por defecto.
	- Desactiva el modo DukeDoom por defecto.
	- Inicializa el array de distancias de cada rayo a la pared (`fov_distances

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Retorno:
	- `true` si la inicialización fue exitosa.
	- `false` si ocurrió un error al asignar memoria para `fov_distances`.

	NOTA:
	- La memoria para `fov_distances` se asigna dinámicamente en función del 
	  ancho de la ventana. Si la asignación falla, se muestra un mensaje de 
	  error y se retorna `false`.
*/
bool	init_frame_data( t_mlx *mlx)
{
	t_frame	*f;

	f = mlx->frame;
	f->mm_height = mlx->win_height / MINI_HEIGHT;
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
		return (perror("Error allocating memory for fov_distances\n"), false);
	return (true);
}

/*
	Crea un archivo de log para registrar los FPS del juego.

	Detalles:
	- Este archivo se utiliza para registrar los FPS por segundo que mantiene 
	  el juego durante su ejecución.
	- Permite analizar cómo afectan las diferentes configuraciones del motor 
	  gráfico al rendimiento del juego.
	- El archivo de log se crea en el directorio `/log` con el nombre 
	  `cub3d_log.txt`.
	- Si el archivo ya existe, se sobrescribe al iniciar una nueva ejecución 
	  del juego.

	Funcionamiento:
	- Abre o crea el archivo `cub3d_log.txt` en modo escritura (`O_WRONLY`), 
	  con las opciones `O_CREAT` (crear si no existe) y `O_TRUNC` (sobrescribir 
	  si ya existe).
	- Establece los permisos del archivo a `0644` (lectura y escritura para el 
	  propietario, solo lectura para otros usuarios).
	- Si ocurre un error al crear o abrir el archivo, muestra un mensaje de 
	  error con `perror` y retorna `-1`.

	Retorno:
	- Devuelve el file descriptor del archivo si se crea correctamente.
	- Devuelve `-1` si ocurre un error durante la creación o apertura del 
	  archivo.

	NOTA:
	- El directorio `log/` debe existir antes de ejecutar el programa, ya que 
	  esta función no lo crea automáticamente. Si el directorio no existe, la 
	  función fallará al intentar crear el archivo.
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
