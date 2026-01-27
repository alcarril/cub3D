/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/26 00:21:47 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

static int	assign_texture_to_map(char *identifier, char *path,
	t_carbon_map *map)
{
	if (ft_strncmp(identifier, "NO", 2) == 0 && ft_strlen(identifier) == 2)
		map->textures.north = path;
	else if (ft_strncmp(identifier, "SO", 2) == 0
		&& ft_strlen(identifier) == 2)
		map->textures.south = path;
	else if (ft_strncmp(identifier, "WE", 2) == 0
		&& ft_strlen(identifier) == 2)
		map->textures.west = path;
	else if (ft_strncmp(identifier, "EA", 2) == 0
		&& ft_strlen(identifier) == 2)
		map->textures.east = path;
	else if (ft_strncmp(identifier, "BO", 2) == 0
		&& ft_strlen(identifier) == 2)
		map->textures.bonus = path;
	else
	{
		printf("Error: Invalid texture identifier: %s\n", identifier);
		return (0);
	}
	return (1);
}

int	parse_texture(char *line, t_carbon_map *map)
{
	char	**split;
	char	*path;

	if (!line || !map)
		return (0);
	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || split[2])
		return (free_split(split), 0);
	path = ft_strdup(split[1]);
	if (!path)
		return (free_split(split), 0);
	if (!assign_texture_to_map(split[0], path, map))
		return (free(path), free_split(split), 0);
	return (free_split(split), 1);
}

int	is_texture_line(char *line)
{
	if (!line)
		return (0);
	if ((ft_strncmp(line, "NO ", 3) == 0)
		|| (ft_strncmp(line, "SO ", 3) == 0)
		|| (ft_strncmp(line, "WE ", 3) == 0)
		|| (ft_strncmp(line, "EA ", 3) == 0)
		|| (ft_strncmp(line, "BO ", 3) == 0))
		return (1);
	return (0);
}
