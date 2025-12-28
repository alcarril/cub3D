/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_close.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:53:10 by alejandro         #+#    #+#             */
/*   Updated: 2025/12/28 11:34:44 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

char	init_mlx_components(t_mlx *mlx)
{
	if (!mlx)
		return (0);
	mlx->mlx_var = mlx_init();
	if (!mlx->mlx_var)
		return (0);
	mlx->mlx_window = mlx_new_window(mlx->mlx_var, WIDTH, HEIGHT, "cube3D");
	if (!mlx->mlx_window)
		return (destroy_mlx_componets(0, mlx_destroy_window, 0, mlx), 0);
	mlx->mlx_img = mlx_new_image(mlx->mlx_var, HEIGHT, WIDTH);
	if (!mlx->mlx_img)
		return (destroy_mlx_componets(0, mlx_destroy_window,
				mlx_destroy_display, mlx), 0);
	mlx->bit_map_address = mlx_get_data_addr(mlx->mlx_img,
			&(mlx->bits_per_pixel), &(mlx->line_length), &(mlx->endian));
	if (!mlx->bit_map_address)
		return (destroy_mlx_componets(mlx_destroy_image, mlx_destroy_window,
				mlx_destroy_display, mlx), 0);
	mlx->bit_map_address_copy = mlx->bit_map_address;//porque
	// create_hooks(mlx);
	return (1);
}

void	create_hooks(t_mlx *mlx)
{
	mlx_hook(mlx->mlx_window, KeyPress, KeyPressMask,
		key_press, mlx);
	mlx_hook(mlx->mlx_window, KeyRelease, KeyReleaseMask,
		key_release, mlx);
	mlx_hook(mlx->mlx_window, DestroyNotify, StructureNotifyMask,
		close_handler, mlx);
	mlx_loop_hook(mlx->mlx_var, render_frame2D, mlx);
	printf("llega\n");
}

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

//init first player pos inisde map and init orientation inside map
//hay que cambiar cosas
void setup_player(t_mlx *mlx)
{
	unsigned int	middle_x;
	unsigned int	middle_y;
	
	middle_x = mlx->map->max_columns / 2;
	middle_y = mlx->map->max_rows / 2;
	if (mlx->map->map_grids[middle_x][middle_y] == FLOOR)
	{
		mlx->player->origin[0] = middle_x;
		mlx->player->pos_x = middle_x;
		mlx->player->origin[1] = middle_y;
		mlx->player->pos_y = middle_y;
	}
	// else
	// {
	// 	//buscar otra posicion libre (flood fill o algo asi)
	// }
	mlx->player->raydir_x = 0;//
	mlx->player->raydir_y = 0;//
	mlx->player->angle = 90.0f;//
	mlx->player->speed = 0.04f;//
	mlx->player->move_down = false;
	mlx->player->move_up = false;
	mlx->player->move_right = false;
	mlx->player->move_left = false;
	mlx->player->r_clockwise = false;
	mlx->player->r_counterclockwise = false;
	printf("llega2\n");

}
