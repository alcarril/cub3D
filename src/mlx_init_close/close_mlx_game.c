/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_mlx_game.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:53:10 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/25 14:41:43 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Cuando se hace mlx_init se esta creado la conexion con el servidor
	x11 (xorg), a esta conexion se le llama display y se esta creacdo
	una estructura dentro a la que apunta mlx_var que almacena
	lascomponentes o dependencias necesarias para la concexion.
	- mlx_destroy display libera en puntero estatico que tiene la mlx
	  var dentro y evita el still reachable.
	- free (mlx->mlx_var libera la estrucutra para la que se ha
	  reservado memoria)
	Cuando se hace el disply con el servidor x11 se puede crear la
	ventana y despues se pueden crear las imagenes asiccionadas a la
	ventana el orden no se puede invertir Al igual pasa para la
	liberacion primero se tiene que liberar la imagen porque esta aso
	ciada a laventana depende de ella. Si intentamos liberar primero la
	ventana y luego la imagen haremos un segfault porque el puntero que
	almacena la imagen esta dentro de la estrcu tura de la ventana por
	lo que estatemos haciendi un acceso de memria a meria ya liberada
	(segfault)
	- mlx_destroy_imag libera la informacion de la imahgen creada para
	  buferizar en la ventana.
	- mlx_destroy window sirve para liberar la memoria de la ventana
*/
void	destroy_mlx_componets(int (*f)(), int (*g)(), int (*t)(),
	t_mlx *mlx)
{
	if (f != NULL)
		f(mlx->mlx_var, mlx->mlx_img);
	if (g != NULL)
		g(mlx->mlx_var, mlx->mlx_window);
	if (t != NULL)
		t(mlx->mlx_var);
	free(mlx->mlx_var);
}

/*
	Funcion que libera las texturas cargadas dentro del array de texturas
	del mapa. Se le pasa el numero de texturas cargadas hasta el momento para
	liberar en caso de error solo las que se han cargado.
*/
void	free_loaded_textures(t_mlx *mlx, int loaded, int max_textures)
{
	int i;
	int max;
	
	if (loaded < max_textures)
		max = loaded;
	else
		max = max_textures;
	i = 0;
	while (i < max)
	{
		if (mlx->map->textures[i].img)
			mlx_destroy_image(mlx->mlx_var, mlx->map->textures[i].img);
		i++;
	}
}

/*
	Funcion que se encarga de cerrar el juego limpiando todos los recursos
	- Libera las texturas cargadas
	- Libera la imagen usada como buffer de la ventana
	- Libera la ventana
	- Libera la conexion con el servidor x11
	- Libera la estrcutura mlx_var que es donde se almacenan todos los componentes
	  de la mlx
	- Sale del programa con exit(1)
	- Se la llama desde los eventos del juego cunado ya esta en el loop de renderizado
	para abortar y salir limpiamente del mismo
	Se usa ando el loop de renderizacion de la mlx ya esta en marcha y se quiere
	cerrar el juego de manera limpia.
*/
int	close_game_manager(t_mlx *mlx)
{
	mlx_mouse_show(mlx->mlx_var, mlx->mlx_window);
	free_loaded_textures(mlx, 5, mlx->map->n_textures);
	mlx_destroy_image(mlx->mlx_var,
		mlx->mlx_img);
	mlx_destroy_window(mlx->mlx_var,
		mlx->mlx_window);
	mlx_destroy_display(mlx->mlx_var);
	free(mlx->mlx_var);
	if (mlx->frame->fov_distances != NULL)
		free(mlx->frame->fov_distances);
	close(mlx->log_fd);
	// free_map_data(mlx);//
	exit(1);
}

/*
	Para cerrar y liberar los componentes de la api de mlx y los componentes del
	juego es como close manager pero no sale con exit.
*/
void	free_game(t_mlx *mlx)
{
	mlx_mouse_show(mlx->mlx_var, mlx->mlx_window);
	free_loaded_textures(mlx, 5, mlx->map->n_textures);
	mlx_destroy_image(mlx->mlx_var,
		mlx->mlx_img);
	mlx_destroy_window(mlx->mlx_var,
		mlx->mlx_window);
	mlx_destroy_display(mlx->mlx_var);
	free(mlx->mlx_var);
	if (mlx->frame->fov_distances != NULL)
		free(mlx->frame->fov_distances);
	free_map_data(mlx);
	close(mlx->log_fd);
}

/*
	Funcion para liberar los datos que hay en mi estrcutura reservados por el parser
	(datos del mapa):
	- Libera las ruttas de la texturas del mapa por si no siempre 4
	- Libera estrxuturas para la limagen de cada textura alocadas dinamicamente por si hay mas de 4
	- Libera la matriz del mapa (puntero doble)
	NOTAS_: MAp textures comenda porque demomdento no es el puntero y el compilador se queja
*/
void	free_map_data(t_mlx *mlx)
{
	unsigned int i;

	i = 0;
	while (mlx->map->texture_paths[i] == NULL)
	{
		free(mlx->map->texture_paths[i]);
		i++;
	}
	// free(mlx->map->textures);
	i = 0;
	while (i < mlx->map->max_rows)
	{
		free(mlx->map->map_grids[i]);
		i++;
	}
}
