#pragma once

#include "IPC/Semaphore.h"
#include "IPC/SharedMemory.h"
#include "Vehicles/CommandVehicle.h"
#include "Vehicles/Commands/VehicleCommand.h"

/**
 * @brief	The vehicle that communicates with the CTH
 */
class CTHVehicle : public CommandVehicle<VehicleCommand>
{
private:

	constexpr static const char* SHARED_MEMORY_NAME = "CTH_MAL_SHARED_MEMORY";
	constexpr static const char* SEMAPHORE_NAME = "CTH_MAL_SEMAPHORE";

	/**
	 * @brief	Data contained in the shared memory block
	 */
	struct SharedMemoryData
	{
		char command[20];
	};

	Semaphore semaphore;
	SharedMemory sharedMemory;
	SharedMemoryData* sharedMemoryDataPointer;

	/**
	 * @brief	Reads the command from the shared memory.
	 */
	void readCommand();

protected:

	virtual bool onStarted() override;

	virtual void onStopped() override;

public:

	/**
	 * @brief	The constructor of CTHVehicle.
	 *
	 * @param[in]	arguments	Arguments of the MAL
	 */
	CTHVehicle(MALArguments& arguments);

	/**
	 * @brief	The constructor of CTHVehicle.
	 */
	~CTHVehicle();

	virtual void VehicleRoutine() override;
};