#include "xor.h"
#include "main.h"
#include "xor_break.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t try_to_guess_keylen()
{
    return 1;
}

int break_xored_file(const char *fi_xored, size_t klen, const char *fo_name)
{
    if ((!fi_xored) || (!fo_name))
        return RET_ERR;

    if (!klen)
        klen = try_to_guess_keylen();

    return RET_ERR;
}
