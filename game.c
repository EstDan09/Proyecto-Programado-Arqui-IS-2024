// includes
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

// teclas
#define vk_space  32
#define vk_enter  10
#define vk_f 102
int key_pressed = 0;
#define KEY_W 119 // ASCII value for 'w'
#define KEY_A 97  // ASCII value for 'a'
#define KEY_S 115 // ASCII value for 's'
#define KEY_D 100 // ASCII value for 'd'

//colores
const short c_wall   = 1;
const short c_space  = 4;
const short c_life   = 5;
const short c_life2   = 5;
const short c_player = 6;
const short c_player2 = 6;
const short c_bullet = 6;
const short c_bullet2 = 6;
const short c_enemy  = 7;
const short c_box    = 8;
const short c_hud    = 1;



// Variables
bool EXIT = false;
short level = 1;
short lifes = 3;
short lifes2 = 3;
short wins1 = 0;
short wins2 = 0;
int bullet_shoot = false;
int bullet2_shoot = false;
int current_lvl_x;
int current_lvl_y;
int w, h;
static int len_xoff = 31;
static int len_yoff = 2;

// codigo elementos de mapas
#define i_wall      1
#define i_life2     2
#define i_space     3
#define i_life      4
#define i_exit      5
#define i_enemy_v   6
#define i_box       7
#define i_enemy_h   8

// arte de menu
const char *menu_logo[7] = {
    " _____  __  __  ____  __      __    _    _ ",
    "(  _  )(  )(  )(_  _)(  )    /__\  ( \/\/ )",
    " )(_)(  )(__)(   )(   )(__  /(__)\  )    ( ",
    "(_____)(______) (__) (____)(__)(__)(__/\__)",
    "                                           ",
    "                                           ",
    "                                           ",
};

/**
 * Funcion para actualizar los NPC´s
*/
int str_len(const char* str) {
    int size = 0;
    while(*str++) ++size;
    return size;
}

int logo_h_size = sizeof(menu_logo)/sizeof(menu_logo[0]);

/**
 * Funcion para obtener el tamaño del logo
*/
int get_logo_w_size(void) {
    int logo_w_size = 1;

    for (int i = 0; i < logo_h_size; i++) {
        int len = str_len(menu_logo[i]);
        if (len > logo_w_size) {
            logo_w_size = str_len(menu_logo[i]);
        }
    }
    return logo_w_size;
}

/**
 * Funcion para dibujar en consola el logo del juego
*/
int logo_w_size = 1;
void draw_logo(int h, int w) {
    
    if (logo_w_size == 1) {
        logo_w_size = get_logo_w_size() / 2;
    }

    
    attron(COLOR_PAIR(c_hud));
    for (int i = 0; i < logo_h_size; i++) {
        mvprintw(3 + i /* Logo Y pos */, w/2 - logo_w_size, menu_logo[i]);
    }
    attroff(COLOR_PAIR(c_hud));
}


/**
 * Mapas de niveles
*/
short arr_size_x;
#define s_wall  "///" 
#define s_exit  "***"
#define s_empty "   "
#define s_life  "1HP"
#define s_life2 "2HP"
#define s_enemy "(-)"
#define s_box   "[=]"
#define s_space "..."


/**
 * Nivel 1
*/
#define lvl_one_x 27
#define lvl_one_y 20
#define level_one_size lvl_one_x

short lvl_one[lvl_one_y][lvl_one_x] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 4, 0, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


/**
 * Nivel 2
*/
#define lvl_two_x 27
#define lvl_two_y 20
#define  level_two_size lvl_two_x

short lvl_two[lvl_two_y][lvl_two_x] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 1, 0, 0, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 7, 0, 4, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 7, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


/**
 * Nivel 3
*/
#define lvl_three_x 27
#define lvl_three_y 20
#define level_three_size lvl_three_x

short lvl_three[lvl_three_y][lvl_three_x] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 7, 4, 7, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 7, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

/**
 * Nivel 4
*/
#define lvl_fo_x 27
#define lvl_fo_y 20
#define level_fo_size lvl_fo_x

short lvl_fo[lvl_fo_y][lvl_fo_x] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


/**
 * Nivel 5
*/
#define lvl_five_x 27
#define lvl_five_y 20
#define  level_five_size lvl_five_x

