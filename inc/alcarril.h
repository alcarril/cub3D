/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alcarril.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 03:14:57 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/13 15:30:23 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef ALCARRIL_H
# define ALCARRIL_H

/*
	LIBRERIAS:
*/

# include <stdbool.h>
# include <limits.h>

/*
	DEFINES:
*/

# define WIDTH 720
# define HEIGHT 720

# define MAX_COLUMS 20
# define MAX_ROWS 15

#define MINI_WIDTH 4
#define MINI_HEIGHT 4
# define MINI_ZOOM_FACTOR 1.75f
# define MINIMAP_MAX_ZOOM 3.5f
# define MINIMAP_MIN_ZOOM 0.5f

#define WIN_SCALE 10

# define WALL 1
# define FLOOR 0

# define X 1
# define Y 0

#define VERTICAL 1
#define HORIZONTAL 0

# define PI 3.14159265
# define EPSILON 0.20f
# define FRICTION 0.5f

//esto podria ir en enum
#define N 0
#define S 1
#define E 2
#define W 3

#define CONTROLS_INFO "Press \"z\" on your keyboard to show \
player controls and engine config Keys\n"

# define PITCH_FACTOR 0.02f//
# define MAX_PIXELS_PITCH 0.75f

#define MOUSE_INIT_SENSX 0.2f
# define MOUSE_PITCH_FACTOR 0.002f//
#define MOUSE_DEADZONEX 4
#define MOUSE_DEADZONEY 8
#define MOUSE_MAX_MOV 60
#define ON true
#define OFF false
#define MOUSE_MAX_SENSX 0.9f
#define MOUSE_MIN_SENSX 0.05f
#define MAX_MOUSE_PITCH_FACTOR 0.014f//
#define MIN_MOUSE_PITCH_FACTOR 0.0009f//

#define MousePressMask 1L<<2//norminette


/*
	STRUCTS: Usamos el padrón de diseño "Data Oriented Design" para optimizar el acceso a memoria y el uso del caché.
	Asi evitamos paginar estructuras muy grandes que provoquen fallos de caché (cache missing) y lentitud en el acceso a datos.
	Ademas limitamos el padding de las estructuras para optimizar el uso de memoria.
*/


/*
    DECLARACIONES ANTICIPADAS:
*/
typedef struct s_mlx_api_components t_mlx;
typedef struct s_texture_img t_texture;
typedef struct s_wall	t_wall;
typedef struct	s_mouse		t_mouse;
typedef struct	s_player_controls	t_controls;
typedef struct s_player_data t_player;
typedef struct s_map t_map;
typedef struct s_frame_data t_frame;
typedef struct s_ray t_ray;

typedef struct	s_texture_img
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_texture;

typedef struct s_wall
{
	int		wall_height;      // Altura de la pared en píxeles
	int		wall_start;       // Inicio de la pared en la pantalla pixeles
	int		wall_end;         // Fin de la pared en la pantalla pixeles
	int		tex_x;            // Coordenada X de la textura en la pared
	int		tex_y;            // Coordenada Y de la textura en la pared
	float	text_v_step;      // Text height to screen height ratio
	float	tex_pos;          // Posición inicial en la textura
	double	wall_x;           // Posición de impacto en la pared (0-1)
	t_texture *texture;       // Puntero a la textura seleccionada
}	t_wall;

typedef struct	s_ray
{
	float			raydir[2];
	float			delta[2];
	float			sidedist[2];
	int				map[2];
	unsigned int	step[2];
	bool			side_hit;
	float			wall_dist;
	float			proyected_wall_dist;
}	t_ray;

typedef struct	s_mouse
{
	int		pos_x; // posiicon x en matrxi de pixeles (pantalla)
	int		pos_y; // posicion y en matriz de pixeles (pantalla)
	int		axis_x; //posicion x de referncia matriz de pixeles (pantalla)
	int		axis_y; // posicion x de referncia matriz de pixeles (pantalla)
	float	sens_x; // grados por pixel del mouse
	float	pitch_factor; // fractor de pitch por pixel del mouse (ajustable)
	bool	onoff; //activar y desactivar mouse
}	t_mouse;

