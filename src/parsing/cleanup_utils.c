/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:00:00 by carbon-m          #+#    #+#             */
/*   Updated: 2026/01/26 00:46:27 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

void	cleanup_map(t_carbon_map *map)
{
	if (!map)
		return ;
	if (map->textures.north)
		free(map->textures.north);
	if (map->textures.south)
		free(map->textures.south);
	if (map->textures.west)
		free(map->textures.west);
	if (map->textures.east)
		free(map->textures.east);
	if (map->textures.bonus)
		free(map->textures.bonus);
	if (map->map_grids)
		free_map_grid(map->map_grids, map->height);
}

int	cleanup_parse(t_carbon_parse *data, int success)
{
	if (!data)
		return (success);
	if (data->fd >= 0)
		close(data->fd);
	if (data->file_content)
		free_split(data->file_content);
	if (data->filename)
		free(data->filename);
	return (success);
}

void	free_map_grid(char **grid, int height)
{
	int	i;

	if (!grid)
		return ;
	i = 0;
	while (i < height)
	{
		if (grid[i])
			free(grid[i]);
		i++;
	}
	free(grid);
}
