#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "stm32f476g-disco-LJMS-flac_input_stream.h"

typedef struct Flac Flac;

typedef struct
{
    unsigned sample_rate;
    unsigned channels;
    unsigned bits_per_sample;
    uint64_t total_samples;
} FlacMetadata;

typedef struct
{
    uint8_t *buffer;
    int size;
    int samples;
} FlacFrame;

Flac *create_new_flac(InputStream *input);

bool read_flac_metadata(Flac *flac, FlacMetadata *info);

bool read_flac_frame(Flac *flac, FlacFrame **frame);

void destroy_flac_frame(FlacFrame *frame);

void destroy_flac_metadata(FlacMetadata *info);

void destroy_flac(Flac *flac);