#pragma once

#include "VehicleCommand.h"

/**
 * @brief	Command to disarm the MAVLink device
 */
class VehicleCommandDisarm : public VehicleCommand
{
protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;
};