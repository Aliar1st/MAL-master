#pragma once

#include "Vehicles/Commands/VehicleCommandTakeOff.h"

/**
 * @brief	Command to perform a take-off
 */
class EXSVehicleCommandTakeOff : public VehicleCommandTakeOff
{
private:

	constexpr static const float TAKE_OFF_ALTITUDE = 5.0f;

public:

	EXSVehicleCommandTakeOff();

	virtual const char* GetName() override;
};