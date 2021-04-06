#pragma once

#include "MavlinkAbstractionArguments.h"
#include "VehicleArguments.h"

/**
 * @brief	Structure holding all program arguments
 */
struct MALArguments
{
	MavlinkAbstractionArguments mavlinkAbstractionArguments;
	VehicleArguments vehicleArguments;
};