#pragma once

#include "VehicleCommand.h"

/**
 * @brief	Command to arm the MAVLink device
 */
class VehicleCommandArm : public VehicleCommand
{
protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;
};