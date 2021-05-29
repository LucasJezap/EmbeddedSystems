#pragma once

#define MAX_NUMBER_OF_FILES 20
#define MAX_PATH_LENGTH 50

typedef struct
{
    char files[MAX_NUMBER_OF_FILES][MAX_PATH_LENGTH + 1];
    int count;
} Files;

void search_for_flac_files(const char *path, Files *files);