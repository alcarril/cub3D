/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 00:00:00 by carbon-m          #+#    #+#             */
/*   Updated: 2026/03/19 18:38:01 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

int	is_numeric_string(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	parse_rgb_values(char **rgb_split, t_carbon_color *color)
{
	if (!rgb_split || !rgb_split[0] || !rgb_split[1]
		|| !rgb_split[2] || rgb_split[3])
		return (0);
	if (!is_numeric_string(rgb_split[0]) || !is_numeric_string(rgb_split[1])
		|| !is_numeric_string(rgb_split[2]))
	{
		printf("Error: RGB values must be numeric: %s,%s,%s\n",
			rgb_split[0], rgb_split[1], rgb_split[2]);
		return (0);
	}
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

int	parse_color_header(char *line, t_carbon_map *map,
	char **rgb_start, t_carbon_color **color)
{
	char	*cursor;
	char	identifier[2];

	cursor = line;
	while (*cursor == ' ' || *cursor == '\t')
		cursor++;
	if ((cursor[0] != 'F' && cursor[0] != 'C')
		|| (cursor[1] != ' ' && cursor[1] != '\t'))
		return (0);
	identifier[0] = cursor[0];
	identifier[1] = '\0';
	if (!get_color_pointer(identifier, map, color))
		return (0);
	*rgb_start = cursor + 1;
	while (**rgb_start == ' ' || **rgb_start == '\t')
		(*rgb_start)++;
	return (1);
}
