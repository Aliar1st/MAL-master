#pragma once

#include "Vehicles/Commands/VehicleCommandDisarm.h"

/**
 * @brief	Command to disarm the MAVLink device
 */
class EXSVehicleCommandDisarm : public VehicleCommandDisarm
{
public:

	virtual const char* GetName() override;
};