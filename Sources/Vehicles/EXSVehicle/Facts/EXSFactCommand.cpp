#include "EXSFactCommand.h"

#include "Vehicles/Commands/VehicleCommand.h"

EXSFactCommand::EXSFactCommand(VehicleCommand* command)
	: EXSFact("Command", "Mission", NULL)
{
	if (command)
	{
		SetValue(command->GetName());
	}
	else
	{
		SetValue("None");
	}
}