/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge_copy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:00:00 by carbon-m          #+#    #+#             */
/*   Updated: 2026/01/26 01:15:18 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

void	copy_textures(t_carbon_map *cmap, t_parse_bridge *bridge)
{
	if (cmap->textures.north)
		ft_strlcpy(bridge->texture_paths[0], cmap->textures.north, 256);
	else
		bridge->texture_paths[0][0] = '\0';
	if (cmap->textures.south)
		ft_strlcpy(bridge->texture_paths[1], cmap->textures.south, 256);
	else
		bridge->texture_paths[1][0] = '\0';
	if (cmap->textures.west)
		ft_strlcpy(bridge->texture_paths[2], cmap->textures.west, 256);
	else
		bridge->texture_paths[2][0] = '\0';
	if (cmap->textures.east)
		ft_strlcpy(bridge->texture_paths[3], cmap->textures.east, 256);
	else
		bridge->texture_paths[3][0] = '\0';
	if (cmap->textures.bonus)
		ft_strlcpy(bridge->texture_paths[4], cmap->textures.bonus, 256);
	else
		bridge->texture_paths[4][0] = '\0';
}

void	copy_colors_and_player(t_carbon_map *cmap, t_parse_bridge *bridge)
{
	bridge->floor_rgb[0] = cmap->floor.r;
	bridge->floor_rgb[1] = cmap->floor.g;
	bridge->floor_rgb[2] = cmap->floor.b;
	bridge->ceiling_rgb[0] = cmap->ceiling.r;
	bridge->ceiling_rgb[1] = cmap->ceiling.g;
	bridge->ceiling_rgb[2] = cmap->ceiling.b;
	bridge->player_x = cmap->player.x;
	bridge->player_y = cmap->player.y;
	bridge->player_angle = cmap->player.angle;
	bridge->width = cmap->width;
	bridge->height = cmap->height;
}

void	copy_map_grid(t_carbon_map *cmap, t_parse_bridge *bridge)
{
	int		i;
	int		j;

	bridge->grid = allocate_map_grid(cmap->height, cmap->width);
	if (!bridge->grid)
		return ;
	i = 0;
	while (i < cmap->height)
	{
		j = 0;
		while (j < cmap->width)
		{
			bridge->grid[i][j] = cmap->map_grids[i][j];
			j++;
		}
		i++;
	}
}

void	populate_colors(t_parse_bridge *bridge, t_map *alca_map)
{
	alca_map->floor_color[0] = bridge->floor_rgb[0];
	alca_map->floor_color[1] = bridge->floor_rgb[1];
	alca_map->floor_color[2] = bridge->floor_rgb[2];
	alca_map->ceiling_color[0] = bridge->ceiling_rgb[0];
	alca_map->ceiling_color[1] = bridge->ceiling_rgb[1];
	alca_map->ceiling_color[2] = bridge->ceiling_rgb[2];
	alca_map->floor_color_hex = rgb_to_hex(bridge->floor_rgb[0],
			bridge->floor_rgb[1], bridge->floor_rgb[2]);
	alca_map->ceiling_color_hex = rgb_to_hex(bridge->ceiling_rgb[0],
			bridge->ceiling_rgb[1], bridge->ceiling_rgb[2]);
}
