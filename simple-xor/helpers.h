#ifndef _HELPERS_H
#define _HELPERS_H

#include <stddef.h>

size_t h_fsize(const char *fname);
char *h_read_data_from_file(const char *fname, size_t flen);
int h_save_data_to_file(const char *orig_fname,
            const char *end, char *d, size_t dlen);

#endif  // _HELPERS_H
