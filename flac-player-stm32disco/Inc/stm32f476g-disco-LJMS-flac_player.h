#pragma once

typedef enum
{
    Stopped,
    Playing,
    Paused
} PlayerState;

void init_flac_player(void);
void update_flac_player(void);

PlayerState get_flac_player_state(void);
unsigned get_flac_sample_rate(void);
unsigned get_flac_current_timer(void);
unsigned get_flac_max_timer(void);

void flac_player_play(const char *filename);
void flac_player_pause(void);
void flac_player_resume(void);
void flac_player_stop(void);