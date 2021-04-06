#pragma once

#include <mutex>
#include "Commands/TestVehicleCommand.h"
#include "Vehicles/CommandVehicle.h"

/**
 * @brief	The vehicle that is used for testing
 */
class TestVehicle : public CommandVehicle<TestVehicleCommand>
{
private:

	constexpr static const char* INDENT = "  ";

	constexpr static const unsigned int MAX_LEVENSHTEIN_COMMAND_DISTANCE = 2;

	/**
	 * @brief	Selects the command to execute given the string of a command.
	 *
	 * @param[in]	name			Name of the command to search for
	 * @param[out]	closestName		Closest name of the command
	 * @param[out]	distance		Distance to the selected command. Unchanged if no command was selected
	 *
	 * @return	Selected command. Note that even if a non-null value is returned, the distance to that command may be greater than zero. NULL if no command was selected
	 */
	TestVehicleCommand* selectCommand(const char* name, const char** closestName, size_t* distance);

	/**
	 * @brief	Returns whether the given command shall be handled
	 *
	 * @param[in]	command			Command to check whether to handle. May be NULL
	 * @param[out]	closestName		Closest name of the command
	 * @param[in]	distance		Distance to the command.
	 *
	 * @return	True if the command shall be handled; False otherwise
	 */
	bool handleCommand(TestVehicleCommand* command, const char* closestName, size_t commandDistance);

	/**
	 * @brief	Prints the command list
	 */
	void printCommandList();

public:

	/**
	 * @brief	The constructor of TestVehicle.
	 *
	 * @param[in]	arguments	Arguments of the MAL
	 */
	TestVehicle(MALArguments& arguments);

	/**
	 * @brief	The constructor of TestVehicle.
	 */
	~TestVehicle();

	virtual void VehicleRoutine() override;
};