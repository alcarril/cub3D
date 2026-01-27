/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alcarril.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 03:14:57 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/27 04:09:10 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALCARRIL_H
# define ALCARRIL_H

/*
	LIBRARIES:
*/
# include <stdbool.h>
# include <limits.h>
# include <stddef.h>
# include <stdint.h>

# define WIDTH 600
# define HEIGHT 600
# define WIN_SCALE 15
# define MAX_COLUMS 25
# define MAX_ROWS 25

//window minimum width and height
# define BASE_WIDTH 1280
# define BASE_HEIGHT 720

//map axis
# define X 1
# define Y 0

//math constants
# define PI 3.14159265
# define EPSILON 0.20f

//time
# define MAX_DT_FRAME 16 //in ms para 60 fps

//aceleraion vector indexes
# define FRONT 0
# define BACK 1
# define SIDES 2

//aceleration 2D plane
# define ACCELERATION_FRONT_FACTOR 6.00f
# define ACCELERATION_BACK_FACTOR 5.00f
# define ACCELERATION_SIDES_FACTOR 6.00f
# define SPRINT_ACCELERATION_FRONT_FACTOR 10.00f

//player speed max min values
# define SPEED_DIGITAL 0.033f
# define MAX_PLAYER_SPEED 0.008f
# define MAX_PLAYER_SPEED_STRINT 0.018f
# define SPEEDMIN_MS 0.0001f

//traccion factor player
# define TRACCION_K 1.0f

//factor de relacion aceleracion jugador eje z respecto gravedad
# define ACCELERATION_K_EARTH 2.0f
# define ACCELERATION_K_MOON 3.0f
# define ACCELERATION_K_HULKPITER 2.0f
# define ACCELERATION_K_SPECTRO 1.5f
# define ACCELERATION_K_JETPACK 15.0f

//map axis z for camz
# define MIDDLE_Z 0.0f
# define MAP_MAX_Z 1.0f
# define FLOOR_Z -1.0f

//map phisics
# define WALL_FRICTION 0.5f
# define FLOOR_FRICTION_MS 0.002f
# define AIR_FRICTION_MS 0.001f
# define GRAVITY_EARTH 30.0f
# define GRAVITY_MOON 5.0f
# define GRAVITY_HULKPITER 60.0f
# define GRAVITY_SPECTRO 5.0f
# define GRAVITY_JETPACK 5.0f

//pitch player
# define PITCH_FACTOR 0.02f//
# define MAX_PIXELS_PITCH 0.75f

//hits and walls
# define WALL 1
# define BONUS_WALL 2
# define FLOOR 0
# define VERTICAL 1
# define HORIZONTAL 0

//directions
# define N 0
# define S 1
# define E 2
# define W 3

//switches
# define ON true
# define OFF false

//Minimap
# define MINI_WIDTH 4
# define MINI_HEIGHT 4
# define MINI_ZOOM_FACTOR 1.75f
# define MINIMAP_MAX_ZOOM 3.5f
# define MINIMAP_MIN_ZOOM 0.5f

//mouse
# define MOUSE_INIT_SENSX 0.2f
# define MOUSE_PITCH_FACTOR 0.002f
# define MOUSE_DEADZONEX 4
# define MOUSE_DEADZONEY 8
# define MOUSE_MAX_MOV 50
# define MOUSE_MAX_SENSX 0.9f
# define MOUSE_MIN_SENSX 0.1f
# define MAX_MOUSE_PITCH_FACTOR 0.014f
# define MIN_MOUSE_PITCH_FACTOR 0.0009f
# define OUT false
# define IN true

//rgb
# define R 0
# define G 1
# define B 2

//define refsbuff
# define HEIG 0
# define WIDT 1
# define HOR 2

//fog colora
# define FOG_CLARO 0xA0A0A0
# define FOG_MEDIO_CLARO 0xA0A0A0
# define FOG_MEDIO_OSCURO 0x606060
# define FOG_OSCURO 0x202020

