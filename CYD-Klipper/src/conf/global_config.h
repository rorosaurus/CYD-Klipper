#ifndef _GLOBAL_CONFIG_INIT
#define _GLOBAL_CONFIG_INIT

#include "lvgl.h"

#define CONFIG_VERSION 6
#define PRINTER_CONFIG_COUNT 8
#define DISPLAY_SECRETS 0

enum {
    REMAINING_TIME_CALC_PERCENTAGE = 0,
    REMAINING_TIME_CALC_INTERPOLATED = 1,
    REMAINING_TIME_CALC_SLICER = 2,
};

enum {
    SHOW_STATS_ON_PROGRESS_PANEL_NONE = 0,
    SHOW_STATS_ON_PROGRESS_PANEL_LAYER = 1,
    SHOW_STATS_ON_PROGRESS_PANEL_PARTIAL = 2,
    SHOW_STATS_ON_PROGRESS_PANEL_ALL = 3,
};

typedef struct _PRINTER_CONFIG {
    union {
        unsigned int raw;
        struct {
            // Internal
            bool ip_configured : 1;
            bool auth_configured : 1;

            // External
            bool light_mode : 1;
            bool invert_colors : 1;
            unsigned char remaining_time_calc_mode : 2;
            unsigned char show_stats_on_progress_panel : 2;

            bool custom_filament_move_macros : 1;
        };
    };

    char printer_name[25];
    char klipper_host[65];
    char klipper_auth[33];
    unsigned short klipper_port;
    
    unsigned char color_scheme;

    unsigned short hotend_presets[3];
    unsigned short bed_presets[3];

    unsigned short printer_move_x_steps[3];
    unsigned short printer_move_y_steps[3];
    unsigned short printer_move_z_steps[3];
} PRINTER_CONFIG;

typedef struct _GLOBAL_CONFIG {
    unsigned char version;
    union {
        unsigned int raw;
        struct {
            // Internal
            bool screen_calibrated : 1;
            bool wifi_configured : 1;

            // External
            bool rotate_screen : 1;
            bool auto_ota_update : 1;
            bool multi_printer_mode : 1;
            bool on_during_print : 1;
            bool display_mode : 1; // Driver specifc usage. Currently only used on ESP32-2432S028R to fix the screen on the usb-c model
            bool disable_m117_messaging : 1;
            bool sort_macros : 1;
            bool show_estop : 1;
        };
    };

    PRINTER_CONFIG printer_config[PRINTER_CONFIG_COUNT];

    float screen_cal_x_offset;
    float screen_cal_x_mult;
    float screen_cal_y_offset;
    float screen_cal_y_mult;

    char wifi_SSID[33];
    char wifi_password[65];
    
    unsigned char brightness;
    unsigned char screen_timeout;
    unsigned char printer_index;
} GLOBAL_CONFIG;

// Volatile/temporary config that doesn't survive a reset
typedef struct _TEMPORARY_CONFIG {
    bool debug : 1;
    bool remote_echo : 1;
} TEMPORARY_CONFIG;
    

typedef struct _COLOR_DEF {
    lv_palette_t primary_color;
    short primary_color_light;
    lv_palette_t secondary_color;
} COLOR_DEF;

extern GLOBAL_CONFIG global_config;
extern TEMPORARY_CONFIG temporary_config;
extern COLOR_DEF color_defs[];

#define LOG(x) if(temporary_config.debug){ Serial.print(x);}
#define LOG_LN(x) if(temporary_config.debug){ Serial.println(x);}
#define LOG_F(x) if(temporary_config.debug){ Serial.printf x ;}   // use with double braces, LOF_F(("x=%d\n",x));

void write_global_config();
void verify_version();
void load_global_config();

PRINTER_CONFIG* get_current_printer_config();
int get_printer_config_count();
void set_printer_config_index(int index);
int get_printer_config_free_index();

#endif // !_GLOBAL_CONFIG_INIT