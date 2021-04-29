#pragma once

#include "stm32f476g-disco-flac.h"

typedef struct {
    Flac* flac;
    FlacFrame* frame;
    int position;
} FlacBuffer;

FlacBuffer FlacBuffer_New(Flac* flac);
void FlacBuffer_Destroy(FlacBuffer* self);
int FlacBuffer_Read(FlacBuffer* self, void* dest, int size);