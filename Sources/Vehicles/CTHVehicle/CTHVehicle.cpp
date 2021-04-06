#include "CTHVehicle.h"

#include <iostream>
#include <stdlib.h>
#include "Utilities/StringUtil.h"
#include "Vehicles/Commands/VehicleCommandArm.h"
#include "Vehicles/Commands/VehicleCommandDisarm.h"
#include "Vehicles/Commands/VehicleCommandGuided.h"
#include "Vehicles/Commands/VehicleCommandLand.h"
#include "Vehicles/Commands/VehicleCommandTakeOff.h"

CTHVehicle::CTHVehicle(MALArguments& arguments)
	: CommandVehicle("CTH Vehicles",
{
	// Keep this list ordered alphabetically
	new VehicleCommandArm(),
	new VehicleCommandDisarm(),
	new VehicleCommandGuided(),
	new VehicleCommandLand(),
	new VehicleCommandTakeOff(5.0f)
})
{
	StartRepeatedly(arguments, true);
}

CTHVehicle::~CTHVehicle()
{
	Stop();
}

void CTHVehicle::VehicleRoutine()
{
	readCommand();
}

bool CTHVehicle::onStarted()
{
	// Open semaphore
	if (Semaphore::IsError(semaphore.Open(SEMAPHORE_NAME)))
	{
		std::cout << "Could not open semaphore of name " << SEMAPHORE_NAME << std::endl;

		return false;
	}

	// Open shared memory
	if (SharedMemory::IsError(sharedMemory.Open(SHARED_MEMORY_NAME, sizeof(SharedMemoryData))))
	{
		std::cout << "Could not open shared memory block of name " << SHARED_MEMORY_NAME << std::endl;

		return false;
	}

	std::cout << "Shared memory block of name " << SHARED_MEMORY_NAME << " opened" << std::endl;

	// Setup data
	sharedMemoryDataPointer = static_cast<SharedMemoryData*>(sharedMemory.GetDataPointer());

	return true;
}

void CTHVehicle::onStopped()
{
	// Close shared memory
	sharedMemory.Close();

	// Close semaphore
	semaphore.Close();
}

void CTHVehicle::readCommand()
{
	// Safely access the command
	semaphore.Down();

	const char* commandName = StringUtil::Strcpy(sharedMemoryDataPointer->command);

	semaphore.Up();

	// Search for the command
	for (size_t i = 0; i < commands.size(); i++)
	{
		VehicleCommand* command = commands[i];

		if (!StringUtil::Strcmp(command->GetName(), commandName))
		{
			// Execute the command
			command->Execute(this, true, true);

			// Reset the command
			semaphore.Down();
			sharedMemoryDataPointer->command[0] = '\0';
			semaphore.Up();

			break;
		}
	}

	delete[] commandName;
}