//dist vision for fog
# define CLOSE_DISTANCE 6.0f
# define MEDIUM_DISTANCE 10.0f
# define FAR_DISTANCE 14.0f

//ambiances
# define ASTURIAS 1
# define CEMENTERY 2
# define OPEN 3
# define MATRIX 4

//hud color
# define COLOR_HUD 0x47704CFF

//Fisxed shift
# define FP_SHIFT 16

//messages
# define CONTROLS_INFO "Press \"z\" on your keyboard to show \
player controls and engine config Keys\n"

/*
	STRUCTS: We use the "Data Oriented Design" pattern to optimize memory 
	access and cache usage. This helps avoid paging very large structures 
	that could cause cache misses and slow data access. Additionally, we 
	limit structure padding to optimize memory usage.
*/

/*
	Forward declarations of structs to avoid circular dependencies.
*/
typedef struct s_mlx_api_components	t_mlx;
typedef struct s_texture_img		t_texture;
typedef struct s_ambiance			t_ambiance;
typedef struct s_wall				t_wall;
typedef struct s_mouse				t_mouse;
typedef struct s_player_controls	t_controls;
typedef struct s_player_data		t_player;
typedef struct s_map				t_map;
typedef struct s_frame_data			t_frame;
typedef struct s_ray				t_ray;
typedef struct s_phisics			t_phisics;
typedef struct s_player_phisics		t_plphisics;
typedef struct s_locals				t_locals;
typedef struct s_rf					t_rf;
typedef unsigned int				t_u;

typedef struct s_phisics
{
	float	gravity;
	float	air_friction;
	float	floor_friction;
}	t_phisics;

typedef struct s_player_phisics
{
	float	jump_force;
}	t_plphisics;

typedef struct s_rf
{
	int				y;
	int				width;
	int				height;
	int				horizon;
	char			*bitmap;
	int				line_len;
	int				bpp;
	int				line_bytes;
	unsigned int	ceiling_color;
	unsigned int	floor_color;
	unsigned int	offset;
}	t_rf;

typedef struct s_locals
{
	float			text_v_step;
	float			tex_pos;
	unsigned int	*tex_ptr;
	unsigned int	*fb_ptr;
	int				i;
	int				wall_end;
	int				tex_y;
	int				tex_stride;
	int				win_width;
}	t_locals;

typedef struct s_texture_img
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_texture;

typedef struct s_ambiance
{
	int		ambiance;
	int		fog_color_walls;
	int		fog_color_fc;
	float	mult_fog_walls;
	float	mult_fog_ceiling;
	float	mult_fog_floor;
	float	k_factor_walls;
	float	k_factor_ceiling;
	float	k_factor_floor;
	float	mult_shader_walls;
	float	mult_shader_ceiling;
	float	mult_shader_floor;
	float	v_max_distance_map;
	float	vinv_max_diatance;
}	t_ambiance;

typedef struct s_wall
{
	int			wall_height;// Altura de la pared en píxeles
	int			wall_start;// Inicio de la pared en la pantalla pixeles
	int			wall_end;// Fin de la pared en la pantalla pixeles
	int			tex_x;// Coordenada X de la textura en la pared
	int			tex_y;// Coordenada Y de la textura en la pared
	float		text_v_step;// Text height to screen height ratio
	float		tex_pos;// Posición inicial en la textura
	float		vertical_offset; // Desplazamiento vertical de la pared
	double		wall_x;// Posición de impacto en la pared (0-1)
	t_texture	*texture;// Puntero a la textura seleccionada
}	t_wall;

typedef struct s_ray
{
	float			raydir[2];
	float			delta[2];
	float			sidedist[2];
	float			proyected_wall_dist;
	float			wall_dist;
	int				map[2];
	int				wall_value;
	int				step[2];
	bool			side_hit;
	char			padding[3];
}	t_ray;

