#pragma once

#include "Vehicles/CommandVehicle.h"
#include "Vehicles/Commands/VehicleCommand.h"

class EXSFactCollection;

/**
 * @brief	The vehicle that communicates with the EXS
 */
class EXSVehicle : public CommandVehicle<VehicleCommand>
{
private:

#ifdef _WIN32
	constexpr static const char* EXS_EXECUTABLE_NAME = "startclips.bat";
#elif __linux__
	constexpr static const char* EXS_EXECUTABLE_NAME = "./startclips.sh";
#endif // __linux__

	constexpr static const char* SYSTEM_PREVENT_OUTPUT = " > nul";

	char* exsPath = nullptr;
	char* exsExecuteCommand = nullptr;

	EXSFactCollection* factCollection = nullptr;

	/**
	 * @brief	Sets up the path to the EXS
	 */
	void setupEXSPath();

	/**
	 * @brief	Writes the EXS facts.
	 *
	 * @param[in]	deviceDataHeartbeat				Heartbeat device data to write
	 * @param[in]	deviceDataExtendedSystemState	Extended system state device data to write
	 */
	void writeFacts(DeviceDataHeartbeat& deviceDataHeartbeat, DeviceDataExtendedSystemState& deviceDataExtendedSystemState);

	/**
	 * @brief	Executes the EXS.
	 */
	void executeEXS();

	/**
	 * @brief	Reads the EXS facts.
	 */
	void readFacts();

	/**
	 * @brief	Evaluates the EXS facts.
	 */
	void evaluateFacts();

protected:

	virtual bool onStarted() override;

	virtual void onStopped() override;

public:

	/**
	 * @brief	The constructor of EXSVehicle.
	 *
	 * @param[in]	arguments	Arguments of the MAL
	 */
	EXSVehicle(MALArguments& arguments);

	/**
	 * @brief	The constructor of EXSVehicle.
	 */
	~EXSVehicle();

	virtual void VehicleRoutine() override;
};