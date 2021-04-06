#include "VehicleCommand.h"

#include <iostream>

VehicleCommand::VehicleCommand()
{
}

VehicleCommand::~VehicleCommand()
{
}

Vehicle_Command_Execution_Result VehicleCommand::Execute(Vehicle* vehicle, bool printName, bool printExecutionResult, const char* indent)
{
	if (printName)
	{
		std::cout << indent << "Executing command: " << GetName() << std::endl;
	}

	// Execute the command
	Vehicle_Command_Execution_Result result = execute(vehicle);

	if (printExecutionResult)
	{
		// Evaluate the result
		switch (result)
		{
			case Vehicle_Command_Execution_Result::SUCCESS:
			{
				std::cout << indent << "SUCCESS" << std::endl;

				break;
			}

			case Vehicle_Command_Execution_Result::DENIED:
			{
				std::cout << indent << "ERROR: Denied" << std::endl;

				break;
			}

			case Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED:
			{
				std::cout << indent << "ERROR: Vehicles not started" << std::endl;

				break;
			}
		}
	}

	return result;
}

Vehicle_Command_Execution_Result VehicleCommand::waitForDeviceDataCondition(Vehicle* vehicle, const std::vector<ResponseGetDeviceDataBase*>& responses, std::function<bool()> condition)
{
	// Get device data as long as the condition is unsatisfied
	do
	{
		if (vehicle->GetDeviceData(responses) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
		{
			return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
		}

		// Check if any response was denied
		for (ResponseGetDeviceDataBase* response : responses)
		{
			if (!response->success)
			{
				return Vehicle_Command_Execution_Result::DENIED;
			}
		}
	}
	while (!condition());

	return Vehicle_Command_Execution_Result::SUCCESS;
}