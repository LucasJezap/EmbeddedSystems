#include "stm32f476g-disco-LJMS-flac_frame_buffer.h"

#include "log.h"

/**
 * this function creates a new flac buffer to store flac frames
 */
FlacBuffer create_new_flac_buffer(Flac *flac)
{
    return (FlacBuffer){
        .flac = flac,
        .frame = NULL,
        .position = 0};
}

/**
 * this function destroys the existing flac buffer by freeing memory and nullying it
 */
void destroy_flac_buffer(FlacBuffer *self)
{
    free(self->frame);
    *self = (FlacBuffer){
        .flac = NULL,
        .frame = NULL,
        .position = 0};
}

/**
 * this function reads <size> bytes from flac buffer to buffer dest
 * it gets the next frame and copies bytes from it to buffer 
 * if the frame is finished - it gets next frame and continues
 */
int read_flac_buffer(FlacBuffer *self, void *dest, int size)
{
    xprintf("read_flac_buffer: begin\n");
    int written = 0;
    while (written < size)
    {
        xprintf("read_flac_buffer (while): written = %d\n", written);

        if (self->frame == NULL)
        {
            self->position = 0;
            if (!read_flac_frame(self->flac, &self->frame))
            {
                xprintf("read_flac_buffer: finished\n");
                return written;
            }
        }

        xprintf("frame read, frame size = %d\n", self->frame->size);

        int frame_left = self->frame->size - self->position;
        int dest_space_left = size - written;

        if (frame_left <= dest_space_left)
        {
            memcpy(dest + written, &self->frame->buffer[self->position], frame_left);
            written += frame_left;
            destroy_flac_frame(self->frame);
            self->frame = NULL;
            self->position = 0;
        }
        else
        {
            memcpy(dest + written, &self->frame->buffer[self->position], dest_space_left);
            written += dest_space_left;
            self->position += dest_space_left;
        }
    }

    xprintf("read_flac_buffer (end): written = %d\n", written);

    return written;
}
