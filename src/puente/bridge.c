/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 00:00:00 by carbon-m          #+#    #+#             */
/*   Updated: 2026/01/26 01:21:00 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"
#include <string.h>

int	extract_parse_data(t_carbon_map *cmap, t_parse_bridge *bridge)
{
	copy_textures(cmap, bridge);
	copy_colors_and_player(cmap, bridge);
	copy_map_grid(cmap, bridge);
	return (1);
}

static void	copy_grid_data(t_parse_bridge *bridge, t_map *alca_map)
{
	int		i;
	int		j;

	i = 0;
	while (i < (int)alca_map->max_rows)
	{
		j = 0;
		while (j < (int)alca_map->max_columns)
		{
			if (bridge->grid[i][j] == '1')
				alca_map->map_grids[i][j] = WALL;
			else if (bridge->grid[i][j] == '2')
				alca_map->map_grids[i][j] = BONUS_WALL;
			else if (is_floor_char(bridge->grid[i][j]))
				alca_map->map_grids[i][j] = 0;
			else
				alca_map->map_grids[i][j] = WALL;
			j++;
		}
		i++;
	}
}

int	pop_structs(t_parse_bridge *bridge,
		t_map *alca_map, t_player *alcarril_player)
{
	int			i;

	i = -1;
	while (++i < 5)
	{
		if (bridge->texture_paths[i][0] != '\0')
		{
			alca_map->texture_paths[i] = ft_strdup(bridge->texture_paths[i]);
			if (!alca_map->texture_paths[i])
				return (0);
		}
		else
			alca_map->texture_paths[i] = NULL;
	}
	populate_colors(bridge, alca_map);
	alca_map->max_columns = bridge->width;
	alca_map->max_rows = bridge->height;
	alca_map->map_grids = alloc_alca(bridge->height, bridge->width);
	if (!alca_map->map_grids)
		return (0);
	copy_grid_data(bridge, alca_map);
	alcarril_player->pos_x = bridge->player_x;
	alcarril_player->pos_y = bridge->player_y;
	alcarril_player->angle = bridge->player_angle;
	return (1);
}

int	integrate_parsing_to_game(char *map_file,
		t_map *alca_map, t_player *alcarril_player)
{
	t_carbon_map	carbon_map;
	t_parse_bridge	bridge;

	ft_bzero(&carbon_map, sizeof(t_carbon_map));
	ft_bzero(&bridge, sizeof(t_parse_bridge));
	if (parse_map_file(map_file, &carbon_map) != 1)
		return (cleanup_map(&carbon_map), 0);
	if (extract_parse_data(&carbon_map, &bridge) != 1)
	{
		cleanup_map(&carbon_map);
		cleanup_bridge(&bridge);
		return (0);
	}
	if (pop_structs(&bridge, alca_map,
			alcarril_player) != 1)
	{
		cleanup_map(&carbon_map);
		cleanup_bridge(&bridge);
		return (0);
	}
	cleanup_map(&carbon_map);
	cleanup_bridge(&bridge);
	return (1);
}
