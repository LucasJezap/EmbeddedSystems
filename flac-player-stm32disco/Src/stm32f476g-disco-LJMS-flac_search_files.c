#include "stm32f476g-disco-LJMS-flac_search_files.h"

#include "log.h"
#include "Middlewares/Third_Party/FatFs/src/ff.h"

/**
 * this function searches for all files ending with .flac on the plugged USB and it saves it in <files> custom structure
 */
void search_for_flac_files(const char *path, Files *files)
{
    files->count = 0;
    DIR dir;
    FRESULT fres;

    fres = f_opendir(&dir, path);
    if (fres != FR_OK)
    {
        log_error("error: f_opendir\n");
        return;
    }

    while (files->count < MAX_NUMBER_OF_FILES - 1)
    {
        FILINFO file;
        fres = f_readdir(&dir, &file);
        if (fres != FR_OK)
        {
            log_error("error: f_readdir\n");
            return;
        }

        if (file.fname[0] == '\0')
        {
            break;
        }

        if (!(file.fattrib & AM_DIR))
        {
            int len = strlen(file.fname);
            /* check if it's a flac file */
            if (len > 5 && len <= MAX_PATH_LENGTH && strcmp(&file.fname[len - 5], ".flac") == 0)
            {
                xprintf("flac file found: %s\n", file.fname);
                strncpy(files->files[files->count], file.fname, MAX_PATH_LENGTH);
                files->count++;
            }
        }
    }

    f_closedir(&dir);
}
