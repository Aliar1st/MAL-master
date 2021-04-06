#include "TestVehicleCommandCollection.h"

Vehicle_Command_Execution_Result TestVehicleCommandCollection::execute(Vehicle* vehicle)
{
	Vehicle_Command_Execution_Result executionResult = Vehicle_Command_Execution_Result::SUCCESS;

	// Get commands
	const std::vector<VehicleCommand*> commands = getCommands(vehicle);

	// Execute all commands
	for (size_t i = 0; i < commands.size(); i++)
	{
		executionResult = commands[i]->Execute(vehicle, true, true, INDENT);

		// Abort if command not successful
		if (executionResult != Vehicle_Command_Execution_Result::SUCCESS)
		{
			break;
		}

		// Emit a sound after each command
		vehicle->Beep();
	}

	// Delete commands
	for (size_t i = 0; i < commands.size(); i++)
	{
		delete commands[i];
	}

	// No error
	return executionResult;
}