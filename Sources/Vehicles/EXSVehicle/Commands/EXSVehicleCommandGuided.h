#pragma once

#include "Vehicles/Commands/VehicleCommandGuided.h"

/**
 * @brief	Command to change the mode to GUIDED
 */
class EXSVehicleCommandGuided : public VehicleCommandGuided
{
public:

	virtual const char* GetName() override;
};