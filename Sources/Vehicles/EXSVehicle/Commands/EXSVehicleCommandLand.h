#pragma once

#include "Vehicles/Commands/VehicleCommandLand.h"

/**
 * @brief	Command to change the mode to LAND
 */
class EXSVehicleCommandLand : public VehicleCommandLand
{
public:

	virtual const char* GetName() override;
};