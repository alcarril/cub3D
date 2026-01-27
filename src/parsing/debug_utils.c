/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 00:00:00 by carbon-m         #+#    #+#             */
/*   Updated: 2025/12/29 00:00:00 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

void	print_map_dimensions(t_carbon_map *map)
{
	printf("Dimensions: %dx%d\n", map->width, map->height);
	printf("Player: (%.1f, %.1f) facing %c (angle: %.2f)\n",
		map->player.x, map->player.y, map->player.direction,
		map->player.angle);
}

void	print_map_info(t_carbon_map *map)
{
	if (!map)
		return ;
	printf("=== MAP INFO ===\n");
	print_map_dimensions(map);
	print_colors(map);
	print_texture_paths(map);
	print_map_grid_lines(map);
}

void	print_parse_progress(char *step, int success)
{
	if (success)
		printf("%s completed successfully\n", step);
	else
		printf("%s failed\n", step);
}

void	print_texture_info(t_carbon_texture *textures)
{
	if (!textures)
		return ;
	printf("=== TEXTURE INFO ===\n");
	if (textures->north)
		printf("North: %s\n", textures->north);
	else
		printf("North: NOT SET\n");
	if (textures->south)
		printf("South: %s\n", textures->south);
	else
		printf("South: NOT SET\n");
	if (textures->west)
		printf("West:  %s\n", textures->west);
	else
		printf("West:  NOT SET\n");
	if (textures->east)
		printf("East:  %s\n", textures->east);
	else
		printf("East:  NOT SET\n");
	if (textures->bonus)
		printf("Bonus: %s\n", textures->bonus);
	else
		printf("Bonus: NOT SET\n");
	printf("====================\n");
}

void	print_color_info(t_carbon_color *floor, t_carbon_color *ceiling)
{
	if (!floor || !ceiling)
		return ;
	printf("=== COLOR INFO ===\n");
	printf("Floor:   RGB(%d,%d,%d) = 0x%06X\n",
		floor->r, floor->g, floor->b, floor->rgb);
	printf("Ceiling: RGB(%d,%d,%d) = 0x%06X\n",
		ceiling->r, ceiling->g, ceiling->b, ceiling->rgb);
	printf("==================\n");
}
