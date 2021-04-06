#pragma once

#include "Arguments/MALArguments.h"
#include "MavlinkAbstraction/MavlinkAbstraction.h"

/**
 * @brief	An abstract vehicle class
 */
class Vehicle : public MavlinkAbstraction
{
private:

	constexpr static const unsigned int CONNECTION_TIMEOUT = 2000;

	bool outdoor;

protected:

	/**
	 * @brief	Starts the Mavlink Abstraction repeatedly until it could be started.
	 *
	 * @param[in]	arguments	Arguments of the MAL
	 * @param		print		True to print status messages; False otherwise
	 */
	void StartRepeatedly(MALArguments& arguments, bool print);

public:

	/**
	 * @brief	The constructor of Vehicle.
	 *
	 * @param[in]	vehicleName		Name of the vehicle
	 */
	Vehicle(const char* vehicleName);

	/**
	 * @brief	The destructor of Vehicle.
	 */
	virtual ~Vehicle();

	/**
	 * @brief	The routine to perform for this vehicle.
	 *			This function is called as long as the vehicle is started
	 */
	virtual void VehicleRoutine() = 0;

	/**
	 * @brief	Returns whether the vehicle is outdoor
	 *
	 * @return	True if the vehicle is outdoor; False otherwise
	 */
	bool IsOutdoor();
};