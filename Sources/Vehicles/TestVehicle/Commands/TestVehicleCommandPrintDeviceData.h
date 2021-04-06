#pragma once

#include "TestVehicleCommand.h"

/**
 * @brief	A command to print all data of the MAVLink device
 */
class TestVehicleCommandPrintDeviceData : public TestVehicleCommand
{
private:

	constexpr static const char* INDENT = "  ";

protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;

	virtual const std::vector<const char*> GetAlternativeNames() override;

	virtual const char* GetDescription() override;
};