#pragma once

/**
 * Include this header instead of the MAVLink library directly to fix warnings
 */

#ifdef _WIN32

#pragma warning(push, 0)
#pragma warning(disable: 26451)
#include "MAVLink/ardupilotmega/mavlink.h"
#pragma warning(pop)

#elif __linux__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#include "MAVLink/ardupilotmega/mavlink.h"
#pragma GCC diagnostic pop

#endif // __linux__