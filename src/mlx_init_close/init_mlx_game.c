/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx_game.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 20:07:49 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 22:09:20 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Inicializamos los componentes de la mlx:
	- Inicializamos la conexion con el servidor x11 (xorg) con mlx_init
	- Creamos la ventana con las dimensiones en funcion del numero de filas y columnas
	  del mapa y de una escala definida (WIN_SCALE)
	- Creamos la imagen que se va a usar como buffer de la ventana
	- En caso de error liberamos los componentes creados hasta el momento, devolevemos
	  false y mostramos un mensaje de error por stderr y delvolvemos false
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
	mlx->win_height = mlx->map->max_rows * WIN_SCALE * 4;
	mlx->win_width = mlx->map->max_columns * WIN_SCALE * 3;
	mlx->mlx_window = mlx_new_window(mlx->mlx_var, mlx->win_width, mlx->win_height, "cub3D");
	if (mlx->mlx_window == NULL)
	{
		perror("X Window creation failed\n");
		destroy_mlx_componets(0, mlx_destroy_window, mlx_destroy_display, mlx);
		return (false);
	}
	if (init_images_data(mlx) == false)
		return (false);
	return (true);
}

/*
	Inicializamos los datos de la imagen que se va a usar como buffer de la ventana:
	- Creamos la imagen con mlx_new_image
	- En caso de error liberamos los componentes creados hasta el momento, mostramos
	  un mensaje de error por stderr y devolvemos false
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
	- Recorremos las rutas de las texturas del mapa y cargamos cada textura
	  con la funcion load_single_texture
	- En caso de error liberamos las texturas que se hayan cargado hasta el momento,
	  mostramos un mensaje de error por stderr y devolvemos false
*/
bool	load_textures(t_mlx *mlx)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (load_single_texture(mlx, &mlx->map->textures[i],
			mlx->map->texture_paths[i]) == false)
		{
			free_loaded_textures(mlx, i + 1);
			return(false);		
		}
		i++;
	}
	return (true);
}

/*
	Cargamos una textura individual:
	- Cargamos la imagen xpm de la textura con mlx_xpm_file_to_image
	- Obtenemos los datos de la imagen con mlx_get_data_addr
	- En caso de error mostramos un mensaje de error por stderr y devolvemos false
*/
bool	load_single_texture(t_mlx *mlx, t_texture *texture, char *path)
{	
	texture->img = mlx_xpm_file_to_image(mlx->mlx_var, path,
		&texture->width, &texture->height);
	if (texture->img == NULL)
	{
		perror("Xpm file to image fail\n");
		return (false);
	}
	texture->addr = mlx_get_data_addr(texture->img,
		&texture->bits_per_pixel, &texture->line_length, &texture->endian);
	if (texture->addr == NULL || texture->width <= 0 || texture->height <= 0)
	{
		perror("Texture fail while creating image buffer components\n");
		return (false);
	}
	return (true);
}

/*
	Inicializamos los hooks de la ventana y el loop del juego:
	- Cread los hooks para detectar loe eventos de teclado, cunado esto pasa 
	  hacemos un trigger de las funciones key_press y key_release
	- Creamos el hook para detectar el cierre de la ventana, cuando esto pasa
	  hacemos un trigger de la funcion close_handler
	- Creamos los hooks para detectar los eventos del raton, cuando esto pasa
	  hacemos un trigger de las funciones mouse_button_manager y mouse_init_manager
	- Mostramos por consola la informacion de los controles del juego
	- Creamos el loop hook que se encarga de ejecutar el motor del juego y se
	  ejecuta en cada iteracion del loop de la mlx o cuando se detecta un evento
	  para el que hemos creado un hook
*/
void	start_hooks_and_game(t_mlx *mlx)
{
	mlx_hook(mlx->mlx_window, KeyPress, KeyPressMask,
		key_press, mlx);
	mlx_hook(mlx->mlx_window, KeyRelease, KeyReleaseMask,
		key_release, mlx);
	mlx_hook(mlx->mlx_window, DestroyNotify, StructureNotifyMask,
		close_game_manager, mlx);
	mlx_hook(mlx->mlx_window, ButtonPress, (1L<<2), mouse_button_manager, mlx);
	mlx_hook(mlx->mlx_window, EnterNotify, (1L<<4), mouse_init_manager, mlx);
	printf(CONTROLS_INFO);
	mlx_loop_hook(mlx->mlx_var, game_engine, mlx);
}
