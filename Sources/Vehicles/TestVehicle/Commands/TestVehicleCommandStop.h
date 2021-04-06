#pragma once

#include "TestVehicleCommand.h"

/**
 * @brief	A command to stop the test vehicle
 */
class TestVehicleCommandStop : public TestVehicleCommand
{
protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;

	virtual const std::vector<const char*> GetAlternativeNames() override;

	virtual const char* GetDescription() override;
};