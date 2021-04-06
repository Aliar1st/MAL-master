#pragma once

#include <stdint.h>
#include "Response.h"

/**
 * @brief	A response of the "GetTimeOfBoot" request
 */
struct ResponseGetTimeOfBoot : public Response
{
	// Time of boot in microseconds since the UNIX epoch time
	uint64_t timeOfBoot;
};