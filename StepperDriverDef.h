#ifndef STEPPERDRIVERDEF_H
#define STEPPERDRIVERDEF_H

#include <SparkFunAutoDriver.h>

//for setting stepper direction
#define LEFT  REV
#define RIGHT FWD
#define NO_DIR 0x02

#define SLIDER_STEP_LENGTH 464000
#define SLIDER_MAX_SPEED 1000

extern AutoDriver stepperDriver;

#endif

