/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/03/17 11:39:56 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

int	parse_rgb_values(char **rgb_split, t_carbon_color *color)
{
	if (!rgb_split || !rgb_split[0] || !rgb_split[1]
		|| !rgb_split[2] || rgb_split[3])
		return (0);
	color->r = ft_atoi(rgb_split[0]);
	color->g = ft_atoi(rgb_split[1]);
	color->b = ft_atoi(rgb_split[2]);
	if (!validate_rgb_values(color))
	{
		printf("Error: RGB values out of range (0-255): %d,%d,%d\n",
			color->r, color->g, color->b);
		return (0);
	}
	color->rgb = (color->r << 16) | (color->g << 8) | color->b;
	return (1);
}

int	get_color_pointer(char *identifier, t_carbon_map *map,
	t_carbon_color **color)
{
	if (ft_strncmp(identifier, "F", 1) == 0 && ft_strlen(identifier) == 1)
		*color = &map->floor;
	else if (ft_strncmp(identifier, "C", 1) == 0 && ft_strlen(identifier) == 1)
		*color = &map->ceiling;
	else
	{
		printf("Error: Invalid color identifier: %s\n", identifier);
		return (0);
	}
	return (1);
}

static char	*skip_to_rgb(char *line)
{
	while (*line == ' ')
		line++;
	while (*line && *line != ' ')
		line++;
	while (*line == ' ')
		line++;
	return (line);
}

static char	*build_compact_str(char *start)
{
	char	*result;
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (start[++i])
		if (start[i] != ' ')
			j++;
	result = malloc(j + 1);
	if (!result)
		return (NULL);
	i = -1;
	j = 0;
	while (start[++i])
		if (start[i] != ' ')
			result[j++] = start[i];
	result[j] = '\0';
	return (result);
}

static int	get_color_from_line(char *line, t_carbon_map *map,
	t_carbon_color **color)
{
	char	**split;
	char	*trimmed;

	trimmed = line;
	while (*trimmed == ' ')
		trimmed++;
	split = ft_split(trimmed, ' ');
	if (!split || !split[0])
		return (free_split(split), 0);
	if (!get_color_pointer(split[0], map, color))
		return (free_split(split), 0);
	free_split(split);
	return (1);
}

int	parse_color(char *line, t_carbon_map *map)
{
	char			**rgb_split;
	char			*rgb_str;
	t_carbon_color	*color;
	int				result;

	if (!line || !map)
		return (0);
	if (!get_color_from_line(line, map, &color))
		return (0);
	rgb_str = build_compact_str(skip_to_rgb(line));
	if (!rgb_str)
		return (0);
	rgb_split = ft_split(rgb_str, ',');
	free(rgb_str);
	if (!rgb_split)
		return (printf("Error: Invalid RGB format\n"), 0);
	result = parse_rgb_values(rgb_split, color);
	free_split(rgb_split);
	return (result);
}

int	is_color_line(char *line)
{
	if (!line)
		return (0);
	while (*line == ' ')
		line++;
	if ((ft_strncmp(line, "F ", 2) == 0)
		|| (ft_strncmp(line, "C ", 2) == 0))
		return (1);
	return (0);
}

int	validate_rgb_values(t_carbon_color *color)
{
	if (!color)
		return (0);
	if (color->r < 0 || color->r > 255
		|| color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
		return (0);
	return (1);
}
