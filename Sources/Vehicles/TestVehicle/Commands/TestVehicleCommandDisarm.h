#pragma once

#include "TestVehicleCommand.h"

/**
 * @brief	A command to disarm the MAVLink device
 */
class TestVehicleCommandDisarm : public TestVehicleCommand
{
protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;

	virtual const char* GetDescription() override;
};