#pragma once

#include "TestVehicleCommand.h"

/**
 * @brief	A command to move the MAVLink device by an offset
 */
class TestVehicleCommandMoveBy : public TestVehicleCommand
{
protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;

	virtual const char* GetDescription() override;
};