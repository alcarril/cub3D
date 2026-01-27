/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 00:00:00 by carbon-m          #+#    #+#             */
/*   Updated: 2026/01/22 13:53:14 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

static int	error_message(char *msg)
{
	printf("Error\n%s\n", msg);
	return (1);
}

int	main(int argc, char **argv)
{
	t_mlx		mlx;
	t_map		map;
	t_player	player;
	t_frame		frame;

	ft_memset(&map, 0, sizeof(t_map));
	ft_memset(&player, 0, sizeof(t_player));
	if (argc != 2)
		return (error_message("Usage: ./cub3d <map.cub>"));
	if (integrate_parsing_to_game(argv[1], &map, &player) != 1)
		return (error_message("Error loading map"));
	mlx.map = &map;
	if (init_mlx_components(&mlx) == false)
		return (1);
	if (setup_game(&mlx, &player, &map, &frame) == false)
		return (1);
	start_hooks_and_game(&mlx);
	mlx_loop(mlx.mlx_var);
	return (0);
}
