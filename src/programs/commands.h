#ifndef ARM_OPERATING_SYSTEM_COMMANDS_C
#define ARM_OPERATING_SYSTEM_COMMANDS_C

#include "help.h"
#include "train.h"
#include "rotate.h"
#include "scale.h"
#include "color.h"
#include "calc.h"
#include "echo.h"
#include "reboot.h"
#include "pong.h"
#include "ztype.h"
#include "clear.h"

typedef void (*Function)(char*);
struct program {
    char* name;
    Function function;
};
typedef struct program Program;
#define programsLength 11

static Program programs[] = {
        {"help", help},
        {"train", train},
        {"rotate", rotate},
        {"scale", scale},
        {"color", color},
        {"calc", calc},
        {"echo", echo},
        {"reboot",reboot},
        {"pong", pong},
        {"ztype", ztype},
        {"clear", clear}
        };

#endif //ARM_OPERATING_SYSTEM_COMMANDS_C