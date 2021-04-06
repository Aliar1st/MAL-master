#pragma once

#include <vector>
#include "Vehicle.h"

/**
 * @brief	An abstract vehicle that uses commands
 *
 * @tparam	The type of vehicle commands
 */
template<typename T>
class CommandVehicle : public Vehicle
{
protected:

	std::vector<T*> commands;

public:

	/**
	 * @brief	The constructor of CommandVehicle<T>.
	 *
	 * @param[in]	vehicleName		Name of the vehicle
	 * @param		commands		A vector containing all commands
	 */
	CommandVehicle(const char* vehicleName, const std::vector<T*> commands)
		: Vehicle(vehicleName),
		  commands(commands)
	{
	}

	/**
	 * @brief	The destructor of CommandVehicle<T>.
	 */
	virtual ~CommandVehicle()
	{
		for (size_t i = 0; i < commands.size(); i++)
		{
			delete commands[i];
		}
	}
};