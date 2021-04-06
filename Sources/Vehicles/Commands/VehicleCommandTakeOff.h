#pragma once

#include "VehicleCommand.h"

/**
 * @brief	Command to perform a take-off
 */
class VehicleCommandTakeOff : public VehicleCommand
{
private:

	constexpr static const float ALTITUDE_THRESHOLD = 0.25f;

	float altitude;

protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	/**
	 * @brief	The constructor of VehicleCommandTakeOff
	 *
	 * @param	altitude	Altitude to take off to [m]
	 */
	VehicleCommandTakeOff(float altitude);

	virtual const char* GetName() override;
};