typedef struct	s_player_controls
{
	bool	move_up; 
	bool	move_down;
	bool	move_left;
	bool	move_right;
	bool	r_counterclockwise;
	bool	r_clockwise;
	bool	sprint;
	bool	look_up;
	bool	look_down;
}	t_controls;

typedef struct	s_player_data
{
	float	pos_x;
	float	pos_y;
	float	angle;
	float	rad_angle;
	float	speed;
	float	r_speed;
	float	differencial[2];
	float	volume;
	float	fov;
	float	rad_fov;
	float	pitch_factor;
	float	pitch_pix;
	int		max_pitch_pix;
	t_controls	keys;
	t_mouse		mouse;
}	t_player;

typedef struct	s_map
{
	unsigned int	max_columns; //x
	unsigned int	max_rows; //y
	char			map_grids[MAX_ROWS][MAX_COLUMS];

	//textures
	t_texture	textures[4];
	char 		*texture_paths[4];
	//floor ceilling colors
	int		floor_color[3];		// R, G, B
	int		ceiling_color[3];	// R, G, B
	int 	floor_color_hex;	// Hexadecimal
	int 	ceiling_color_hex;	// Hexadecimal
}	t_map;


typedef struct	s_frame_data
{	
	//minimap
	float	mm_height;
	float	mm_widht;
	float	mm_scale[2];
	float 	mm_offset[2];
	float 	mm_zoom_factor;
	bool	minimap_onoff;
	bool	minimap_showrays;


	//Raycasting config
	bool	raycasting_onoff;
	bool	fish_eye;//
	bool	euclidean;
	void	(*draw_walls)(t_mlx *mlx, int column, t_wall *wall, t_ray *ray); //renderizar con ntexturas o sin ellas
	void	(*floor_celling)(t_mlx *mlx); //renderiza suelo y techo speed vs low speed
	
	float	*fov_distances; // Array de distancias hasta las paredes quizas osbre
}	t_frame;

//esta se tendria ue llamar game y tener dentro mlx
//las estrcuturas anidaddas que no requiran cambios
//se pueden cambiar los punteros a estrcuturaas para 
//optiim los accesos al cache de la memoria
typedef struct	s_mlx_api_components
{
	//mlx components
	void	*mlx_var;
	void	*mlx_window;
	void	*mlx_img;
	char	*win_name;// se borra
	
	//window data
	int		win_height;
	int		win_width;

	//image data
	char	*bit_map_address;
	char	*bit_map_address_copy;
	int		bits_per_pixel;
	int		line_length;
	int		endian;

	//game components
	t_player	*player;
	t_map		*map;
	t_frame		*frame;
	
	//log file
	int		log_fd;
}	t_mlx;

/*
	FUNCTION PROTOTIPES:
*/

//Initializations. start game and hooks
bool	init_mlx_components(t_mlx *mlx);
bool	init_images_data(t_mlx *mlx);
bool	load_textures(t_mlx *mlx);
bool	load_single_texture(t_mlx *mlx, t_texture *texture, char *path);
void	start_hooks_and_game(t_mlx *mlx);

//setup game
bool	setup_game(t_mlx *mlx, t_player *player, t_map *map, t_frame *frame);
void	setup_player_mouse(t_mlx *mlx);
void	init_player_orientation_pos(t_player *pl, char cardinal, int pos[2]);
bool	init_frame_data( t_mlx *mlx);
void	get_minimapscale(t_mlx *mlx, float *scale);
int		create_fps_logfile(void);

//Close and free
void	destroy_mlx_componets(int (*f)(), int (*g)(), int (*t)(), t_mlx *mlx);
void	free_loaded_textures(t_mlx *mlx, int loaded_count);
void	free_game(t_mlx *mlx);
int		close_game_manager(t_mlx *mlx);

//quizas desaparezcan
void	init_floor_and_ceiling_colors(t_map *map);
void	setup_window_wh(t_mlx *mlx);
int		rgb_to_hex(int r, int g, int b);

