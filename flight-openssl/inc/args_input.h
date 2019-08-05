#ifndef _INC_ARGS_INPUT_H
#define _INC_ARGS_INPUT_H

typedef enum {
  ARGS_TRUE,
  ARGS_WRONG
} ArgsCheckerState_t;

typedef struct {
  //
} ArgsSummary_t;

ArgsCheckerState_t ArgsCheckInput(int argc, char ** argv, ArgsSummary_t * summary_arg);

#endif  // _INC_ARGS_INPUT_H
