#ifndef ARM_OPERATING_SYSTEM_COMMANDS_C
#define ARM_OPERATING_SYSTEM_COMMANDS_C

#include "help.h"
#include "train.h"
#include "rotate.h"
#include "scale.h"
#include "color.h"
#include "calc.h"
#include "echo.h"
#include "fibonacci.h"

typedef void (*Function)(char*);
struct program {
    char* name;
    Function function;
};
typedef struct program Program;

static Program programs[] = {
        {"help", help},
        {"train", train},
        {"rotate", rotate},
        {"scale", scale},
        {"color", color},
        {"calc", calc},
        {"echo", echo},
        {"fibonacci", fibonacci},
        };

#endif //ARM_OPERATING_SYSTEM_COMMANDS_C