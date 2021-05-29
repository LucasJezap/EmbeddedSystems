#include "stm32f476g-disco-LJMS-flac_input_stream.h"

#include "log.h"

/**
 * this function reads <len> bytes from input stream to buffer buf
 */
int read_input_stream(InputStream *self, void *buf, int len)
{
    UINT bytes_read;
    FRESULT rc = f_read(self->file, buf, (UINT)len, &bytes_read);

    if (rc != FR_OK)
    {
        log_error("error: f_read failed\n");
        return -1;
    }

    return (int)bytes_read;
}

/**
 * this function destroys the input stream by freeing memory and nullying fields
 */
void destroy_input_stream(InputStream *self)
{
    f_close(self->file);
    *self = (InputStream){
        .file = NULL};
}

/**
 * this function returns new InputStream structure with file field populated
 */
InputStream init_input_stream_with_file(FIL *file)
{
    return (InputStream){
        .file = file};
}
