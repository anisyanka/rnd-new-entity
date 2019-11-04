#ifndef _XOR_BREAK_H_
#define _XOR_BREAK_H_

#include <stddef.h>

/**
 * @param fi_xored
 * Name of file with xored data
 *
 * @param klen
 * Length of known key, USE zero to try to guess the key len
 *
 * @param fo_end
 * Name of end of file to save the breaked data
 *
 * @return
 * RET_SUCCESS - break xor success
 * RET_ERR - break xor fault
 *
 **/
int break_xored_file(const char *fi_xored, size_t klen, const char *fo_end);

#endif  // _XOR_BREAK_H_
