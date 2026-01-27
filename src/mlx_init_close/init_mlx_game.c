/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx_game.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 20:07:49 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/24 00:53:00 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Inicializamos los componentes de la mlx:
	- Inicializamos la conexión con el servidor X11 (Xorg) con `mlx_init`. Esto 
	  establece la comunicación entre el programa y el servidor gráfico.
	- Configuramos las dimensiones de la ventana en función del número de filas 
	  y columnas del mapa y de una escala definida (`WIN_SCALE`).
	- Creamos la ventana con `mlx_new_window` y la asociamos a la conexión 
	  establecida con el servidor X11.
	- Creamos la imagen que se usará como buffer de la ventana para el 
	  renderizado.
	- Inicializamos la variable `has_been_mouse_in_window` para controlar si el 
	  ratón ha entrado en la ventana.
	En caso de error:
	- Liberamos los componentes creados hasta el momento.
	- Mostramos un mensaje de error por `stderr`.
	- Devolvemos `false` para indicar que la inicialización falló.
*/
bool	init_mlx_components(t_mlx *mlx)
{
	if (mlx == NULL)
		return (false);
	mlx->mlx_var = mlx_init();
	if (mlx->mlx_var == NULL)
	{
		perror("X11 server disply conexion failed\n");
		return (false);
	}
	setup_window_wh(mlx);
	mlx->mlx_window = mlx_new_window(mlx->mlx_var, mlx->win_width,
			mlx->win_height, "cub3D");
	if (mlx->mlx_window == NULL)
	{
		perror("X Window creation failed\n");
		destroy_mlx_componets(0, mlx_destroy_window, mlx_destroy_display, mlx);
		return (false);
	}
	if (init_images_data(mlx) == false)
		return (false);
	mlx->has_been_mouse_in_window = false;
	return (true);
}

/*
	Inicializamos los datos de la imagen que se usará como buffer de la ventana:
	- Creamos la imagen con `mlx_new_image`, que reserva memoria para el buffer 
	  de la ventana.
	- Obtenemos la dirección de memoria del buffer de la imagen con 
	  `mlx_get_data_addr`, que nos permite acceder directamente a los píxeles 
	  de la imagen para modificarlos.
	En caso de error:
	- Liberamos los componentes creados hasta el momento.
	- Mostramos un mensaje de error por `stderr`.
	- Devolvemos `false` para indicar que la inicialización falló.
*/
bool	init_images_data(t_mlx *mlx)
{
	mlx->mlx_img = mlx_new_image(mlx->mlx_var, mlx->win_width, mlx->win_height);
	if (mlx->mlx_img == NULL)
	{
		perror("X Window image buffer creation failed\n");
		destroy_mlx_componets(0, mlx_destroy_window, mlx_destroy_display, mlx);
		return (false);
	}
	mlx->bit_map_address = mlx_get_data_addr(mlx->mlx_img,
			&(mlx->bits_per_pixel), &(mlx->line_length), &(mlx->endian));
	if (mlx->bit_map_address == NULL)
	{
		perror("X Window image components creation failed");
		destroy_mlx_componets(mlx_destroy_image, mlx_destroy_window,
			mlx_destroy_display, mlx);
		return (false);
	}
	return (true);
}

/*
	Cargamos las texturas del mapa:
	- Recorremos las rutas de las texturas del mapa y cargamos cada textura con 
	  la función `load_single_texture`.
	- Verificamos que cada textura se haya cargado correctamente. Si ocurre un 
	  error, liberamos las texturas que se hayan cargado hasta el momento.
	- Mostramos un mensaje en la consola indicando la ruta de cada textura que 
	  se carga correctamente.
	NOTA:
	- Usamos `n_textures` (número de texturas encontradas en el parseo) para 
	  saber cuántas texturas debemos cargar. Actualmente, este valor está 
	  configurado en 5 por defecto, ya que la conexión entre el parseo y la 
	  ejecución del motor aún no está implementada.
	- Comprobamos cuántas texturas hay desde `texture_paths`, que puede 
	  contener valores `NULL` en el caso de que no se hayan definido todas las 
	  rutas en el archivo de configuración.
	- El array de texturas no puede contener valores `NULL` porque es un array 
	  de estructuras, lo que permite un acceso más rápido en memoria.
*/
bool	load_textures(t_mlx *mlx)
{
	int	i;

	i = 0;
	while (i < mlx->map->n_textures)
	{
		//Esto se va a quitar cuand este hecho el mergero
		if (mlx->map->texture_paths[i] == NULL)
			break ;
		if (load_single_texture(mlx, &mlx->map->textures[i],
				mlx->map->texture_paths[i]) == false)
		{
			free_loaded_textures(mlx, i + 1, mlx->map->n_textures);
			return (false);
		}
		printf("Cargando textura desde la ruta: %s\n", mlx->map->texture_paths[i]);
		i++;
	}
	return (true);
}