typedef struct s_frame_data
{
	float	*fov_distances;
	float	delta_rays;
	float	mm_height;
	float	mm_widht;
	float	mm_scale[2];
	float	mm_offset[2];
	float	mm_zoom_factor;
	int		wall_start_min;
	int		wall_end_max;
	bool	minimap_onoff;
	bool	minimap_showrays;
	bool	raycasting_onoff;
	bool	fish_eye;
	bool	euclidean;
	bool	boost;
	bool	textures_onoff;
	bool	ambiance_onoff;
	bool	phisics_onoff;
	bool	dukedoom_mode;
	t_wall	walls[BASE_WIDTH];
	t_ray	rays[BASE_WIDTH];
}	t_frame;

typedef struct s_mouse
{
	int		pos_x;// x position in pixel matrix (screen)
	int		pos_y;// y position in pixel matrix (screen)
	int		axis_x;// x reference position in pixel matrix (screen)
	int		axis_y;// y reference position in pixel matrix (screen)
	float	sens_x;// degrees per mouse pixel
	float	pitch_factor;// pitch factor per mouse pixel (adjustable)
	bool	onoff;// enable and disable mouse
	bool	out_and_on;
}	t_mouse;

typedef struct s_player_controls
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

typedef struct s_player_data
{
	float		pos_x;
	float		pos_y;
	float		angle;
	float		rad_angle;
	float		speed;
	float		r_speed;
	float		diff[2];
	float		v_move[2];
	float		volume;
	float		fov;
	float		rad_fov;
	float		fov_half;
	float		pitch_factor;
	float		pitch_pix;
	int			max_pitch_pix;
	float		camz;
	float		aceleration_z;
	float		aceleration_zcp;
	float		speed_z;
	float		aceleration[3];
	float		traccion_k;
	float		speed_dt;
	float		max_speed_dt;
	float		speed_a[2];
	float		max_speed_a[2];
	int			min_dt[3];
	int			max_dt;
	bool		is_sprinting;
	bool		is_jumping;
	bool		is_onair;
	bool		is_flying;
	bool		is_hided;
	bool		is_groundpound;
	bool		is_dragging;
	t_controls	keys;
	t_mouse		mouse;
}	t_player;

typedef struct s_map
{
	char			map_grids[14][25]; //esto va a ser un puntero doble a liberar pero la mmera de accader a el es igual
	unsigned int	max_columns; //x lo rellena carbon con los valores del mapa y con esto se reserva memeria
	unsigned int	max_rows; //y lo rrelena carbon cunado copie los valores del maap y con esto se reserva memeria
	//textures (estos se van a tener que liberar)
		//ahora es un array de texturas en memeria local de 5 posiciones tu vas a tenern que reservar memoria para n texturas se ocvnerita en un puntero simple //t_texture	*textures
	t_texture	textures[5]; 
		//esta variable es la que dice cunatas texturas hay en el mapa con ella reservamos memeria paa los paths y las texturas
		//ahora esta por defecto en 5 y lo uso para reservar memeria pero la necesito
	int		n_textures;
		//ahora esta en 5 en memria local pero tu vas a tener que reservar memeria para n texturas se convertira en un puntero doble char **teexture_paths
	char *texture_paths[5];//Esto va a ser un puntero a linerar con los nombre que reserva carbon
	//TODAS MEM ENCARGO DE LIBERARLAS YO NO TE PREOCUPES TU SOLO LIBERALA MEMRIA DE TUS STRCUTURSA DESOUES DE HACER LE EMPAALME¡
	//floor ceilling colors (estos no se tiene que liberar)
	int		floor_color[3];		// R, G, B
	int		ceiling_color[3];	// R, G, B
	int		floor_color_hex;	// Hexadecimal
	int		ceiling_color_hex;	// Hexadecimal
	//Max distance for shading and for (diagonal)
	float	max_distance;
}	t_map;

