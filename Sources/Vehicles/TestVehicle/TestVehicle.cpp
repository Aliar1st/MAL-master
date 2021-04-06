#include "TestVehicle.h"

#include <iostream>
#include "Commands/TestVehicleCommandAltHold.h"
#include "Commands/TestVehicleCommandArm.h"
#include "Commands/TestVehicleCommandBeep.h"
#include "Commands/TestVehicleCommandDisarm.h"
#include "Commands/TestVehicleCommandGuided.h"
#include "Commands/TestVehicleCommandKill.h"
#include "Commands/TestVehicleCommandLand.h"
#include "Commands/TestVehicleCommandMission1.h"
#include "Commands/TestVehicleCommandMission2.h"
#include "Commands/TestVehicleCommandMission3.h"
#include "Commands/TestVehicleCommandMission4.h"
#include "Commands/TestVehicleCommandMission5.h"
#include "Commands/TestVehicleCommandMission6.h"
#include "Commands/TestVehicleCommandMission7.h"
#include "Commands/TestVehicleCommandMission8.h"
#include "Commands/TestVehicleCommandMoveBy.h"
#include "Commands/TestVehicleCommandMoveTo.h"
#include "Commands/TestVehicleCommandPosHold.h"
#include "Commands/TestVehicleCommandPrintDeviceData.h"
#include "Commands/TestVehicleCommandRTL.h"
#include "Commands/TestVehicleCommandRebootFlightController.h"
#include "Commands/TestVehicleCommandStabilize.h"
#include "Commands/TestVehicleCommandStop.h"
#include "Commands/TestVehicleCommandTakeOff.h"
#include "Utilities/InputUtil.h"
#include "Utilities/StringUtil.h"

TestVehicle::TestVehicle(MALArguments& arguments)
	: CommandVehicle("Test Vehicles",
{
	// Keep this list ordered alphabetically
	new TestVehicleCommandAltHold(),
	new TestVehicleCommandArm(),
	new TestVehicleCommandBeep(),
	new TestVehicleCommandDisarm(),
	new TestVehicleCommandGuided(),
	new TestVehicleCommandKill(),
	new TestVehicleCommandLand(),
	new TestVehicleCommandMission1(),
	new TestVehicleCommandMission2(),
	new TestVehicleCommandMission3(),
	new TestVehicleCommandMission4(),
	new TestVehicleCommandMission5(),
	new TestVehicleCommandMission6(),
	new TestVehicleCommandMission7(),
	new TestVehicleCommandMission8(),
	new TestVehicleCommandMoveBy(),
	new TestVehicleCommandMoveTo(),
	new TestVehicleCommandPosHold(),
	new TestVehicleCommandPrintDeviceData(),
	new TestVehicleCommandRTL(),
	new TestVehicleCommandRebootFlightController(),
	new TestVehicleCommandStabilize(),
	new TestVehicleCommandStop(),
	new TestVehicleCommandTakeOff()
})
{
	StartRepeatedly(arguments, true);
}

TestVehicle::~TestVehicle()
{
	Stop();
}

void TestVehicle::VehicleRoutine()
{
	std::cout << "Command: ";

	char* inputString = InputUtil::ReadLineFromConsole();

	// Select the "closest" command
	TestVehicleCommand* command;
	const char* closestName;
	size_t distance;

	command = selectCommand(inputString, &closestName, &distance);

	// Check if command is to be executed
	if (handleCommand(command, closestName, distance))
	{
		// Execute the command
		command->Execute(this, false, true);
	}
	else
	{
		// Print command list
		printCommandList();
	}

	std::cout << std::endl;

	delete[] inputString;
}

TestVehicleCommand* TestVehicle::selectCommand(const char* name, const char** closestName, size_t* distance)
{
	// Do not select a command if no command string was provided or if the help list shall be printed
	if (!StringUtil::Strcmp(name, "") || !StringUtil::Strcmp(name, "help", false))
	{
		return nullptr;
	}

	TestVehicleCommand* selectedCommand = nullptr;
	size_t tempDistance;

	// Look for the command with the smallest levenshtein distance
	for (size_t i = 0; i < commands.size(); i++)
	{
		const std::vector<const char*> commandNames = commands[i]->GetNames();

		// Search in all names
		for (size_t j = 0; j < commandNames.size(); j++)
		{
			const char* commandName = commandNames[j];

			size_t curDistance = StringUtil::Levenshtein(name, commandName, false);

			if (selectedCommand == nullptr || tempDistance > curDistance)
			{
				selectedCommand = commands[i];
				tempDistance = curDistance;
				*closestName = commandName;

				if (!tempDistance)
				{
					break;
				}
			}
		}
	}

	if (selectedCommand)
	{
		*distance = tempDistance;
	}

	return selectedCommand;
}

bool TestVehicle::handleCommand(TestVehicleCommand* command, const char* closestName, size_t commandDistance)
{
	if (!command)
	{
		return false;
	}

	if (!commandDistance)
	{
		return true;
	}

	// Ask the user if they meant the command with the smallest distance
	bool result = false;

	if (commandDistance <= MAX_LEVENSHTEIN_COMMAND_DISTANCE)
	{
		std::cout << "Did you mean \"" << closestName << "\"? (yes|y): ";

		char* inputString = InputUtil::ReadLineFromConsole();

		if (!StringUtil::Strcmp(inputString, "yes", false) || !StringUtil::Strcmp(inputString, "y", false))
		{
			result = true;
		}

		delete[] inputString;
	}

	return result;
}

void TestVehicle::printCommandList()
{
	std::cout << "Available commands:" << std::endl;

	for (size_t i = 0; i < commands.size(); i++)
	{
		TestVehicleCommand* command = commands[i];

		// Get command names and description
		const std::vector<const char*> names = command->GetNames();
		const char* description = command->GetDescription();

		// Print command names
		std::cout << INDENT;

		for (size_t i = 0; i < names.size(); i++)
		{
			std::cout << names[i];

			if (i < names.size() - 1)
			{
				std::cout << ", ";
			}
		}

		// Print command description
		std::cout << " - " << description << std::endl;
	}
}