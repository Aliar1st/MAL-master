#pragma once

#include "TestVehicleCommand.h"

/**
 * @brief	A command to change to the STABILIZE mode
 */
class TestVehicleCommandStabilize : public TestVehicleCommand
{
protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;

	virtual const std::vector<const char*> GetAlternativeNames() override;

	virtual const char* GetDescription() override;
};