typedef struct s_mlx_api_components
{
	void		*mlx_var;
	void		*mlx_window;
	void		*mlx_img;
	char		*bit_map_address;
	char		*bit_map_address_copy;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			win_height;
	int			win_width;
	int			log_fd;
	long long	now_timestamp;
	long long	bef_timestamp;
	long long	del_timestamp;
	bool		has_been_mouse_in_window;
	t_player	*player;
	t_map		*map;
	t_frame		*frame;
	t_ambiance	amb;
	t_phisics	phisics;
}	t_mlx;

/*
	FUNCTION PROTOTIPES:
*/

//Initializations. start game and hooks
bool			init_mlx_components(t_mlx *mlx);
bool			init_images_data(t_mlx *mlx);
bool			load_textures(t_mlx *mlx);
bool			load_single_texture(t_mlx *mlx, t_texture *texture,
					char *path);
void			start_hooks_and_game(t_mlx *mlx);
//setup game
bool			setup_game(t_mlx *mlx, t_player *player, t_map *map,
					t_frame *frame);
void			setup_player_mouse(t_mlx *mlx);//
void			init_player_orientation_pos(t_player *pl, char cardinal,
					int pos[2]);
void			init_player_phisics(t_player *pl, t_phisics *ph);
void			init_player_mouse(t_player *pl, t_mlx *mlx);
bool			init_frame_data( t_mlx *mlx);
void			get_minimapscale(t_mlx *mlx, float *scale);
int				create_fps_logfile(void);
void			setup_default_ambiance(t_map *map, t_ambiance *amb);
void			setup_default_phisics(t_phisics *phisics);
//Close and free
void			destroy_mlx_componets(int (*f)(), int (*g)(), int (*t)(),
					t_mlx *mlx);
void			free_loaded_textures(t_mlx *mlx, int loaded, int max_textures);
void			free_map_data(t_mlx *mlx);//
void			free_game(t_mlx *mlx);
int				close_game_manager(t_mlx *mlx);
//quizas desaparezcan
void			init_floor_and_ceiling_colors(t_map *map);
void			setup_window_wh(t_mlx *mlx);
int				rgb_to_hex(int r, int g, int b);
//hooks and events
int				key_press(int keysym, t_mlx *mlx);
int				key_release(int keysym, t_mlx *mlx);
void			print_controls(void);
void			print_controls2(void);
bool			player_keypress(t_mlx *mlx, int keysym);
bool			player_keypres2(t_mlx *mlx, int keysym);
void			change_fov(t_mlx *mlx);
void			change_player_volume(t_mlx *mlx, bool flag);
void			player_sprint_keypress(t_mlx *mlx);
void			player_sprint_keyrelease(t_mlx *mlx);
bool			graphics_engine_keypress(t_mlx *mlx, int keysym);
void			toogle_raycasting(t_mlx *mlx);
void			toggle_textures(t_mlx *mlx);
void			toogle_floor_celling(t_mlx *mlx);
void			toggle_fish_eye(t_mlx *mlx);
void			toogle_dist_calc(t_mlx *mlx);
bool			ambiance_keypress(t_mlx *mlx, int keysym);
void			select_ambiance(t_mlx *mlx, int ambiance);
void			select_ambiance1(t_mlx *mlx, int ambiance);
void			toogle_ambiance(t_mlx *mlx);
void			toggle_minimap(t_mlx *mlx);
void			toggle_rays(t_mlx *mlx);
bool			phisics_engine_keypress(t_mlx *mlx, int keysym);
void			toggle_phisics_mode(t_mlx *mlx);
void			toggle_dukedoom_mode(t_mlx *mlx);
void			player_space_keypress(t_mlx *mlx);
void			player_space_keyrelease(t_mlx *mlx);
void			player_control_keypress(t_mlx *mlx);
void			player_blockmayus_keypress(t_mlx *mlx);
void			player_q_keypress(t_mlx *mlx);
bool			select_gravity_modes(t_mlx *mlx, int keysym);
void			selectearth_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx);
void			selectmoon_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx);
void			selecthulker_mode(t_phisics *phisics, t_player *pl,
					t_mlx *mlx);
