/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 00:00:00 by carbon-m         #+#    #+#             */
/*   Updated: 2026/01/15 00:00:00 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

void	print_colors(t_carbon_map *map)
{
	printf("Floor: RGB(%d,%d,%d) = 0x%06X\n",
		map->floor.r, map->floor.g, map->floor.b, map->floor.rgb);
	printf("Ceiling: RGB(%d,%d,%d) = 0x%06X\n",
		map->ceiling.r, map->ceiling.g, map->ceiling.b, map->ceiling.rgb);
}

void	print_texture_paths(t_carbon_map *map)
{
	printf("Textures:\n");
	if (map->textures.north)
		printf("  North: %s\n", map->textures.north);
	else
		printf("  North: NULL\n");
	if (map->textures.south)
		printf("  South: %s\n", map->textures.south);
	else
		printf("  South: NULL\n");
	if (map->textures.west)
		printf("  West:  %s\n", map->textures.west);
	else
		printf("  West:  NULL\n");
	if (map->textures.east)
		printf("  East:  %s\n", map->textures.east);
	else
		printf("  East:  NULL\n");
}

void	print_map_grid_lines(t_carbon_map *map)
{
	int	i;

	printf("Map grid:\n");
	i = 0;
	while (i < map->height)
	{
		printf("%2d: |%s|\n", i, map->map_grids[i]);
		i++;
	}
	printf("================\n");
}

void	print_player_info(t_carbon_player *player)
{
	if (!player)
		return ;
	printf("=== PLAYER INFO ===\n");
	printf("Position: (%.1f, %.1f)\n", player->x, player->y);
	printf("Direction: %c\n", player->direction);
	printf("Angle: %.2f radians (%.1f degrees)\n",
		player->angle, player->angle * 180.0 / 3.14159265359);
	printf("===================\n");
}