short lvl_five[lvl_five_y][lvl_five_x] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 2, 4, 2, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 1 },
    { 1, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 4, 2, 4, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


/**
 * Nivel 6
*/
#define lvl_six_x 27
#define lvl_six_y 20
#define level_six_size lvl_six_x

short lvl_six[lvl_six_y][lvl_six_x] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


/**
 * Nivel 7
*/
#define lvl_last_x 27
#define lvl_last_y 20
#define level_last_size lvl_last_x

short lvl_last[lvl_last_y][lvl_last_x] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


/**
 * Funcion para asignar colores
*/
void SetColor() {
    start_color();
    init_pair(c_wall,   COLOR_BLUE,     COLOR_BLACK);
    init_pair(c_space,  COLOR_BLACK,    COLOR_BLACK);
    init_pair(c_life,   COLOR_GREEN,    COLOR_BLACK);
    init_pair(c_life2,   COLOR_GREEN,    COLOR_BLACK);
    init_pair(c_player, COLOR_MAGENTA,  COLOR_BLACK);
    init_pair(c_player2,COLOR_WHITE,    COLOR_BLACK);
    init_pair(c_bullet, COLOR_GREEN,    COLOR_BLACK);
    init_pair(c_bullet2, COLOR_GREEN,    COLOR_BLACK);
    init_pair(c_enemy,  COLOR_RED,      COLOR_BLACK);
    init_pair(c_box,    COLOR_YELLOW,   COLOR_BLACK);
}

// Draw colored instance
#define td_indent 2 // Top & down ident
#define symbol_count 3

/**
 * Funcion para dibujar elementos
*/
void draw_instance(int y, int x, int color, char name[]) {
    attron(COLOR_PAIR(color));

    // Win offset
    int win_xoffset = w/2;
    int win_yoffset = h/2;
    
    // Level offset
    int lvl_xoffset = (current_lvl_x/2)*symbol_count+(current_lvl_x%2);
    int lvl_yoffset = (current_lvl_y+(td_indent*2))/2-(1 /* +1 indent hud */+(current_lvl_y%2));

    mvprintw(
        /* Y pos */ ceil(win_yoffset - lvl_yoffset) + (y+td_indent), 
        /* X pos */ ceil(win_xoffset - lvl_xoffset) + (x*symbol_count), 
        /* S pos */ name
    );
    attroff(COLOR_PAIR(color));
}

/**
 * Struct base de objetos del juego
*/
struct class_obj {
    char symbol[20];
    int hor, ver;
    int x, y;
    int direction;
};

// structs a utilizar
struct class_obj player = {};
struct class_obj player2 = {};
struct class_obj bullet = {};
struct class_obj bullet2 = {};
struct class_obj enemy[5]={};

/**
 * Funcion para el movimineto de los NPC´s
*/
void enemy_move(short current_lvl[][arr_size_x], int index) {
        if (enemy[index].direction == 1 || enemy[index].direction == -1)  {
            // movimiento vertical
            if (current_lvl[enemy[index].y + enemy[index].ver][enemy[index].x] == i_wall || 
                current_lvl[enemy[index].y + enemy[index].ver][enemy[index].x] == i_box)
            {
                enemy[index].direction *= -1;
            }
            
            enemy[index].ver = 1 * enemy[index].direction;
            enemy[index].y += enemy[index].ver;
            
            return;
        }

        if (enemy[index].direction == 2 || enemy[index].direction == -2){
            // movimiento horizontal
            if (current_lvl[enemy[index].y][enemy[index].x + enemy[index].hor] == i_wall || 
                current_lvl[enemy[index].y][enemy[index].x + enemy[index].hor] == i_box)
            {
                enemy[index].direction *= -1;
            }

            enemy[index].hor = 1 * enemy[index].direction;
            enemy[index].x += enemy[index].hor;

            return;
        }
}

/**
 * Funcion para actualizar los NPC´s
*/
void enemy_update(short current_lvl[][arr_size_x]) {
    for (int i = 0; i < sizeof(enemy)/sizeof(enemy[0]); i++) {
        enemy_move(current_lvl, i);
    }
}

/**
 * Funcion para limpiar enemigos de la pantalla
*/
void clear_enemy() {
    for (int i = 0; i < sizeof(enemy)/sizeof(enemy[0]); i++) {
        enemy[i].y = 0;
        enemy[i].x = 0;
        enemy[i].direction = 0;
    }
}

/**
 * Funcion para inicializar objetos
*/
void obj_init(struct class_obj *obj, int x, int y, int dir, char *objname) {
    obj->x = x;
    obj->y = y;
    obj->direction = dir;
    strcpy(obj->symbol, objname);
}

//------------------------------------------------------------------------------------------------------------------------------------------------

// variables jugador 1
int dir_x;
int dir_y;
int dir_shoot;

/**
 * Funcion para gestionar las colisiones del jugador 1
*/
void player_move(int key) {
    // validacion tecla
    int key_left  = ( key == KEY_LEFT  ) ? 1 : 0;
    int key_right = ( key == KEY_RIGHT ) ? 1 : 0;
    int key_down  = ( key == KEY_DOWN  ) ? 1 : 0;
    int key_up    = ( key == KEY_UP    ) ? 1 : 0;
    // direccion
    dir_x = key_right - key_left;
    dir_y = key_down  - key_up;
    // animacion
    if (dir_x == 0 && dir_y == 0) {
        strcpy(player.symbol, "|0|");
    } else {
        if ( dir_x ==  1 ) { dir_shoot =  1; strcpy(player.symbol, "|0>"  ); }
        if ( dir_x == -1 ) { dir_shoot = -1; strcpy(player.symbol, "<0|"  ); }
        if ( dir_y == -1 ) { dir_shoot = -2; strcpy(player.symbol, "/0\\" ); }
        if ( dir_y ==  1 ) { dir_shoot =  2; strcpy(player.symbol, "\\0/" ); }
    }
    player.hor = 1 * dir_x;
    player.ver = 1 * dir_y;
    if (player.hor != 0) {
        player.ver = 0;
    } else if (player.ver != 0) {
        player.hor = 0;
    }
    player.x += player.hor;
    player.y += player.ver;
}

/**
 * Funcion para gestionar las colisiones del jugador 1
*/
void player_collision(short current_lvl[][arr_size_x]) {
    switch(current_lvl[player.y][player.x]) {
        case i_wall:    
        case i_box:     
        case i_space:   
            player.x -= player.hor;
            player.y -= player.ver;
        break;
        case i_life:
            current_lvl[player.y][player.x] = 0;
            lifes = lifes + 1;
        break;
    }
    // Enemigo
    for (short i = 0; i < sizeof(enemy)/sizeof(enemy[0]); i++) {
        if (player.y == enemy[i].y &&
            player.x == enemy[i].x)
        {
            lifes = lifes - 1; 
        }
    }
}

// variables jugador 1
int dir2_x;
int dir2_y;
int dir2_shoot;

/**
 * Funcion para gestionar las colisiones del jugador 1
*/
void player2_move(int key) {
    // validacion tecla
    int key_left2  = ( key == KEY_A  ) ? 1 : 0;
    int key_right2 = ( key == KEY_D ) ? 1 : 0;
    int key_down2 = ( key == KEY_S  ) ? 1 : 0;
    int key_up2    = ( key == KEY_W    ) ? 1 : 0;

    // direccion
    dir2_x = key_right2 - key_left2;
    dir2_y = key_down2  - key_up2;

    // animacion 
    if (dir2_x == 0 && dir2_y == 0) {
        strcpy(player2.symbol, "|X|");
    } else {
        if ( dir2_x ==  1 ) { dir2_shoot =  1; strcpy(player2.symbol, "|X>"  ); }
        if ( dir2_x == -1 ) { dir2_shoot = -1; strcpy(player2.symbol, "<X|"  ); }
        if ( dir2_y == -1 ) { dir2_shoot = -2; strcpy(player2.symbol, "/X\\" ); }
        if ( dir2_y ==  1 ) { dir2_shoot =  2; strcpy(player2.symbol, "\\X/" ); }
    }

    player2.hor = 1 * dir2_x;
    player2.ver = 1 * dir2_y;

    if (player2.hor != 0) {
        player2.ver = 0;
    } else if (player2.ver != 0) {
        player2.hor = 0;
    }

    player2.x += player2.hor;
    player2.y += player2.ver;
}

/**
 * Funcion para manejar las colisiones del jugador 2
*/
void player2_collision(short current_lvl[][arr_size_x]) {
    switch(current_lvl[player2.y][player2.x]) {
        case i_wall:    
        case i_box:     
        case i_space: 
            player2.x -= player2.hor;
            player2.y -= player2.ver;
        break;
        case i_life2:
            current_lvl[player2.y][player2.x] = 0;
            lifes2 = lifes2 + 1;
        break;
    }
    // Enemigo
    for (short i = 0; i < sizeof(enemy)/sizeof(enemy[0]); i++) {
        if (player2.y == enemy[i].y &&
            player2.x == enemy[i].x)
        {
            lifes2 = lifes2 - 1; 
        }
    }
}


/**
 *  Funcion para manejar las colisiones de las balas del jugador 1
*/
void bullet_collision(short current_lvl[][arr_size_x]) {
    switch(current_lvl[bullet.y][bullet.x]) {
        case i_wall:
        case i_space:
            bullet_shoot = false;
        break;
    }
    // fin del recorrido sin colision
    if (current_lvl[bullet.y][bullet.x] == i_box) {
        current_lvl[bullet.y][bullet.x] = 0;
        bullet_shoot = false;
    }
    if (bullet_shoot) {
        for (int i = 0; i < sizeof(enemy)/sizeof(enemy[0]); i++) {
            // 
            if (enemy[i].ver != 0) {
                if (bullet.y == enemy[i].y &&
                    bullet.x == enemy[i].x || 
                    bullet.y - enemy[i].direction == enemy[i].y &&
                    bullet.x == enemy[i].x)
                {
                    enemy[i].y = 0;
                    enemy[i].x = 0;
                    enemy[i].direction = 0;
                    bullet_shoot = false;
                    break;
                }
            }

            // Horizontal collision
            if (enemy[i].hor != 0) {
                if (bullet.y == enemy[i].y &&
                    bullet.x == enemy[i].x || 
                    bullet.y == enemy[i].y &&
                    bullet.x - enemy[i].direction == enemy[i].x)
                {
                    enemy[i].y = 0;
                    enemy[i].x = 0;
                    enemy[i].direction = 0;
                    bullet_shoot = false;
                    break;
                }
            }
        }

        // Check collision with player2
        if (bullet.y == player2.y && bullet.x == player2.x) {
            lifes2 = lifes2 - 1;
            bullet_shoot = false;
        }
    }

}

/**
 *  Funcion para manejar las colisiones de las balas del jugador 2
*/
void bullet2_collision(short current_lvl[][arr_size_x]) {
    switch(current_lvl[bullet2.y][bullet2.x]) {
        case i_wall:
        case i_space:
            bullet2_shoot = false;
        break;
    }

    // Kill Box
    if (current_lvl[bullet2.y][bullet2.x] == i_box) {
        current_lvl[bullet2.y][bullet2.x] = 0;
        bullet2_shoot = false;
    }

    // Kill Enemy
    if (bullet2_shoot) {
        for (int i = 0; i < sizeof(enemy)/sizeof(enemy[0]); i++) {
            // Vertival collision
            if (enemy[i].ver != 0) {
                if (bullet2.y == enemy[i].y &&
                    bullet2.x == enemy[i].x || 
                    bullet2.y - enemy[i].direction == enemy[i].y &&
                    bullet2.x == enemy[i].x)
                {
                    enemy[i].y = 0;
                    enemy[i].x = 0;
                    enemy[i].direction = 0;
                    bullet2_shoot = false;
                    break;
                }
            }

            // Horizontal collision
            if (enemy[i].hor != 0) {
                if (bullet2.y == enemy[i].y &&
                    bullet2.x == enemy[i].x || 
                    bullet2.y == enemy[i].y &&
                    bullet2.x - enemy[i].direction == enemy[i].x)
                {
                    enemy[i].y = 0;
                    enemy[i].x = 0;
                    enemy[i].direction = 0;
                    bullet2_shoot = false;
                    break;
                }
            }
        }

        // Check collision with player1
        if (bullet2.y == player.y && bullet2.x == player.x) {
            lifes = lifes - 1;  // Decrease player1's life
            bullet2_shoot = false;
        }
    }

}

/**
 *  Funcion para actualizar el nivel
*/
void set_lvl_param(short current_lvl[][arr_size_x], int clx, int cly) {
    static int i = 0;
    for (int y = 0; y < cly; y++) {
        for (int x = 0; x < clx; x++) {
            if (current_lvl[y][x] == i_enemy_v) {
                obj_init(&enemy[i], x, y, 1, "");
                i++;
            } else if (current_lvl[y][x] == i_enemy_h) {
                obj_init(&enemy[i], x, y, 2, "");
                i++;
            }

            if (i >= sizeof(enemy)/sizeof(enemy[0])) {
                i = 0;
            }
        }
    }

}


/**
 *  Funcion para revisar si hay que hacer cambio de nivel
*/
bool next_lvl(short current_lvl[][arr_size_x]) {
    if (lifes == 0 || lifes2 == 0) {
        if (lifes == 0){
            wins2++;
        }
        if (lifes2 == 0){
            wins1++;
        }
        level = level + 1;
        return true;
    }

    return false;
}

/**
 *  Funcion para dibujar en consola el nivel
*/
void draw_level(short lvl[][arr_size_x]) {
    for (int y = 0; y < current_lvl_y; y++) {
        for (int x = 0; x < current_lvl_x; x++) {
            switch(lvl[y][x]) {

                // Draw static object
                case i_wall:   draw_instance(y, x, c_wall,  s_wall);  break;
                case i_box:    draw_instance(y, x, c_box,   s_box);   break;
                case i_space:  draw_instance(y, x, c_wall,  s_space); break;
                case i_life:   draw_instance(y, x, c_life,  s_life);  break;
                case i_life2:  draw_instance(y, x, c_life2,  s_life2);  break;
                case i_exit:   draw_instance(y, x, c_life,  s_exit);  break;

                // Draw dynamic object
                default:

                    // Draw player
                    if (x == player.x && y == player.y) {
                        draw_instance(y, x, c_player, player.symbol);
                        break;
                    }

                    // Draw player2
                    if (x == player2.x && y == player2.y) {
                        draw_instance(y, x, c_player2, player2.symbol);
                        break;
                    }

                    // Draw bullet
                    if (x == bullet.x && y == bullet.y) {
                        if (!bullet_shoot) {
                            break;
                        }
                        draw_instance(y, x, c_bullet, bullet.symbol);
                    }

                    // Draw bullet2
                    if (x == bullet2.x && y == bullet2.y) {
                        if (!bullet2_shoot) {
                            break;
                        }
                        draw_instance(y, x, c_bullet2, bullet2.symbol);
                    }

                    // Draw enemy
                    for (int i = 0; i < 5; i++) {
                        if (x == enemy[i].x && y == enemy[i].y) {
                            draw_instance(y, x, c_enemy, s_enemy);
                            break;
                        }
                    }

                break;
            }
        }
    }
}

/**
 *  Funcion para dibujar el HUD
*/
void draw_hud() {
    mvprintw(1, 2, "Player 1 HP: %d   Player 2 HP: %d   level: %d   Player 1 Points: %d   Player 2 Points: %d\n", lifes, lifes2, level, wins1, wins2);
}

/**
 *  Funcion para gestionar el Game Over
*/
void game_over() {
    EXIT = true;
    endwin();
    if (wins1 > wins2) {
        printf("Player 1 Wins!\n");
    }
    if (wins1 < wins2) {
        printf("Player 2 Wins!\n");
    }
}

/**
 *  Funcion para actualizar la bala del jugador 1
*/
void bullet_update(void) {
    if (!bullet_shoot) {
            bullet.x = player.x;
            bullet.y = player.y;
        if (key_pressed == vk_space) {
            bullet.direction = dir_shoot;
            bullet_shoot = true;
        }
    } else {
        switch(bullet.direction) {
            case 1:
                bullet.hor = 1;
                bullet.ver = 0;
            break;

            case -1:
                bullet.hor = -1;
                bullet.ver = 0;
            break;

            case 2:
                bullet.hor = 0;
                bullet.ver = 1;
            break;

            case -2:
                bullet.hor = 0;
                bullet.ver = -1;
            break;
        }

        bullet.x += bullet.hor;
        bullet.y += bullet.ver;
    }
}

/**
 *  Funcion para actualizar la bala del jugador 2
*/
void bullet2_update(void) {
    if (!bullet2_shoot) {
            bullet2.x = player2.x;
            bullet2.y = player2.y;
        if (key_pressed == vk_f) {
            bullet2.direction = dir2_shoot;
            bullet2_shoot = true;
        }
    } else {
        switch(bullet2.direction) {
            case 1:
                bullet2.hor = 1;
                bullet2.ver = 0;
            break;

            case -1:
                bullet2.hor = -1;
                bullet2.ver = 0;
            break;

            case 2:
                bullet2.hor = 0;
                bullet2.ver = 1;
            break;

            case -2:
                bullet2.hor = 0;
                bullet2.ver = -1;
            break;
        }

        bullet2.x += bullet2.hor;
        bullet2.y += bullet2.ver;
    }
}

/**
 *  Funcion para actualizar el juego en general
*/
void game_update(int key, short current_lvl[][arr_size_x]) {
    player_move(key);
    player_collision(current_lvl);
    player2_move(key);
    player2_collision(current_lvl);
    enemy_update(current_lvl);
    bullet_update();
    bullet_collision(current_lvl);
    bullet2_update();
    bullet2_collision(current_lvl);
    draw_level(current_lvl);
}

/**
 *  Funcion para actualizar la bala del jugador 1
*/
void level_init(short index_lvl) {
    static bool init = true;

    if (!init) {
        if (index_lvl == 1) { init = next_lvl(lvl_one);   game_update(key_pressed, lvl_one);   }
        if (index_lvl == 2) { init = next_lvl(lvl_two);   game_update(key_pressed, lvl_two);   }
        if (index_lvl == 3) { init = next_lvl(lvl_three); game_update(key_pressed, lvl_three); }
        if (index_lvl == 4) { init = next_lvl(lvl_fo);    game_update(key_pressed, lvl_fo);    }
        if (index_lvl == 5) { init = next_lvl(lvl_five);  game_update(key_pressed, lvl_five);  }
        if (index_lvl == 6) { init = next_lvl(lvl_six);   game_update(key_pressed, lvl_six);   }
        if (index_lvl == 7) { init = next_lvl(lvl_last);   game_update(key_pressed, lvl_last);   }
        
        return;
    }

    switch(index_lvl) {
        case 1:
            player.x = 2;
            player.y = 2;
            player2.x = 19;
            player2.y = 15; 
            current_lvl_x = lvl_one_x;
            current_lvl_y = lvl_one_y;
            arr_size_x = level_one_size;
            clear_enemy();
            set_lvl_param(lvl_one, current_lvl_x, current_lvl_y);
            init = false;
        break;

        case 2:
            lifes = 3;
            lifes2 = 3;
            player.x = 2;
            player.y = 2;
            player2.x = 19;
            player2.y = 15;
            current_lvl_x = lvl_two_x;
            current_lvl_y = lvl_two_y;
            arr_size_x = level_two_size;
            clear_enemy();
            set_lvl_param(lvl_two, current_lvl_x, current_lvl_y);
            init = false;
        break;

        case 3:
            lifes = 3;
            lifes2 = 3;
            player.x = 2;
            player.y = 2;
            player2.x = 19;
            player2.y = 15;
            current_lvl_x = lvl_three_x;
            current_lvl_y = lvl_three_y;
            arr_size_x = level_three_size;
            clear_enemy();
            set_lvl_param(lvl_three, current_lvl_x, current_lvl_y);
            init = false;
        break;

        case 4:
            lifes = 3;
            lifes2 = 3;
            player.x = 2;
            player.y = 2;
            player2.x = 19;
            player2.y = 15;
            current_lvl_x = lvl_fo_x;
            current_lvl_y = lvl_fo_y;
            arr_size_x = level_fo_size;
            clear_enemy();
            set_lvl_param(lvl_fo, current_lvl_x, current_lvl_y);
            init = false;
        break;

        case 5:
            lifes = 3;
            lifes2 = 3;
            player.x = 2;
            player.y = 2;
            player2.x = 19;
            player2.y = 15;
            current_lvl_x = lvl_five_x;
            current_lvl_y = lvl_five_y;
            arr_size_x = level_five_size;
            clear_enemy();
            set_lvl_param(lvl_five, current_lvl_x, current_lvl_y);
            init = false;
        break;

        case 6:
            lifes = 3;
            lifes2 = 3;
            player.x = 2;
            player.y = 2;
            player2.x = 19;
            player2.y = 15;
            current_lvl_x = lvl_six_x;
            current_lvl_y = lvl_six_y;
            arr_size_x = level_six_size;
            clear_enemy();
            set_lvl_param(lvl_six, current_lvl_x, current_lvl_y);
            init = false;
        break;

        case 7:
            lifes = 3;
            lifes2 = 3;
            player.x = 2;
            player.y = 2;
            player2.x = 19;
            player2.y = 15;
            clear_enemy();
            set_lvl_param(lvl_last, current_lvl_x, current_lvl_y);
            init = false;
        break;
        case 8:
            game_over();
        break;
    }

}

/**
 *  Funcion main del programa donde se llaman a todas las otras funciones
*/
int main(void) {
    initscr();
    keypad(stdscr, TRUE);
    savetty();
    cbreak();
    noecho();
    timeout(0);
    leaveok(stdscr, TRUE);
    curs_set(0);
    if (!has_colors()) {
        endwin();
        printf("Error con los colores a manipular\n");
    }
    typedef enum {
        STATE_MENU,
        STATE_INFO,
        STATE_GAME,
        STATE_EXIT,
    } game_states;

    game_states current_state;
    current_state = STATE_MENU;
    obj_init(&player, 5 /* x pos */, 5 /* y pos */, 0, "|O|");
    obj_init(&player2, 2 /* x pos */, 2 /* y pos */, 0, "|X|");
    obj_init(&bullet, player.x, player.x, 0, " * ");
    obj_init(&bullet2, player2.x, player2.x, 0, " * ");
    // Efecto Start Game
    const char *item_start_game[2] = {
        "> START GAME <",
        "start game",
    };
    // Efecto Item
    const char *item_info[2] = {
        "> INFO <",
        "info",
    };
    // Efecto Exit
    const char *item_exit[2] = {
        "> EXIT <",
        "exit",
    };
    while (!EXIT) {
        SetColor();
        getmaxyx(stdscr, h, w);
        static int menu_item = 0;
        if (key_pressed == KEY_UP)   menu_item--;
        if (key_pressed == KEY_DOWN) menu_item++;
        if (menu_item >= 2) menu_item = 2;
        if (menu_item <= 0) menu_item = 0;
        switch(current_state) {
            case STATE_MENU:
                draw_logo(h, w);
                int select_start_game = menu_item == 0 ? 0 : 1;
                mvprintw(h/2 - logo_h_size + 9, w/2 - str_len(item_start_game[select_start_game])/2, item_start_game[select_start_game]);
                int select_info = menu_item == 1 ? 0 : 1;
                mvprintw(h/2 - logo_h_size + 11, w/2 - str_len(item_info[select_info])/2, item_info[select_info]);
                int select_exit = menu_item == 2 ? 0 : 1;
                mvprintw(h/2 - logo_h_size + 13, w/2 - str_len(item_exit[select_exit])/2, item_exit[select_exit]);
                mvprintw(h-2, 2, "Arquitecura de Computadores IS 2024");
                // caja del hud
                attron(COLOR_PAIR(c_hud));
                box(stdscr, 0, 0);
                attron(COLOR_PAIR(c_hud));

                if (key_pressed == vk_enter) { 
                    switch(menu_item) {
                        case 0:
                            current_state = STATE_GAME;
                        break;

                        case 1:
                            current_state = STATE_INFO;
                        break;

                        case 2:
                            current_state = STATE_EXIT;
                        break;
                    }
                }
            break;
            case STATE_INFO:
                mvprintw(h/2-len_yoff,   w/2-len_xoff, "Outlaw version Arqui ");
                mvprintw(h/2-len_yoff+1, w/2-len_xoff, "Trata de bajar la vida de tu contrincante");
                mvprintw(h/2-len_yoff+2, w/2-len_xoff, "Puedes recuperar vida si recoges paquetes de vida");
                mvprintw(h/2-len_yoff+3, w/2-len_xoff, "El mejor de 7 rondas gana");
                mvprintw(h/2-len_yoff+4, w/2-len_xoff, "Disfruten");
                mvprintw(h-4, w/2-ceil(len_xoff/2), "press 'q' to exit menu");
                box(stdscr, 0, 0);
            break;
            case STATE_GAME:
                level_init(level);
                draw_hud();
                box(stdscr, 0, 0);
            break;
            case STATE_EXIT:
                endwin();
                EXIT = TRUE;
            break;
        }
        if (key_pressed == 'q')
            current_state = STATE_MENU;

        key_pressed = wgetch(stdscr);
        napms(100);
        key_pressed = wgetch(stdscr);
        erase();
    }
    endwin();
    return 0;
}
