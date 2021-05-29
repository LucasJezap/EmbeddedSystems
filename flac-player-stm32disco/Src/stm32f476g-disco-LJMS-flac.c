#include "stm32f476g-disco-LJMS-flac.h"

#include "log.h"
#include "LIBFLAC/include/FLAC/stream_decoder.h"

struct Flac
{
    FLAC__StreamDecoder *decoder;
    InputStream *input;
    FlacMetadata info;
    FlacFrame *frame;
};

/**
 * this function is a callback for read event in flac decoder
 * it reads bytes from input stream and returns appropriate flag e.g. END_OF_STREAM
 */
static FLAC__StreamDecoderReadStatus read_callback(
    const FLAC__StreamDecoder *decoder,
    FLAC__byte *buffer,
    size_t *bytes,
    void *client_data)
{
    xprintf("flac_player: read_callback\n");

    Flac *flac = (Flac *)client_data;

    if (*bytes <= 0)
    {
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
    }

    int read_bytes = read_input_stream(flac->input, buffer, *bytes);

    if (read_bytes > 0)
    {
        *bytes = (size_t)read_bytes;
        return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
    }
    else if (read_bytes == 0)
    {
        *bytes = 0;
        return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
    }
    else
    {
        *bytes = 0;
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
    }
}

/**
 * this function is a callback for write event in flac decoder
 * it first checks if buffer has correct data (no NULLs allowed)
 * then it fill frame buffer structure with appropriate values`
 */
static FLAC__StreamDecoderWriteStatus write_callback(
    const FLAC__StreamDecoder *decoder,
    const FLAC__Frame *frame,
    const FLAC__int32 *const *buffer,
    void *client_data)
{
    xprintf("flac_player: write_callback\n");

    Flac *flac = (Flac *)client_data;

    for (int i = 0; i < frame->header.channels; i++)
    {
        if (buffer[i] == NULL)
        {
            log_error("error: buffer[%d] is NULL\n", i);
            return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
        }
    }

    int samples = frame->header.blocksize;
    int channels = frame->header.channels;
    int bytes_per_sample = frame->header.bits_per_sample / 8;
    int size = samples * channels * bytes_per_sample;

    flac->frame = malloc(sizeof(FlacFrame));
    *flac->frame = (FlacFrame){
        .size = size,
        .buffer = malloc(size)};

    for (int sample = 0; sample < samples; sample++)
    {
        for (int channel = 0; channel < channels; channel++)
        {
            for (int byte = 0; byte < bytes_per_sample; byte++)
            {
                flac->frame->buffer[(sample * channels + channel) * bytes_per_sample + byte] =
                    (uint8_t)((buffer[channel][sample] >> (byte * 8)));
            }
        }
    }

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

/**
 * this function is a callback for metadata event in flac decoder
 * it saves important values in global variables such as total samples or sample rate
 */
static void metadata_callback(
    const FLAC__StreamDecoder *decoder,
    const FLAC__StreamMetadata *metadata,
    void *client_data)
{
    xprintf("flac_player: metadata_callback\n");

    Flac *flac = (Flac *)client_data;

    if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO)
    {
        flac->info = (FlacMetadata){
            .total_samples = metadata->data.stream_info.total_samples,
            .sample_rate = metadata->data.stream_info.sample_rate,
            .channels = metadata->data.stream_info.channels,
            .bits_per_sample = metadata->data.stream_info.bits_per_sample};

        xprintf("total samples = %lu\n", (unsigned long)flac->info.total_samples);
        xprintf("sample rate = %u\n", flac->info.sample_rate);
        xprintf("channels = %u\n", flac->info.channels);
        xprintf("bits per sample = %u\n", flac->info.bits_per_sample);
    }
}

/**
 * this function is a callback for error event in flac decoder
 */
static void error_callback(
    const FLAC__StreamDecoder *decoder,
    FLAC__StreamDecoderErrorStatus status,
    void *client_data)
{
    log_error("Got error callback: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
}

/**
 * this function creates new Flac structure using methods from libFLAC ...decoder_new and ...init_stream
 * init_stream takes appropriate callback functions that happen during certain events
 */
Flac *create_new_flac(InputStream *input)
{
    Flac *flac = malloc(sizeof(Flac));
    *flac = (Flac){
        .input = input};

    flac->decoder = FLAC__stream_decoder_new();
    if (flac->decoder == NULL)
    {
        log_error("error: allocating decoder\n");
        destroy_flac(flac);
        return NULL;
    }

    FLAC__stream_decoder_set_md5_checking(flac->decoder, true);

    FLAC__StreamDecoderInitStatus init_status = FLAC__stream_decoder_init_stream(
        flac->decoder,
        &read_callback,
        NULL,
        NULL,
        NULL,
        NULL,
        &write_callback,
        &metadata_callback,
        &error_callback,
        flac);
    if (init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK)
    {
        log_error("error: initializing decoder: %s\n", FLAC__StreamDecoderInitStatusString[init_status]);
        destroy_flac(flac);
        return NULL;
    }

    return flac;
}

/**
 * this function destroys the Flac structure by using method from libFLAC ...decoder_delete and freeing it's memory
 */
void destroy_flac(Flac *flac)
{
    if (flac != NULL)
    {
        if (flac->decoder != NULL)
        {
            FLAC__stream_decoder_delete(flac->decoder);
        }
        free(flac);
    }
}

/**
 * this function reads the flac metadata using method from libFLAC ...process_until_end_of_metadata
 */
bool read_flac_metadata(Flac *flac, FlacMetadata *info)
{
    if (FLAC__stream_decoder_process_until_end_of_metadata(flac->decoder))
    {
        *info = flac->info;
        return true;
    }
    else
    {
        log_error("error: reading metadata: %s\n",
                  FLAC__StreamDecoderStateString[FLAC__stream_decoder_get_state(flac->decoder)]);
        return false;
    }
}

/**
 * this function reads the flac frame using method from libFLAC ...process_single
 */
bool read_flac_frame(Flac *flac, FlacFrame **frame)
{
    unsigned int t = xTaskGetTickCount();

    if (FLAC__stream_decoder_process_single(flac->decoder))
    {
        if (flac->frame == NULL)
        {
            return false;
        }
        *frame = flac->frame;
        flac->frame = NULL;

        t = xTaskGetTickCount() - t;
        xprintf("read_flac_frame: frame read with size: %d in (time) %u ms\n", (*frame)->size, t);
        return true;
    }
    else
    {
        log_error("error: reading frame: %s\n",
                  FLAC__StreamDecoderStateString[FLAC__stream_decoder_get_state(flac->decoder)]);
        return false;
    }
}

/**
 * this function destroys the FlacMetadata structure by freeing it's memory
 */
void destroy_flac_metadata(FlacMetadata *info)
{
    free(info);
}

/**
 * this function destroys the FlacFrame structure by freeing it's memory
 */
void destroy_flac_frame(FlacFrame *frame)
{
    free(frame->buffer);
    free(frame);
}
