#include "TestVehicleCommandStop.h"

const char* TestVehicleCommandStop::GetName()
{
	return "stop";
}

const std::vector<const char*> TestVehicleCommandStop::GetAlternativeNames()
{
	return {"quit"};
}

const char* TestVehicleCommandStop::GetDescription()
{
	return "Stops the Test Vehicles";
}

Vehicle_Command_Execution_Result TestVehicleCommandStop::execute(Vehicle* vehicle)
{
	vehicle->Stop();

	return Vehicle_Command_Execution_Result::SUCCESS;
}
