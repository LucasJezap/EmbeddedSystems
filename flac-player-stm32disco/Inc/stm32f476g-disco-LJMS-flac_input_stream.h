#pragma once

#include "Middlewares/Third_Party/FatFs/src/ff.h"

typedef struct
{
    FIL *file;
} InputStream;

InputStream init_input_stream_with_file(FIL *file);
int read_input_stream(InputStream *self, void *buf, int len);
void destroy_input_stream(InputStream *self);
