#include "helpers.h"
#include "main.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t h_fsize(const char *fname)
{
    struct stat info;

    stat(fname, &info);

    return (size_t)info.st_size;
}

char *h_read_data_from_file(const char *fname, size_t flen)
{
    FILE *fi;
    char *fdata = NULL;

    if ((fi = fopen(fname, "r")) == NULL) {
        ERR_PRNT("can't open <%s> file", fname);

        return NULL;
    }

    if ((fdata = (char *)malloc(sizeof(char) * flen)) == NULL) {
        ERR_PRNT("can't allocate memory for data of <%s>", fname);

        return NULL;
    }

    if (flen != fread((void *)fdata, sizeof(char), flen, fi)) {
        ERR_PRNT("can't read all file data\n");

        return NULL;
    }

    return fdata;
}

int h_save_data_to_file(const char *orig_fname,
            const char *end, char *d, size_t dlen)
{
    FILE *fo;
    char *fo_name;
    size_t fo_len = strlen(orig_fname) + strlen(end);

    /* memory for save new file name */
    fo_name = (char *)malloc(sizeof(char) * fo_len);

    if (fo_name) {
        sprintf(fo_name, "%s%s", orig_fname, end);

        DBG_PRNT("output file is: <%s>\n", fo_name);
    } else {
        ERR_PRNT("can't allocate data for output file name\n");

        return RET_ERR;
    }

    if ((fo = fopen(fo_name, "w")) == NULL) {
        ERR_PRNT("can't create out file <%s>\n", fo_name);

        free(fo_name);

        return RET_ERR;
    }

    (void)fwrite(d, sizeof(char), dlen, fo);

    free(fo_name);
    (void)fclose(fo);

    return RET_SUCCESS;
}