/*
	Cargamos una textura individual desde un archivo XPM:
	- Cargamos la imagen XPM de la textura con `mlx_xpm_file_to_image`, que 
	  convierte el archivo XPM en una imagen compatible con la `mlx`.
	- Obtenemos los datos de la imagen con `mlx_get_data_addr`, que nos permite
	  acceder directamente a los píxeles de la textura para renderizarlos.
	En caso de error:
	- Mostramos un mensaje de error por `stderr`.
	- Devolvemos `false` para indicar que la carga de la textura falló.
	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico.
	- texture: Puntero a la estructura de la textura que se va a cargar.
	- path: Ruta del archivo XPM que contiene la textura.
	Retorno:
	- `true` si la textura se cargó correctamente.
	- `false` si ocurrió un error durante la carga.
*/
bool	load_single_texture(t_mlx *mlx, t_texture *texture, char *path)
{
	texture->img = mlx_xpm_file_to_image(mlx->mlx_var, path,
			&texture->width, &texture->height);
	if (texture->img == NULL)
	{
		perror("Xpm file to image fail");
		return (false);
	}
	texture->addr = mlx_get_data_addr(texture->img,
			&texture->bits_per_pixel, &texture->line_length, &texture->endian);
	if (texture->addr == NULL || texture->width <= 0 || texture->height <= 0)
	{
		perror("Texture fail while creating image buffer components");
		return (false);
	}
	return (true);
}

/*
	Inicializamos los hooks de la ventana y el loop del juego:

	- Creamos hooks para detectar eventos de teclado:
	  - `KeyPress`: Llama a la función `key_press` cuando se presiona una tecla
	  - `KeyRelease`: Llama a la función `key_release` cuando se suelta una 
		tecla.
	- Creamos un hook para detectar el cierre de la ventana:
	  - `DestroyNotify`: Llama a la función `close_game_manager` cuando se 
		cierra la ventana.
	- Creamos hooks para eventos del ratón:
	  - `ButtonPress`: Llama a la función `mouse_button_manager` cuando se 
		presiona un botón del ratón.
	  - `EnterNotify`: Llama a la función `mouse_init_manager` cuando el ratón 
		entra en la ventana.
	- Mostramos por consola la información de los controles del juego.
	- Creamos el loop principal del juego con `mlx_loop_hook`, que ejecuta la 
	  función `game_engine` en cada iteración del loop de la `mlx` o cuando se 
	  detecta un evento para el que hemos creado un hook.

	Esta función configura todos los eventos necesarios para interactuar con 
	el juego y asegura que el motor del juego se ejecute correctamente en cada 
	iteración del loop.
*/
void	start_hooks_and_game(t_mlx *mlx)
{
	mlx_hook(mlx->mlx_window, KeyPress, KeyPressMask,
		key_press, mlx);
	mlx_hook(mlx->mlx_window, KeyRelease, KeyReleaseMask,
		key_release, mlx);
	mlx_hook(mlx->mlx_window, DestroyNotify, StructureNotifyMask,
		close_game_manager, mlx);
	mlx_hook(mlx->mlx_window, ButtonPress, (1L << 2),
		mouse_button_manager, mlx);
	mlx_hook(mlx->mlx_window, EnterNotify, (1L << 4),
		mouse_init_manager, mlx);
	printf(CONTROLS_INFO);
	mlx_loop_hook(mlx->mlx_var, game_engine, mlx);
}