void			selectspectr_mode(t_phisics *phisics, t_player *pl,
					t_mlx *mlx);
void			selectjetpack_mode(t_phisics *phisics, t_player *pl,
					t_mlx *mlx);
void			minimap_zoom(t_mlx *mlx, bool flag);
int				mouse_init_manager(t_mlx *mlx);
void			toogle_mouse(t_mlx *ml);
int				mouse_button_manager(int mouse_button, int x, int y,
					t_mlx *mlx);
//player move
bool			is_collision(float x, float y, t_mlx *mlx, float e);
void			rotate_player(t_player *player, float delta_grades);
void			vectorization(t_player *pl, long long dt, float speed,
					float angle);
void			move_player(t_mlx *mlx);
void			axis_y_pitch(t_player *player);
void			jump_speed_vecmove(t_mlx *mlx);
//move player phisics
void			difspeed_and_vecmove(t_mlx *mlx, long long delta_time);
void			difvspeed_and_vecmove_nukedoom(t_mlx *mlx,
					long long delta_time);
void			acelerate_player(t_mlx *mlx, unsigned int vi, long long dt,
					bool *mo);
void			decelerate_player(t_mlx *mlx, long long dt, bool mo);
void			acelerate_dukedoom(t_mlx *mlx, unsigned int vi, long long dt,
					bool *mo);
void			normalize_vector(float *v_speed, float *max_speed);
void			decelerate_dukedoom(t_mlx *mlx, long long dt, bool mo);
void			vectorization_dukedoom(t_player *pl, long long dt,
					float angle);
//moveplayer air
void			decelerate_player_air(t_mlx *mlx, long long dt);
void			decelerate_dukedoom_air(t_mlx *mlx, long long dt);
void			airborne_vectorization(t_player *pl, long long dt,
					bool is_dukedoom);
void			jump(t_mlx *mlx, long long dt_ms);
void			check_altitude(t_player	*pl);
//mouse move
void			get_mouse_pos_and_move(t_mlx *mlx);
bool			is_mouse_in_window(t_mlx *mlx, int mouse_x, int mouse_y);
void			move_player_with_mouse(t_mlx *mlx, int *pix_dif,
					bool *is_move);
bool			clamp_mouse_deltax(int *pix_dif);
bool			clamp_mouse_deltay(int *pix_dif);
void			reset_mouse_position(t_mlx *mlx, bool *is_move);
//engine loop
int				game_engine(t_mlx *mlx);
//buffering pixels and lines
void			buffering_pixel(int x, int y, t_mlx *mlx, int color);
void			buffering_line(int y, int color, t_mlx *mlx, int width);
//render utils
void			fps_counter_average(t_mlx *mlx);
void			fps_counter_realtime(t_mlx *mlx);
void			ft_memset_int(void *s, int c, size_t n);
bool			is_wall_tile(char map_value);
//raycasting
void			throw_rays(t_mlx *mlx);
void			cast_ray(t_mlx *mlx, unsigned int n_ray, float ray_angle);
void			set_ray(t_mlx *mlx, t_ray *ray, float ray_angle);
void			draw_wall_column(t_mlx *mlx, int column, t_wall *wall,
					t_ray *ray);
void			scale_wall_phisics(t_wall *wall, float perp_dist, t_mlx *mlx);
//dda algorithm
float			get_distance_to_wall(t_mlx *mlx, t_ray *ray, float ray_angle);
void			calc_side_dist(t_mlx *mlx, t_ray *ray);
void			dda_loop(t_mlx *mlx, t_ray *ray);
float			get_ray_distance(t_mlx *mlx, t_ray *ray);
float			get_ray_distance_euclidean(t_mlx *mlx, t_ray *ray);
//render 3d
void			render_frame3d(t_mlx *mlx);
//floor and ceiling
void			render_floor_and_ceiling(t_mlx *mlx);
void			render_floor_and_ceiling_speed(t_mlx *mlx);
void			load_locals(t_rf *s, t_mlx *mlx, int *ceiling_limit,
					int *floor_limit);