//hooks and events
int		key_press(int keysym, t_mlx *mlx);
int		key_release(int keysym, t_mlx *mlx);
void	print_controls(void);
bool	player_keypress(t_mlx *mlx, int keysym);
bool	player_keypres2(t_mlx *mlx, int keysym);
void	change_fov(t_mlx *mlx);
void	toogle_raycasting(t_mlx *mlx);
void	toggle_textures(t_mlx *mlx);
void	toogle_floor_celling(t_mlx *mlx);
void	toggle_fish_eye(t_mlx *mlx);
void	toogle_dist_calc(t_mlx *mlx);
void	toggle_minimap(t_mlx *mlx);
void	toggle_rays(t_mlx *mlx);
void	minimap_zoom(t_mlx *mlx, bool flag);
int		mouse_init_manager(t_mlx *mlx);
void	toogle_mouse(t_mlx *mlx);
int		mouse_button_manager(int mouse_button, int x, int y, t_mlx *mlx);
int		mouse_move_manager(int x, int y, t_mlx *mlx);

//hooks and events phisics


//player move
bool	is_collision(float x, float y, t_mlx *mlx, float e);
void	rotate_player(t_player *player, float delta_grades);
void	vectorization(t_player *player);
void	move_player(t_mlx *mlx);
void	axis_y_pitch(t_player *player);

//mouse move
void	get_mouse_pos_and_move(t_mlx *mlx);
bool	is_mouse_in_window(t_mlx *mlx, int mouse_x, int mouse_y);
bool	clamp_mouse_deltax(int *pix_dif);
bool	clamp_mouse_deltay(int *pix_dif);
void	reset_mouse_position(t_mlx *mlx, bool *is_move);

//render
int		game_engine(t_mlx *mlx);

//render utils
void	buffering_pixel(int x, int y, t_mlx *mlx, int color);
void	buffering_line(int y, int color, t_mlx *mlx, int width);
void	fps_counter_average(t_mlx *mlx);
void	fps_counter_realtime(t_mlx *mlx);

//raycasting
void	throw_rays(t_mlx *mlx);
void	cast_ray(t_mlx *mlx, unsigned int n_ray, float ray_angle);
void	set_ray(t_mlx *mlx, t_ray *ray, float ray_angle);
void	scale_wall(t_wall *wall, float perpendicular_distance, int win_height, int pitch);
void	draw_wall_column(t_mlx *mlx, int column, t_wall *wall, t_ray *ray);

//dda algorithm
float	get_distance_to_wall(t_mlx *mlx, t_ray *ray, float ray_angle);
void	calc_side_dist(t_mlx * mlx, t_ray *ray);
void	dda_loop(t_mlx * mlx, t_ray *ray);
float	get_ray_distance(t_mlx *mlx, t_ray *ray);
float	get_ray_distance_euclidean(t_mlx *mlx, t_ray *ray);

//floor and ceiling
void	render_floor_and_ceiling(t_mlx *mlx);
void	render_floor_and_ceiling_speed(t_mlx *mlx);

//textured walls
void			draw_wall_column_tex(t_mlx *mlx, int column, t_wall *wall, t_ray *ray);
t_texture		*select_texture(t_mlx *mlx, t_ray *ray);
double			calculate_wall_x(t_mlx *mlx, t_ray *ray);
void	calculate_tex(t_wall *wall, t_texture *texture, int win_height, int pitch);
unsigned int	extract_color(t_texture *texture, int tex_x, int tex_y);

//fog blur shaders
unsigned int apply_fog_pixel(unsigned int color, unsigned int fog_color, float distance, float max_distance);
unsigned int apply_blur(t_mlx *mlx, int column, int row);
void apply_fog(t_mlx *mlx, unsigned int fog_color, float max_distance);

//render minimap 2D
int		render_frame2D(t_mlx *mlx);
void	draw_mini_pixel_offset(t_mlx *mlx, int *win, float *scaled_zoom);
bool	is_wall(t_mlx *mlx, float *map);
void	is_person2D(t_mlx *mlx, int *window, float *map);
bool	is_minimapzone(int win_x, int win_y, t_mlx *mlx);

//render rays 2D
void	draw_rays2D(t_mlx *mlx);
void	draw_ray2D(t_mlx *mlx, float *differencial, float rad);
bool	touch_wall(t_mlx *mlx, float x, float y);

//debug
void	print_texture_values(t_mlx *mlx);

#endif