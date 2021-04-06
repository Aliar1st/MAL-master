#pragma once

#include "IPC/Semaphore.h"
#include "IPC/SharedMemory.h"
#include "Vehicles/Vehicle.h"

/**
 * @brief	The vehicle that communicates with the RTL
 */
class RTLVehicle : public Vehicle
{
private:

	constexpr static const char* SHARED_MEMORY_NAME = "MAL_RTL_SHARED_MEMORY";
	constexpr static const char* SEMAPHORE_NAME = "MAL_RTL_SEMAPHORE";

	/**
	 * @brief	Data contained in the shared memory block
	 */
	struct SharedMemoryData
	{
		// Position in Local NED frame
		float x; // X Position [m]
		float y; // Y Position [m]
		float z; // Z Position [m]
		float xSpeed; // Speed in X direction [m/s]
		float ySpeed; // Speed in Y direction [m/s]
		float zSpeed; // Speed in Z direction [m/s]
		uint64_t positionTime; // Time since UNIX epoch of position data [us]

		// Orientation in respect to UAV NED
		float roll; // Roll angle [rad]
		float pitch; // Pitch angle [rad]
		float yaw; // Yaw angle [rad]
		float rollSpeed; // Roll angular speed [rad/s]
		float pitchSpeed; // Pitch angular speed [rad/s]
		float yawSpeed; // Yaw angular speed [rad/s]
		uint64_t orientationTime; // Time since UNIX epoch of orientation data [us]

		// Position of the origin of the Local NED frame
		int32_t originLatitude; // Origin latitude [degE7]
		int32_t originLongitude; // Origin longitude [degE7]
		int32_t originAltitude; // Origin altitude above Mean Sea Level [mm]
	};

	Semaphore semaphore;
	SharedMemory sharedMemory;
	SharedMemoryData* sharedMemoryDataPointer;
	bool sharedMemoryDataSet;

	ResponseGetTimeOfBoot responseGetTimeOfBoot;
	ResponseGetDeviceDataGPSGlobalOrigin responseGetDeviceDataGPSGlobalOrigin;

	/**
	 * @brief	Requests the initial device data
	 *
	 * @return	True if successful; False otherwise
	 */
	bool requestInitialDeviceData();

	/**
	 * @brief	Prints information on whether a device data response was successfully received
	 *
	 * @param[in]		dataName			Name of the received data
	 * @param			success				Whether the response was successfully received
	 * @param[in,out]	alreadyReceived		True if the response has already been received in the past successfully; False otherwise.
	 *										After this function returns, contains true if the response was successfully received
	 */
	void printDeviceDataResponse(const char* dataName, bool success, bool& alreadyReceived);

	/**
	 * @brief	Converts a given time since the boot of the MAVLink device to the UNIX epoch time
	 *
	 * @param	timeSinceBoot	Time since boot in milliseconds
	 *
	 * @return	Epoch time in microseconds
	 */
	uint64_t convertTimeSinceBootToEpochTime(uint32_t timeSinceBoot);

protected:

	virtual bool onStarted() override;

	virtual void onStopped() override;

public:

	/**
	 * @brief	The constructor of RTLVehicle.
	 *
	 * @param[in]	arguments	Arguments of the MAL
	 */
	RTLVehicle(MALArguments& arguments);

	/**
	 * @brief	The constructor of RTLVehicle.
	 */
	~RTLVehicle();

	virtual void VehicleRoutine() override;
};