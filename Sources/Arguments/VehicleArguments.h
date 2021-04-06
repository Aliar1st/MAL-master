#pragma once

#include "Arguments.h"
#include "Vehicles/Vehicles.h"

/**
 * @brief	Structure holding program arguments of the vehicles
 */
struct VehicleArguments : public Arguments
{
	Vehicles vehicle = Vehicles::TEST;
	bool outdoor = false;
};