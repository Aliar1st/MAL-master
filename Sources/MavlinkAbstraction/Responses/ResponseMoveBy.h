#pragma once

#include <stdint.h>
#include "ResponseSuccess.h"

/**
 * @brief	A response of the "MoveBy" request
 */
struct ResponseMoveBy : public ResponseSuccess
{
	int32_t targetLatitude; // Target latitude [degE7]
	int32_t targetLongitude; // Target longitude [degE7]
	int32_t targetAltitude; // Target altitude [mm]
	uint16_t targetHeading; // Target heading [cdeg]
};