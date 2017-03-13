#include <utils/Resolution.h>

#ifndef INC_CONFIG
#define INC_CONFIG

#define NAMESPACE VolkswagenDriversPerception
#define BEGIN_NAMESPACE namespace VolkswagenDriversPerception {
#define END_NAMESPACE   }

#define PROJECT_NAME   "VolkswagenDriversPerception"

#define VERSION_MAJOR  "0"
#define VERSION_MINOR  "1"
#define VERSION_PATCH  "1"
#define VERSION        "0.1.1"
#define VERSION_STRING "VolkswagenDriversPerception ver. 0.1.1 (David Geisler david.geisler@uni-tuebingen.de)"

/* #undef WITH_CUDA */
#define WITH_OPENCV   ON
#define WITH_BOOST    ON
/* #undef WITH_SIMULINK */
/* #undef DEBUG */

/* #undef DEBUG */

#define BUILD_LIB_CORE ON
#define BUILD_LIB_GAZE ON
#define BUILD_LIB_IO ON
#define BUILD_LIB_SALIENCY ON
#define BUILD_LIB_UTILS ON
#define BUILD_LIB_VISIO ON

//ignore deprecated warnings (opencv generates a lot of it)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#define STACK_SIZE 1024*1024*512
#define PYRAMIDES 3
#define THREADS 4
#define RESOLUTION RES_HQVGA

#endif //INC_CONFIG
