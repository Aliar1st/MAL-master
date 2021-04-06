#include "RTLVehicle.h"

#include <cassert>
#include <iostream>

RTLVehicle::RTLVehicle(MALArguments& arguments)
	: Vehicle("RTL Vehicles")
{
	StartRepeatedly(arguments, true);
}

RTLVehicle::~RTLVehicle()
{
	Stop();
}

void RTLVehicle::VehicleRoutine()
{
	// Retrieve data
	ResponseGetDeviceDataLocalPositionNED responseLocalPositionNED;
	ResponseGetDeviceDataAttitude responseAttitute;

	if (GetDeviceData({&responseLocalPositionNED, &responseAttitute}) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		// Vehicles is stopped
		return;
	}

	if (!responseLocalPositionNED.success ||
		!responseAttitute.success)
	{
		// Could not retrieve data
		return;
	}

	// Convert times to amount of microseconds since UNIX epoch
	uint64_t positionTime = convertTimeSinceBootToEpochTime(responseLocalPositionNED.deviceData.timeSinceBoot);
	uint64_t attitudeTime = convertTimeSinceBootToEpochTime(responseAttitute.deviceData.timeSinceBoot);

	// Prepare shared memory data
	SharedMemoryData sharedMemoryData;

	sharedMemoryData.x = responseLocalPositionNED.deviceData.x;
	sharedMemoryData.y = responseLocalPositionNED.deviceData.y;
	sharedMemoryData.z = responseLocalPositionNED.deviceData.z;
	sharedMemoryData.xSpeed = responseLocalPositionNED.deviceData.xSpeed;
	sharedMemoryData.ySpeed = responseLocalPositionNED.deviceData.ySpeed;
	sharedMemoryData.zSpeed = responseLocalPositionNED.deviceData.zSpeed;
	sharedMemoryData.positionTime = positionTime;

	sharedMemoryData.roll = responseAttitute.deviceData.roll;
	sharedMemoryData.pitch = responseAttitute.deviceData.pitch;
	sharedMemoryData.yaw = responseAttitute.deviceData.yaw;
	sharedMemoryData.rollSpeed = responseAttitute.deviceData.rollSpeed;
	sharedMemoryData.pitchSpeed = responseAttitute.deviceData.pitchSpeed;
	sharedMemoryData.yawSpeed = responseAttitute.deviceData.yawSpeed;
	sharedMemoryData.orientationTime = attitudeTime;

	sharedMemoryData.originLatitude = responseGetDeviceDataGPSGlobalOrigin.deviceData.latitude;
	sharedMemoryData.originLongitude = responseGetDeviceDataGPSGlobalOrigin.deviceData.longitude;
	sharedMemoryData.originAltitude = responseGetDeviceDataGPSGlobalOrigin.deviceData.altitude;

	// Write data to shared memory data block
	if (sharedMemoryDataSet)
	{
		semaphore.Down();
	}
	memcpy(sharedMemoryDataPointer, &sharedMemoryData, sizeof(SharedMemoryData));
	semaphore.Up();

	sharedMemoryDataSet = true;
}

bool RTLVehicle::onStarted()
{
	// Create semaphore
	Semaphore_Return semaphoreCreationResult = semaphore.Create(SEMAPHORE_NAME, 0);
	if (Semaphore::IsError(semaphoreCreationResult))
	{
		std::cout << "Could not create semaphore of name " << SEMAPHORE_NAME << std::endl;

		return false;
	}
	else if (semaphoreCreationResult == SEMAPHORE_RETURN_SUCCESS_ALREADY_EXISTED)
	{
		// Set value to 0
		while (semaphore.DownExtended(0) != SEMAPHORE_RETURN_ERROR_OPERATION_TIMEOUT)
		{
		}
	}

	// Create shared memory
	if (SharedMemory::IsError(sharedMemory.Create(SHARED_MEMORY_NAME, sizeof(SharedMemoryData))))
	{
		std::cout << "Could not create shared memory block of name " << SHARED_MEMORY_NAME << std::endl;

		return false;
	}

	std::cout << "Shared memory block of name " << SHARED_MEMORY_NAME << " created" << std::endl;

	// Setup data
	sharedMemoryDataPointer = static_cast<SharedMemoryData*>(sharedMemory.GetDataPointer());
	sharedMemoryDataSet = false;

	return requestInitialDeviceData();
}

void RTLVehicle::onStopped()
{
	// Destroy shared memory
	sharedMemory.Destroy();

	// Destroy semaphore
	semaphore.Destroy();
}

bool RTLVehicle::requestInitialDeviceData()
{
	// Request time of boot
	if (GetTimeOfBoot(&responseGetTimeOfBoot) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		// Vehicles is stopped
		std::cout << "Could not request time of boot because the vehicle is stopped" << std::endl;

		return false;
	}

	// Request device data until it could be retrieved
	bool responseGPSGlobalOriginAlreadyReceived = false;

	do
	{
		if (GetDeviceData({&responseGetDeviceDataGPSGlobalOrigin}) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
		{
			// Vehicles is stopped
			std::cout << "Could not request device data because the vehicle is stopped" << std::endl;

			return false;
		}

		printDeviceDataResponse("GPS global origin", responseGetDeviceDataGPSGlobalOrigin.success, responseGPSGlobalOriginAlreadyReceived);
	}
	while (!responseGetDeviceDataGPSGlobalOrigin.success);

	return true;
}

void RTLVehicle::printDeviceDataResponse(const char* dataName, bool success, bool& alreadyReceived)
{
	if (!success)
	{
		std::cout << "Could not request " << dataName << ". Trying again.." << std::endl;
	}
	else if (!alreadyReceived)
	{
		std::cout << "Retrieved " << dataName << "." << std::endl;
		alreadyReceived = true;
	}
}

uint64_t RTLVehicle::convertTimeSinceBootToEpochTime(uint32_t timeSinceBoot)
{
	return responseGetTimeOfBoot.timeOfBoot + static_cast<uint64_t>(timeSinceBoot) * static_cast<uint64_t>(1000);
}