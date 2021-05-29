#include "stm32f476g-disco-LJMS-flac_player.h"

#include <assert.h>
#include <stdint.h>

#include "stm32f476g-disco-LJMS-flac.h"
#include "stm32f476g-disco-LJMS-flac_frame_buffer.h"
#include "log.h"
#include "Drivers/BSP/Components/wm8994/wm8994.h"
#include "Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_audio.h"

// buffer for audio data - it need to be big enough but not too big
// below value is experimental one
#define AUDIO_OUT_BUFFER_SIZE 130000

typedef enum
{
    None = 0,
    TransferredFirstHalf,
    TransferredSecondHalf,
} TransferEvent;

static uint8_t audio_buffer[AUDIO_OUT_BUFFER_SIZE];
static uint8_t audio_transfer_event = None;
static unsigned last_audio_transfer_event_time = 0;

static PlayerState state = Stopped;
static uint64_t samples_played;
static FIL file;
static InputStream input_stream;
static FlacBuffer flac_buffer;
static Flac *flac;
static FlacMetadata flac_info;

/**
 * this function returns the current event and changes it to None
 */
static TransferEvent get_transfer_event(void)
{
    TransferEvent event = audio_transfer_event;
    audio_transfer_event = None;
    return event;
}

/**
 * this function is a callback for Half Transfer event - when the middle of frame buffer was reached
 */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
    audio_transfer_event = TransferredFirstHalf;
    unsigned t = osKernelSysTick();
    xprintf("[%u] BSP_AUDIO_OUT_HalfTransfer_CallBack (%u)\n", t, t - last_audio_transfer_event_time);
    last_audio_transfer_event_time = t;
}

/**
 * this function is a callback for Complete Transfer event - when the end of frame buffer was reached
 */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
    audio_transfer_event = TransferredSecondHalf;
    unsigned t = osKernelSysTick();
    xprintf("[%u] BSP_AUDIO_OUT_TransferComplete_CallBack (%u)\n", t, t - last_audio_transfer_event_time);
    last_audio_transfer_event_time = t;
}

/**
 * this function is initializing the flac player -> with frequency 44.1kHz, headphones mode, volume 60 and audioframe slot 02
 */
void init_flac_player(void)
{
    xprintf("initializing audio...\n");
    if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, 75, AUDIO_FREQUENCY_44K) != 0)
    {
        log_fatal_and_die(" error\n");
    }
    BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
}

/**
 * this function updates the flac player -> only if it's currently playing
 */
void update_flac_player(void)
{
    if (state == Playing)
    {
        TransferEvent event = get_transfer_event();
        if (event)
        {
            unsigned t1 = osKernelSysTick();
            xprintf("[%u] handling event\n", t1);

            uint32_t offset = (event == TransferredFirstHalf)
                                  ? 0
                                  : AUDIO_OUT_BUFFER_SIZE / 2;

            int bytes_read = read_flac_buffer(&flac_buffer, &audio_buffer[offset], AUDIO_OUT_BUFFER_SIZE / 2);

            samples_played += bytes_read / flac_info.channels / (flac_info.bits_per_sample / 8);

            if (bytes_read < AUDIO_OUT_BUFFER_SIZE / 2)
            {
                xprintf("stop at eof\n");
                flac_player_stop();
            }

            unsigned t2 = osKernelSysTick();
            xprintf("[%u] completed handling event in (%u)\n", t2, t2 - t1);
        }
    }
}

/**
 * this function returns the current state of flac player
 */
PlayerState get_flac_player_state(void)
{
    return state;
}

/**
 * this function returns the sample rate of current .flac file
 */
unsigned get_flac_sample_rate(void)
{
    return flac_info.sample_rate;
}

/**
 * this function returns the current timer of current .flac file
 */
unsigned get_flac_current_timer(void)
{
    return samples_played;
}

/**
 * this function returns the total timer of current .flac file
 */
unsigned get_flac_max_timer(void)
{
    return flac_info.total_samples;
}

/**
 * this function starts playing the current .flac file
 */
void flac_player_play(const char *filename)
{
    xprintf("flac_player_play: '%s'\n", filename);

    assert(state == Stopped);

    state = Playing;

    xprintf("opening FLAC file...\n");
    FRESULT res = f_open(&file, filename, FA_READ);
    if (res != FR_OK)
    {
        log_fatal_and_die(" error, res = %d\n", res);
    }

    xprintf("initializing decoder...\n");
    input_stream = init_input_stream_with_file(&file);
    flac = create_new_flac(&input_stream);
    flac_buffer = create_new_flac_buffer(flac);

    xprintf("reading FLAC metadata...\n");
    if (!read_flac_metadata(flac, &flac_info))
    {
        log_fatal_and_die(" error\n");
    }

    xprintf("filling first half of buffer...\n");
    int bytes_read = read_flac_buffer(&flac_buffer, audio_buffer, AUDIO_OUT_BUFFER_SIZE / 2);
    if (bytes_read < AUDIO_OUT_BUFFER_SIZE / 2)
    {
        xprintf("stop at eof\n");
        flac_player_stop();
        return;
    }

    audio_transfer_event = TransferredSecondHalf;

    xprintf("starting hardware playing...\n");
    BSP_AUDIO_OUT_Play((uint16_t *)&audio_buffer[0], AUDIO_OUT_BUFFER_SIZE);
    BSP_AUDIO_OUT_Resume();

    xprintf("flac_player_play: finished\n");
}

/**
 * this function pauses the current .flac file
 */
void flac_player_pause(void)
{
    xprintf("flac_player_pause\n");

    assert(state == Playing);

    state = Paused;
    BSP_AUDIO_OUT_Pause();
}

/**
 * this function resumes the current .flac file
 */
void flac_player_resume(void)
{
    xprintf("flac_player_resume\n");

    assert(state == Paused);

    state = Playing;
    BSP_AUDIO_OUT_Resume();
}

/**
 * this function stops (hardware) the current .flac file
 */
void flac_player_stop(void)
{
    xprintf("flac_player_stop\n");

    assert(state == Playing || state == Paused);

    state = Stopped;

    xprintf("stopping hardware playing...\n");
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);

    xprintf("destroying decoder...\n");
    destroy_flac_buffer(&flac_buffer);
    destroy_flac(flac);
    flac = NULL;
    destroy_input_stream(&input_stream);

    xprintf("closing file...\n");
    f_close(&file);

    samples_played = 0;

    xprintf("flac_player_stop: finished\n");
}
