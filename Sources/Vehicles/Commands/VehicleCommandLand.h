#pragma once

#include "VehicleCommand.h"

/**
 * @brief	Command to change the mode to LAND
 */
class VehicleCommandLand : public VehicleCommand
{
protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;
};