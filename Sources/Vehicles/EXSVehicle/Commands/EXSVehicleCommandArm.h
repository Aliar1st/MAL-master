#pragma once

#include "Vehicles/Commands/VehicleCommandArm.h"

/**
 * @brief	Command to arm the MAVLink device
 */
class EXSVehicleCommandArm : public VehicleCommandArm
{
public:

	virtual const char* GetName() override;
};