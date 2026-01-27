/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiance_configs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:14:29 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 20:54:55 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Configuracion de la ambiance cementery
	- Fog color: Dark medium gray
	- Max distance: Medium distance
	- Fog multipliers: 1.7 for walls, floor, and ceiling (mucha niebla)
	- Shader k-factors: 8.0 for walls, 1.0 for floor and ceiling
	- Shader multipliers: 1.0 for walls, floor, and ceiling
	- Ambiance type: CEMENTERY
*/
void	config_ambiance_cementery(t_ambiance *amb)
{
	amb->fog_color_walls = FOG_MEDIO_OSCURO;
	amb->fog_color_fc = FOG_MEDIO_OSCURO;
	amb->v_max_distance_map = MEDIUM_DISTANCE;
	amb->vinv_max_diatance = 1.0f / amb->v_max_distance_map;
	amb->mult_fog_walls = 1.7f;
	amb->mult_fog_floor = 1.7f;
	amb->mult_fog_ceiling = 1.7f;
	amb->k_factor_floor = 1.0f;
	amb->k_factor_ceiling = 1.0f;
	amb->k_factor_walls = 8.0f;
	amb->mult_shader_walls = 1.0f;
	amb->mult_shader_floor = 1.0f;
	amb->mult_shader_ceiling = 1.0f;
	amb->ambiance = CEMENTERY;
}

/*
	Configuracion de la ambiance asturias
	- Fog color: Light medium gray
	- Max distance: 70% of map max distance
	- Fog multipliers: 1.7 for walls and floor, 1.0 for ceiling
	- Shader k-factors: 8.0 for walls, 3.0 for floor, 1.0 for ceiling
	- Shader multipliers: 1.0 for walls, 1.3 for floor, 0.3 for ceiling
	- Ambiance type: ASTURIAS
	- Configuracion para ambientes abiertos	y luminosos con niebla moderada
*/
void	config_ambiance_asturias(t_map *map, t_ambiance *amb)
{
	amb->fog_color_walls = FOG_MEDIO_CLARO;
	amb->fog_color_fc = FOG_MEDIO_CLARO;
	amb->v_max_distance_map = map->max_distance * 0.7f;
	amb->vinv_max_diatance = 1.0f / amb->v_max_distance_map;
	amb->mult_fog_walls = 1.7f;
	amb->mult_fog_floor = 1.7f;
	amb->mult_fog_ceiling = 1.0f;
	amb->k_factor_walls = 8.0f;
	amb->k_factor_floor = 3.0f;
	amb->k_factor_ceiling = 1.0f;
	amb->mult_shader_walls = 1.0f;
	amb->mult_shader_floor = 1.3f;
	amb->mult_shader_ceiling = 0.3f;
	amb->ambiance = ASTURIAS;
}

/*
	Configuracion de la ambiance open
	- Fog color: Dark medium gray for walls, light medium gray for 
		floor and ceiling
	- Max distance: 90% of map max distance
	- Fog multipliers: 0.2 for walls and ceiling, 0.3 for floor
	- Shader k-factors: 8.0 for walls, 4.0 for ceiling, 1.0 for floor
	- Shader multipliers: 1.0 for walls, 0.5 for ceiling, 0.7 for floor
	- Ambiance type: OPEN
	- Configuracion para ambientes abiertos	y luminosos simula exteriores
*/
void	config_ambiance_open(t_map *map, t_ambiance *amb)
{
	amb->fog_color_walls = FOG_MEDIO_OSCURO;
	amb->fog_color_fc = FOG_MEDIO_CLARO;
	amb->v_max_distance_map = map->max_distance * 0.9f;
	amb->vinv_max_diatance = 1.0f / amb->v_max_distance_map;
	amb->mult_fog_walls = 0.2f;
	amb->mult_fog_floor = 0.3f;
	amb->mult_fog_ceiling = 0.2f;
	amb->k_factor_walls = 8.0f;
	amb->k_factor_floor = 1.0f;
	amb->k_factor_ceiling = 4.0f;
	amb->mult_shader_walls = 1.0f;
	amb->mult_shader_floor = 0.7f;
	amb->mult_shader_ceiling = 0.5f;
	amb->ambiance = OPEN;
}

/*
	Configuracion de la ambiance matrix
	- Fog color: Greenish
	- Max distance: 70% of map max distance
	- Fog multipliers: 0.3 for walls, 0.4 for floor, 0.3 for ceiling
	- Shader k-factors: 8.0 for walls, 3.0 for floor, 1.0 for ceiling
	- Shader multipliers: 1.0 for walls, 1.3 for floor, 0.3 for ceiling
	- Ambiance type: ASTURIAS
	- Configuracion para ambientes abiertos	y luminosos con niebla moderada
*/
void	config_ambiance_matrix(t_map *map, t_ambiance *amb)
{
	amb->fog_color_walls = 0x71DB00;
	amb->fog_color_fc = 0x71DB00;
	amb->v_max_distance_map = map->max_distance * 0.7f;
	amb->vinv_max_diatance = 1.0f / amb->v_max_distance_map;
	amb->mult_fog_walls = 0.3f;
	amb->mult_fog_floor = 0.4f;
	amb->mult_fog_ceiling = 0.3f;
	amb->k_factor_walls = 8.0f;
	amb->k_factor_floor = 3.0f;
	amb->k_factor_ceiling = 1.0f;
	amb->mult_shader_walls = 1.0f;
	amb->mult_shader_floor = 1.3f;
	amb->mult_shader_ceiling = 0.3f;
	amb->ambiance = MATRIX;
}
