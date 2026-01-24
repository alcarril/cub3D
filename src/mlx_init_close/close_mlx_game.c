/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_mlx_game.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:53:10 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/24 00:50:14 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Cuando se hace `mlx_init`, se está creando la conexión con el servidor X11 
	(Xorg). A esta conexión se le llama display, y se está creando una 
	estructura dentro a la que apunta `mlx_var`, que almacena los componentes o
	dependencias necesarias para la conexión.
	- `mlx_destroy_display` libera un puntero estático que tiene la `mlx_var` 
	  dentro y evita el "still reachable".
	- `free(mlx->mlx_var)` libera la estructura para la que se ha reservado 
	  memoria.
	Cuando se establece el display con el servidor X11, se puede crear la 
	ventana y, después, las imágenes asociadas a la ventana. El orden no se 
	puede invertir. De igual forma, para la liberación, primero se tiene que 
	liberar la imagen porque está asociada a la ventana y depende de ella. Si
	se intenta liberar primero la ventana y luego la imagen, se producirá un 
	segfault porque el puntero que almacena la imagen está dentro de la 
	estructura de la ventana. Esto provocará un acceso a memoria ya liberada 
	(segfault).
	- `mlx_destroy_image` libera la información de la imagen creada para 
	  bufferizar en la ventana.
	- `mlx_destroy_window` libera la memoria de la ventana.
	Detalles:
	- Primero libera la imagen (`mlx_img`) si la función `f` no es NULL.
	- Luego libera la ventana (`mlx_window`) si la función `g` no es NULL.
	- Finalmente, libera la conexión con el servidor X11 (`mlx_var`) si la 
	  función `t` no es NULL.
	- Libera la memoria reservada para la estructura `mlx_var`.
	Parámetros:
	- f: Puntero a la función para liberar la imagen.
	- g: Puntero a la función para liberar la ventana.
	- t: Puntero a la función para liberar la conexión con el servidor X11.
	- mlx: Puntero a la estructura principal del motor gráfico.
	Esta función asegura que los recursos se liberen en el orden correcto para
	evitar errores como segfaults.
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
	Libera las texturas cargadas dentro del array de texturas del mapa.
	Detalles:
	- Se libera cada textura cargada en el array `textures` de la estructura 
	  del mapa.
	- Se utiliza el parámetro `loaded` para liberar únicamente las texturas 
	  que ya han sido cargadas, en caso de que ocurra un error antes de cargar
	  todas las texturas esperadas.
	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico.
	- loaded: Número de texturas cargadas hasta el momento.
	- max_textures: Número máximo de texturas que se espera cargar.
	Esta función es útil para manejar errores durante la carga de texturas,
	asegurando que solo se liberen las texturas que ya han sido cargadas.
*/
void	free_loaded_textures(t_mlx *mlx, int loaded, int max_textures)
{
	int	i;
	int	max;

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
	Cierra el juego limpiando todos los recursos y saliendo del programa.

	Detalles:
	- Muestra el cursor del mouse si estaba oculto.
	- Libera las texturas cargadas.
	- Libera la imagen usada como buffer de la ventana.
	- Libera la ventana.
	- Libera la conexión con el servidor X11.
	- Libera la estructura `mlx_var`, que almacena todos los componentes de la
	  `mlx`.
	- Libera la memoria reservada para las distancias del FOV si están 
	  inicializadas.
	- Cierra el archivo de log.
	- Sale del programa con `exit(1)`.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico.

	Esta función se utiliza cuando el loop de renderización de la `mlx` está en
	marcha y se desea cerrar el juego de manera limpia.
	Nota:
	 Actualmente, la liberación de los datos del mapa está comentada.
	 Si se desea liberar esa memoria, se debe descomentar la línea.
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
	Libera todos los recursos del juego y los componentes de la API de `mlx` 
	sin salir del programa.

	Detalles:
	- Muestra el cursor del mouse si estaba oculto.
	- Libera las texturas cargadas.
	- Libera la imagen usada como buffer de la ventana.
	- Libera la ventana.
	- Libera la conexión con el servidor X11.
	- Libera la estructura `mlx_var`, que almacena todos los componentes de la 
	  `mlx`.
	- Libera la memoria reservada para las distancias del FOV si están 
	  inicializadas.
	- Libera los datos del mapa cargados por el parser.
	- Cierra el archivo de log.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico.

	Esta función es similar a `close_game_manager`, pero no utiliza `exit` para 
	salir del programa.
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
	Libera los datos del mapa reservados por el parser.

	Detalles:
	- Libera las rutas de las texturas del mapa almacenadas en el array 
	  `texture_paths`. Estas rutas son cadenas de texto alocadas dinámicamente.
	- Libera la matriz del mapa (`map_grids`), que es un puntero doble que 
	  contiene las celdas del mapa.
	- Libera las estructuras de las imágenes de las texturas si están 
	  alocadas dinámicamente.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico.

	Notas:
	- Actualmente, la liberación de `map->textures` está comentada porque no 
	  es un puntero dinámico y el compilador podría generar advertencias. Esto 
	  debe revisarse si en el futuro se cambia la implementación.
*/
void	free_map_data(t_mlx *mlx)
{
	unsigned int	i;

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
