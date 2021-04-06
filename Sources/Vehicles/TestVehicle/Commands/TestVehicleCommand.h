#pragma once

#include "Vehicles/Commands/VehicleCommand.h"

/**
 * @brief	A base class for a command for the test vehicle
 */
class TestVehicleCommand : public VehicleCommand
{
public:

	/**
	 * @brief	Returns a vector containing all alternative
	 *
	 * @return	Vector of all alternative names.
	 */
	virtual const std::vector<const char*> GetAlternativeNames();

	/**
	 * @brief	Returns a vector containing all names
	 *
	 * @return	Vector of all names.
	 */
	const std::vector<const char*> GetNames();

	/**
	 * @brief	Returns the description of this command
	 *
	 * @return	Description
	 */
	virtual const char* GetDescription() = 0;
};