#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xor.h"
#include "main.h"
#include "xor_break.h"
#include "simple_xor_test.h"

struct what_xor_oper {
    int e_flag;
    int d_flag;
    int b_flag;
    int t_flag;
    int l_flag;
    char *fi;
    char *key;
};

static void usage(void)
{
    printf("Usage: ./xor_enc.a <flags> -i [FILE] -k [KEYWORD]\n");
    printf("flags:\n");
    printf("-i --> input file pointer\n");
    printf("-k --> keyword pointer\n");
    printf("-t --> test mode (if it is on, other options are ignored)\n");
    printf("-e --> encrypt [FILE] with [KEYWORD]\n");
    printf("-d --> decrypt [FILE] with [KEYWORD]\n");
    printf("-b --> break xored [FILE] ([KEYWORD] isn't used in this case)\n");
    printf("-l --> lenth of key to break XORed file. DO NOT USE this flag if want to guess key len\n");
}

static int parse_args(int argc, char **argv, struct what_xor_oper *op)
{
    int c;

    while ((c = getopt(argc, argv, "edbti:k:l:")) != RET_ERR) {
        switch (c) {
        case 'i':
            op->fi = optarg;
            break;
        case 'k':
            op->key = optarg;
            break;
        case 'e':
            op->e_flag = DO_OP;
            break;
        case 'd':
            op->d_flag = DO_OP;
            break;
        case 'b':
            op->b_flag = DO_OP;
            break;
        case 't':
            op->t_flag = DO_OP;
            return RET_SUCCESS;
        case 'l':
            op->l_flag = atoi((const char *)optarg);
            break;
        case '?':
            if ((optopt == 'i') || (optopt == 'k')) {
                ERR_PRNT("need value with -%c option\n\n", optopt);

                return RET_ERR;
            } else if (isprint(optopt)) {
                ERR_PRNT("unknown option `-%c'\n\n", optopt);

                return RET_ERR;
            } else {
                ERR_PRNT("unknown option character `\\x%x'\n\n", optopt);

                return RET_ERR;
            }
            break;
        default:
            break;
        }
    }

    DBG_PRNT("File - <%s>\n", op->fi);
    DBG_PRNT("Key - <%s>\n", op->key);
    DBG_PRNT("e - %c\n", op->e_flag ? '1' : '0');
    DBG_PRNT("d - %c\n", op->d_flag ? '1' : '0');
    DBG_PRNT("b - %c\n", op->b_flag ? '1' : '0');
    DBG_PRNT("l - %d\n", op->l_flag);
    DBG_PRNT("t - %c\n", op->t_flag ? '1' : '0');

    if (op->t_flag == DO_OP)
        return RET_SUCCESS;

    if ((op->e_flag | op->d_flag | op->b_flag) == NOT_DO_OP) {
        ERR_PRNT("you should set at least the one flag\n");

        return RET_ERR;
    }

    if ((op->e_flag & op->d_flag) || (op->e_flag & op->b_flag) || \
        (op->d_flag & op->b_flag)) {
        ERR_PRNT("mutual exclusion operations\n");

        return RET_ERR;
    }
    if (access((const char *)op->fi, F_OK) == -1 ) {
        ERR_PRNT("file <%s> not exists\n", op->fi);

        return RET_ERR;
    }

    if (access((const char *)op->fi, R_OK) == -1 ) {
        ERR_PRNT("you can't access to read the <%s> file", op->fi);

        return RET_ERR;
    }

    return RET_SUCCESS;
}

int main(int argc, char **argv)
{
    struct what_xor_oper xor_oper = { 0 };

    if (parse_args(argc, argv, &xor_oper) != RET_SUCCESS) {
        usage();

        return EXIT_FAILURE;
    }

    if (xor_oper.t_flag) {
        simple_xor_run_test();
    } else if (xor_oper.e_flag) {
        if (xor_file((const char *)xor_oper.fi, (const char *)xor_oper.key,
                    ".xored") != RET_SUCCESS) {
            ERR_PRNT("xor_file() failed\n");

            return EXIT_FAILURE;
        }
    } else if (xor_oper.d_flag) {
        if (dexor_file((const char *)xor_oper.fi, (const char *)xor_oper.key,
                ".dexored") != RET_SUCCESS) {
            ERR_PRNT("dexor_file() failed\n");

            return EXIT_FAILURE;
        }
    } else if (xor_oper.b_flag) {
        if (break_xored_file(xor_oper.fi, xor_oper.l_flag,
                    ".breaked") == RET_SUCCESS) {
            DBG_PRNT("break the xored file is success\n");
        } else {
            ERR_PRNT("break_xored_file() failed\n");

            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
