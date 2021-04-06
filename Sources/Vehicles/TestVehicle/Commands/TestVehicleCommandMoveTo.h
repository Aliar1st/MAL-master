#pragma once

#include "TestVehicleCommand.h"

/**
 * @brief	A command to move the MAVLink device to an absolute position
 */
class TestVehicleCommandMoveTo : public TestVehicleCommand
{
protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;

	virtual const char* GetDescription() override;
};