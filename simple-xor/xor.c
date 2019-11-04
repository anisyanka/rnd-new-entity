#include "xor.h"
#include "main.h"
#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int check_key_and_fname(const char *fname, const char *key,
                size_t *klen, size_t *flen)
{
    if ((fname) && (key)) {
        *klen = strlen(key);
        *flen = h_fsize((const char *)fname);

        if (*klen > *flen) {
            ERR_PRNT("key length more than file size\n");

            return RET_ERR;
        }

        if (*flen > MAX_FILE_SIZE) {
            ERR_PRNT("file <%s> is large. Max is %d MB\n", 
                        fname, MAX_FILE_SIZE / PREFIX_FOR_MB);

            return RET_ERR;
        }
    } else {
        ERR_PRNT("file name or key is NULL\n");

        return RET_ERR;
    }

    return RET_SUCCESS;
}

int xor_bare_data(char *d, size_t dlen, const char *k, size_t klen)
{
    if (klen > dlen) {
        ERR_PRNT("key length more data\n");

        return RET_ERR;
    }

    int i = 0;
    int j = 0;

    for (; i < dlen; ++i) {
        d[i] ^= k[j];

        ++j;
        if (j == klen)
            j = 0;
    }

    return RET_SUCCESS;
}

int xor_file(const char *fname, const char *key, const char *fo_end)
{
    size_t klen;
    size_t flen;
    char *fdata;
    int ret = RET_SUCCESS;

    /* check input params */
    if ((check_key_and_fname(fname, key, &klen, &flen)) != RET_SUCCESS) {
        ERR_PRNT("check_key_and_fname() failed\n");

        return RET_ERR;
    }

    /* get data */
    if ((fdata = h_read_data_from_file(fname, flen)) == NULL) {
        ERR_PRNT("h_read_data_from_file() failed\n");

        return RET_ERR;
    }

    /* fdata will change after this */
    if ((xor_bare_data(fdata, flen, key, klen)) != RET_SUCCESS) {
        ERR_PRNT("xor_bare_data() failed\n");

        ret = RET_ERR;
        goto end;
    }

    if ((h_save_data_to_file(fname, fo_end, fdata, flen)) != RET_SUCCESS) {
        ERR_PRNT("h_save_data_to_file() failed\n");

        ret = RET_ERR;
        goto end;
    }

end:
    free(fdata);
    return ret;
}

int dexor_file(const char *fname, const char *key, const char *fo_end)
{
    return xor_file(fname, key, fo_end);
}
