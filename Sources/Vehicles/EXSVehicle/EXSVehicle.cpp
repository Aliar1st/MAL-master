#include "EXSVehicle.h"

#include <iostream>
#include <stdlib.h>
#include "Commands/EXSVehicleCommandArm.h"
#include "Commands/EXSVehicleCommandDisarm.h"
#include "Commands/EXSVehicleCommandGuided.h"
#include "Commands/EXSVehicleCommandLand.h"
#include "Commands/EXSVehicleCommandTakeOff.h"
#include "Facts/EXSFactCollection.h"
#include "Facts/EXSFactCommand.h"
#include "Facts/EXSFactCopterAltitude.h"
#include "Facts/EXSFactCopterState.h"
#include "Facts/EXSFactFlightMode.h"
#include "Facts/EXSFactLandedState.h"
#include "Utilities/StringUtil.h"

EXSVehicle::EXSVehicle(MALArguments& arguments)
	: CommandVehicle("EXS Vehicles",
{
	// Keep this list ordered alphabetically
	new EXSVehicleCommandArm(),
	new EXSVehicleCommandDisarm(),
	new EXSVehicleCommandGuided(),
	new EXSVehicleCommandLand(),
	new EXSVehicleCommandTakeOff()
})
{
	setupEXSPath();

	StartRepeatedly(arguments, true);
}

EXSVehicle::~EXSVehicle()
{
	Stop();

	delete[] exsExecuteCommand;
	delete[] exsPath;
}

void EXSVehicle::VehicleRoutine()
{
	// Get device data
	ResponseGetDeviceDataHeartbeat responseHeartbeat;
	ResponseGetDeviceDataExtendedSystemState responseExtendedSystemState;

	if (GetDeviceData({&responseHeartbeat, &responseExtendedSystemState}) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		std::cout << "ERROR: Vehicles not started" << std::endl;

		return;
	}

	if (!responseHeartbeat.success ||
		!responseExtendedSystemState.success)
	{
		std::cout << "ERROR: Retrieving of device data denied" << std::endl;

		return;
	}

	// Write facts
	writeFacts(responseHeartbeat.deviceData, responseExtendedSystemState.deviceData);

	// Execute EXS
	executeEXS();

	// Read facts
	readFacts();

	// Evaluate the facts (i.e. execute the given command)
	evaluateFacts();
}

bool EXSVehicle::onStarted()
{
	// Initial facts
	factCollection = new EXSFactCollection(exsPath);
	factCollection->SetFact(EXSFact("MissionState", "Mission", "Triggered"));

	return true;
}

void EXSVehicle::onStopped()
{
	delete factCollection;
}

void EXSVehicle::setupEXSPath()
{
#ifdef _WIN32
	// On Windows the EXS is located at "%USERPROFILE%\Documents\APOLI_CLIP\".
	char* userProfilePath;

	if (!_dupenv_s(&userProfilePath, NULL, "USERPROFILE") && userProfilePath != NULL)
	{
		exsPath = StringUtil::Strcat(userProfilePath, "\\Documents\\APOLI_CLIP\\");
		free(userProfilePath);
	}
#elif __linux__
	// On Linux the EXS is located at "%HOME%/projects/APOLI_CLIP/".
	char* userHomePath = getenv("HOME");

	if (userHomePath)
	{
		exsPath = StringUtil::Strcat(userHomePath, "/projects/APOLI_CLIP/");
	}
#endif // __linux__

	// Default values
	if (!exsPath)
	{
		exsPath = StringUtil::Strcpy("");
	}

	// Set the executable command
	exsExecuteCommand = StringUtil::Strcpy(std::string("cd \"" + std::string(exsPath) + "\" && \"" + EXS_EXECUTABLE_NAME + "\"" + SYSTEM_PREVENT_OUTPUT).c_str());
}

void EXSVehicle::writeFacts(DeviceDataHeartbeat& deviceDataHeartbeat, DeviceDataExtendedSystemState& deviceDataExtendedSystemState)
{
	// Evaluate the vehicle values
	factCollection->SetFact(EXSFactCommand(nullptr));
	factCollection->SetFact(EXSFactCopterAltitude());
	factCollection->SetFact(EXSFactCopterState(deviceDataHeartbeat));
	factCollection->SetFact(EXSFactLandedState(deviceDataExtendedSystemState));
	factCollection->SetFact(EXSFactFlightMode(deviceDataHeartbeat));

	// Write the facts to the file
	factCollection->Write();
}

void EXSVehicle::executeEXS()
{
	system(exsExecuteCommand);
}

void EXSVehicle::readFacts()
{
	factCollection->Read();
}

void EXSVehicle::evaluateFacts()
{
	// Check for the value of the "Command" fact
	char* commandFactStringValue = factCollection->GetFactValue("Command", "Mission");

	if (commandFactStringValue)
	{
		// Search for the command
		for (size_t i = 0; i < commands.size(); i++)
		{
			VehicleCommand* command = commands[i];

			if (!StringUtil::Strcmp(command->GetName(), commandFactStringValue))
			{
				// Execute the command
				command->Execute(this, true, true);

				break;
			}
		}
	}
}