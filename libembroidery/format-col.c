#include "format-col.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <stdlib.h>

int readCol(EmbPattern* pattern, const char* fileName)
{
    int numberOfColors, i;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-col.c readCol(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-col.c readCol(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "r");
    if(!file)
    {
        /* NOTE: The .col format is an optional color file. Do not log an error if the file does not exist */
        return 0;
    }
    embThreadList_free(pattern->threadList);
    if(fscanf(file, "%d\r", &numberOfColors) < 1)
    {
        /* TODO: log error */
        return 0;
    }
    for(i = 0; i < numberOfColors; i++)
    {
        int num, blue, green, red;
        EmbThread t;
        char line[30];
        if(fscanf(file, "%s\r", line) < 1)
        {
            /* TODO: log error */
            return 0;
        }
        if(sscanf(line,"%d,%d,%d,%d\n\r", &num, &blue, &green, &red) != 4)
        {
            break;
        }
        t.catalogNumber = "";
        t.description = "";
        t.color.r = (unsigned char)red;
        t.color.g = (unsigned char)green;
        t.color.b = (unsigned char)blue;
        embPattern_addThread(pattern, t);
    }
    fclose(file);
    return 1;
}

int writeCol(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    int i, colorCount;
    EmbThreadList *colors;

    if(!pattern) { embLog_error("format-col.c writeCol(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-col.c writeCol(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "w");
    if(!file)
    {
        embLog_error("format-col.c writeCol(), cannot open %s for writing\n", fileName);
        return 0;
    }
    colorCount = embThreadList_count(pattern->threadList);
    fprintf(file, "%d\n\r", colorCount);
    colors = pattern->threadList;
    i = 0;
    while(colors)
    {
        EmbColor c;
        c = colors->thread.color;
        fprintf(file, "%d,%d,%d,%d\n\r", i, (int)c.r, (int)c.g, (int)c.b);
        i++;
        colors = colors->next;
    }
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
