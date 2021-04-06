#include "Vehicle.h"

#include <iostream>

Vehicle::Vehicle(const char* vehicleName)
{
	std::cout << "Created " << vehicleName << std::endl;
}

Vehicle::~Vehicle()
{
}

void Vehicle::StartRepeatedly(MALArguments& arguments, bool print)
{
	MavlinkAbstraction_Return startReturn = MAVLINK_ABSTRACTION_RETURN_SUCCESS;

	do
	{
		if (print && startReturn != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
		{
			std::cout << "Could not start vehicle (tried for " << CONNECTION_TIMEOUT << "ms). Trying again..." << std::endl;
		}

		startReturn = Start(arguments.mavlinkAbstractionArguments, CONNECTION_TIMEOUT);
	}
	while (startReturn != MAVLINK_ABSTRACTION_RETURN_SUCCESS);

	// The vehicle may have stopped the Mavlink Abstraction already
	if (!IsStarted())
	{
		return;
	}

	outdoor = arguments.vehicleArguments.outdoor;

	if (print)
	{
		std::cout << "Started Vehicles" << std::endl;
		std::cout << "Outdoor: " << (outdoor ? "YES" : "NO") << std::endl;
	}
}

bool Vehicle::IsOutdoor()
{
	return outdoor;
}