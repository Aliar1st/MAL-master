#include "TestVehicleCommandBeep.h"

const char* TestVehicleCommandBeep::GetName()
{
	return "beep";
}

const char* TestVehicleCommandBeep::GetDescription()
{
	return "Emits a sound";
}

Vehicle_Command_Execution_Result TestVehicleCommandBeep::execute(Vehicle* vehicle)
{
	vehicle->Beep();

	return Vehicle_Command_Execution_Result::SUCCESS;
}
