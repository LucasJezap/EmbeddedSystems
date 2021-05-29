#pragma once

#include "stm32f476g-disco-LJMS-flac.h"

typedef struct
{
    Flac *flac;
    FlacFrame *frame;
    int position;
} FlacBuffer;

FlacBuffer create_new_flac_buffer(Flac *flac);
void destroy_flac_buffer(FlacBuffer *self);
int read_flac_buffer(FlacBuffer *self, void *dest, int size);