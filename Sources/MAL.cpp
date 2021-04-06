// gtest uses its own main function so we mustn't define one on our own
#ifndef GTEST

#include <iostream>
#include "Arguments/MALArgumentParser.h"
#include "Vehicles/CTHVehicle/CTHVehicle.h"
#include "Vehicles/EXSVehicle/EXSVehicle.h"
#include "Vehicles/RTLVehicle/RTLVehicle.h"
#include "Vehicles/TestVehicle/TestVehicle.h"

int main(int argc, char** argv)
{
	MALArguments arguments;

	if (MALArgumentParser().Parse(argc, argv, &arguments))
	{
		// Initialize the vehicle
		Vehicle* vehicle;
		switch (arguments.vehicleArguments.vehicle)
		{
			default:
			case Vehicles::TEST:
			{
				vehicle = new TestVehicle(arguments);
				break;
			}

			case Vehicles::EXS:
			{
				vehicle = new EXSVehicle(arguments);
				break;
			}

			case Vehicles::CTH:
			{
				vehicle = new CTHVehicle(arguments);
				break;
			}

			case Vehicles::RTL:
			{
				vehicle = new RTLVehicle(arguments);
				break;
			}
		}

		if (vehicle->IsStarted())
		{
			std::cout << "-------------------------" << std::endl;

			// Loop vehicle routine
			while (vehicle->IsStarted())
			{
				vehicle->VehicleRoutine();
			}
		}

		delete vehicle;

		return 0;
	}

	return -1;
}

#endif // GTEST