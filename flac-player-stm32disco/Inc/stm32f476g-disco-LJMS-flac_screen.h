#pragma once

#include <stdbool.h>

void init_screen(void);

void display_info_on_screen(const char *info);

void render_usb(int which_big);

void render_flac_player(
    int number_of_files,
    int current_file,
    const char *current_file_name,
    unsigned sample_rate,
    unsigned current_timer,
    unsigned max_timer,
    bool is_playing);

void handle_touch(void);

bool is_back_button_touched(void);
bool is_play_button_touched(void);
bool is_next_button_touched(void);
