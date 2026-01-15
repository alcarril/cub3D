/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/15 13:58:55 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

int	parse_texture(char *line, t_map *map)
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
	if (ft_strncmp(split[0], "NO", 2) == 0 && ft_strlen(split[0]) == 2)
		map->textures.north = path;
	else if (ft_strncmp(split[0], "SO", 2) == 0 && ft_strlen(split[0]) == 2)
		map->textures.south = path;
	else if (ft_strncmp(split[0], "WE", 2) == 0 && ft_strlen(split[0]) == 2)
		map->textures.west = path;
	else if (ft_strncmp(split[0], "EA", 2) == 0 && ft_strlen(split[0]) == 2)
		map->textures.east = path;
	else
	{
		printf("Error: Invalid texture identifier: %s\n", split[0]);
		return (free(path), free_split(split), 0);
	}
	return (free_split(split), 1);
}

int	is_texture_line(char *line)
{
	if (!line)
		return (0);
	if ((ft_strncmp(line, "NO ", 3) == 0)
		|| (ft_strncmp(line, "SO ", 3) == 0)
		|| (ft_strncmp(line, "WE ", 3) == 0)
		|| (ft_strncmp(line, "EA ", 3) == 0))
		return (1);
	return (0);
}
