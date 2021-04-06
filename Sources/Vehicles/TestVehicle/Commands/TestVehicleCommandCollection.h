#pragma once

#include "TestVehicleCommand.h"

/**
 * @brief	A command collection
 */
class TestVehicleCommandCollection : public TestVehicleCommand
{
private:

	constexpr static const char* INDENT = "  ";

protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

	/**
	 * @brief	Returns all commands of the collection
	 *
	 * @param[in]	vehicle		Vehicle this command performs on
	 *
	 * @brief	Vector containing all commands. These commands are allocated dynamically using new
	 */
	virtual const std::vector<VehicleCommand*> getCommands(Vehicle* vehicle) = 0;
};