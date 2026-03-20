/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/03/19 16:17:50 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

static char	*skip_spaces(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static int	contains_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}

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
	else
	{
		printf("Error: Invalid texture identifier: %s\n", identifier);
		return (0);
	}
	return (1);
}

int	parse_texture(char *line, t_carbon_map *map)
{
	char	identifier[3];
	char	*cursor;
	char	*path;

	if (!line || !map)
		return (0);
	cursor = skip_spaces(line);
	if ((cursor[0] == '\0' || cursor[1] == '\0')
		|| (cursor[2] != ' ' && cursor[2] != '\t'))
		return (0);
	identifier[0] = cursor[0];
	identifier[1] = cursor[1];
	identifier[2] = '\0';
	cursor = skip_spaces(cursor + 2);
	path = ft_strtrim(cursor, " \t");
	if (!path || path[0] == '\0' || contains_space(path))
		return (free(path), 0);
	if (!assign_texture_to_map(identifier, path, map))
		return (free(path), 0);
	return (1);
}

int	is_texture_line(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	while (line[i] == ' ')
		i++;
	if ((ft_strncmp(&line[i], "NO ", 3) == 0)
		|| (ft_strncmp(&line[i], "SO ", 3) == 0)
		|| (ft_strncmp(&line[i], "WE ", 3) == 0)
		|| (ft_strncmp(&line[i], "EA ", 3) == 0)
		|| (ft_strncmp(&line[i], "BO ", 3) == 0))
		return (1);
	return (0);
}