//render walls
void			render_walls(t_mlx *mlx);
void			render_walls_tex(t_mlx *mlx);
void			render_walls_no_textures(t_mlx *mlx);
void			render_walls_tex_speed(t_mlx *mlx);
void			render_walls_ambiance(t_mlx *mlx);
//textured walls
void			draw_wall_column_tex(t_mlx *mlx, int column, t_wall *wall,
					t_ray *ray);
void			drawwallcoltexspeed(t_mlx *mlx, int column, t_wall *wall,
					t_ray *ray);
t_texture		*select_texture(t_mlx *mlx, t_ray *ray);
double			calculate_wall_x(t_mlx *mlx, t_ray *ray);
void			calculate_tex(t_wall *wall, t_texture *texture, int win_height,
					t_player *player);

unsigned int	extract_color(t_texture *texture, int tex_x, int tex_y);
//floor_and_ceiling amiances
void			render_floor_and_ceiling_amb(t_mlx *mlx);
//wall ambiance
void			drawinglopp_tex_amb(t_mlx *mlx, int column, t_wall *wall,
					t_ray *ray);
//fog blur shaders
unsigned int	apply_fog_pixel(unsigned int col, unsigned int fog_c, float p);
unsigned int	apply_desaturation(unsigned int color, float factor);
//shaders
unsigned int	apply_shade(unsigned int color, float shade);
unsigned int	shade_linear(unsigned int color, float dist, float max_dist);
unsigned int	shade_inverse(unsigned int color, float k, float prop_dist);
unsigned int	shade_exponential(unsigned int color, float dens, float prop);
//ambiance
void			config_ambiance_cementery(t_ambiance *amb);
void			config_ambiance_asturias(t_map *map, t_ambiance *amb);
void			config_ambiance_open(t_map *map, t_ambiance *amb);
void			config_ambiance_matrix(t_map *map, t_ambiance *amb);
float			dist_factor_floor(int win_height, int win_y, int horizon,
					int ambient);
float			dist_factor_ceiling(int win_y, int horizon, int ambient);
unsigned int	apllyamb_ceiling(t_ambiance *a, float df, unsigned int rcol);
unsigned int	apllyamb_floor(t_ambiance *a, float df, unsigned int rcol);
//render minimap 2D
int				render_frame2d(t_mlx *mlx);
void			update_center_minimap_offset(t_mlx *mlx, float *escaled_zoom);
void			draw_mini_pixel_offset(t_mlx *mlx, int *win,
					float *scaled_zoom);
bool			is_wall(t_mlx *mlx, float *map);
void			is_person2d(t_mlx *mlx, int *window, float *map);
//render rays 2D
void			draw_rays2d(t_mlx *mlx, float *scal_z);
void			draw_ray2d(t_mlx *mlx, float *differencial, float rad,
					float *scal_z);
bool			touch_wall(t_mlx *mlx, float x, float y);
//mem utils
void			ft_memsetboost(void *s, int c, size_t n);
void			ft_memsetlonglong(void *s, int c, size_t n);
void			ft_memsetlong(void *s, int c, size_t n);
void			ft_memsetint(void *s, int c, size_t n);
void			ft_memsetchar(void *s, int c, size_t n);
void			ft_bzeroboost(void *s, size_t n);
void			ft_memfillboost(void *s, int c, size_t n);
void			ft_memfilllonglong(void **s, int c, size_t *n);
void			ft_memfilllong(void **s, int c, size_t *n);
void			ft_memfillint(void **s, int c, size_t *n);
void			ft_memfillchar(void *s, int c, size_t n);
//debug
void			print_texture_values(t_mlx *mlx);

#endif