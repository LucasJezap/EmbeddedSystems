#include "stm32f476g-disco-LJMS-flac_main.h"

#include "stm32f476g-disco-LJMS-flac_search_files.h"
#include "log.h"
#include "stm32f476g-disco-LJMS-flac_player.h"
#include "stm32f476g-disco-LJMS-flac_screen.h"
#include "Inc/usb_host.h"

extern ApplicationTypeDef Appli_state;

static Files files;
static int current_file = 0;

/**
 * this function returns the filepath of current file
 */
static const char *get_filepath(void)
{
    /* 3 for 0:/ and 1 for end */
    static char current_path[3 + MAX_PATH_LENGTH + 1];
    snprintf(current_path, sizeof(current_path), "0:/%s", files.files[current_file]);
    return current_path;
}

/**
 * this function changes the state of flac player e.g. from paused to playing
 */
static void change_flac_player_state(void)
{
    switch (get_flac_player_state())
    {
    case Stopped:
        flac_player_play(get_filepath());
        break;

    case Playing:
        flac_player_pause();
        break;

    case Paused:
        flac_player_resume();
        break;
    }
}

/**
 * this function waits for USB device to be plugged in (containing .flac files to play)
 */
static void usb_wait(void)
{
    xprintf("waiting for USB...\n");
    int i = 0;
    do
    {
        render_usb(i);
        xprintf(".");
        osDelay(250);
        i = (i + 1) % 3;
    } while (Appli_state != APPLICATION_READY);
    xprintf("USB is ready.\n");
}

/**
 * this function makes flac player go to previous song
 */
static void go_to_previous_song(void)
{
    if (get_flac_player_state() != Stopped)
    {
        flac_player_stop();
    }

    current_file = (current_file - 1 + files.count) % files.count;
    flac_player_play(get_filepath());
}

/**
 * this function makes flac player go to next song
 */
static void go_to_next_song(void)
{
    if (get_flac_player_state() != Stopped)
    {
        flac_player_stop();
    }

    current_file = (current_file + 1) % files.count;
    flac_player_play(get_filepath());
}

/**
 * this is the main function of the flac player 
 * it initializes screen, waits for USB, initializes flac player and indefinitely updates the flac player
 * (changes songs, screen and so on)
 */
void main_task(void)
{
    /* Three times to make sure screen is correctly initialized */
    init_screen();
    init_screen();
    init_screen();

    /* Here we are waiting for USB with flac files */
    usb_wait();
    osDelay(500);

    /* Here we are searching for .flac files on the USB 0: */
    display_info_on_screen("Searching FLAC files...");
    osDelay(500);
    search_for_flac_files("0:", &files);

    /* Here we initialize flac player */
    display_info_on_screen("Please wait...");
    osDelay(500);
    init_flac_player();

    for (;;)
    {
        handle_touch();
        render_flac_player(
            files.count,
            current_file,
            files.files[current_file],
            get_flac_sample_rate(),
            get_flac_current_timer(),
            get_flac_max_timer(),
            get_flac_player_state() == Playing);

        if (is_play_button_touched())
        {
            change_flac_player_state();
        }
        else if (is_back_button_touched())
        {
            go_to_previous_song();
        }
        else if (is_next_button_touched())
        {
            go_to_next_song();
        }

        update_flac_player();
    }
}
