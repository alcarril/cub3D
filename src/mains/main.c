/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:12:38 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/10 20:32:45 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

//main con mapa antes de tener hecho el parseo
int main (void)
{
	t_mlx	mlx;
	t_map	map;
	t_player	player;
	t_frame	frame;
	
	map.max_columns = MAX_COLUMS;
	map.max_rows = MAX_ROWS;
	char map_data[15][20] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};
	memcpy(map.map_grids, map_data, sizeof(map_data));
	mlx.map = &map;//quizas osbre si no hay que escalar
	map.floor_color[0] = 100;
	map.floor_color[1] = 100;
	map.floor_color[2] = 100;
	map.ceiling_color[0] = 200;
	map.ceiling_color[1] = 200;
	map.ceiling_color[2] = 200;
	char *texture_paths[4] = {
		"/home/alejandro/42Universe/42Proyects/cube3D/assets/textures/other/test/south.xpm", // NORTE -> SUR
		"/home/alejandro/42Universe/42Proyects/cube3D/assets/textures/other/test/north.xpm", // SUR -> NORTE
		"/home/alejandro/42Universe/42Proyects/cube3D/assets/textures/other/test/east.xpm",  // OESTE -> ESTE
		"/home/alejandro/42Universe/42Proyects/cube3D/assets/textures/other/test/west.xpm"  // ESTE -> OESTE
	};
	int i = 0;
	while (i < 4)
	{
		map.texture_paths[i] = texture_paths[i];
		i++;
	}
	//LO de antes de qui es la infomacion que tienes que cargar tu
	if (init_mlx_components(&mlx) == false)
		return (1);
	if (setup_game(&mlx, &player, &map, &frame) == false)
		return (1);
	start_hooks_and_game(&mlx);
	mlx_loop(mlx.mlx_var);
	return(0);
}