#include "VehicleArgumentParser.h"

#include <iostream>

void VehicleArgumentParser::PrintHelp()
{
	// Vehicles
	std::cout << INDENT << "--vehicle, -v" << std::endl;
	std::cout << INDENT << INDENT << "The next argument specifies the vehicle to launch" << std::endl;
	std::cout << INDENT << INDENT << "The default value is test" << std::endl;
	std::cout << INDENT << INDENT << "Possible argument values are:" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "test" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "The Test Vehicles is launched" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "exs" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "The EXS Vehicles is launched" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "cth" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "The CTH Vehicles is launched" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "rtl" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "The RTL Vehicles is launched" << std::endl;

	// Outdoor
	std::cout << INDENT << "--outdoor, -o" << std::endl;
	std::cout << INDENT << INDENT << "If specified, the vehicle will use its outdoor setup (if existing)." << std::endl;
}

void VehicleArgumentParser::preParse()
{
	vehicleParsed = false;
	outdoorParsed = false;
}

bool VehicleArgumentParser::postParse(Arguments* arguments)
{
	// No arguments are required
	return true;
}

bool VehicleArgumentParser::parseArgument(size_t argc, char* argv[], size_t* argumentsParsed, Arguments* arguments)
{
	VehicleArguments* vehicleArguments = static_cast<VehicleArguments*>(arguments);

	*argumentsParsed = 0;

	if (isArgument(argv[0], {"--vehicle", "-v"}))
	{
		(*argumentsParsed)++;

		return parseVehicle(argc, argv, argumentsParsed, vehicleArguments);
	}
	if (isArgument(argv[0], {"--outdoor", "-o"}))
	{
		(*argumentsParsed)++;

		return parseOutdoor(argc, argv, argumentsParsed, vehicleArguments);
	}

	return true;
}

bool VehicleArgumentParser::parseVehicle(size_t argc, char* argv[], size_t* argumentsParsed, VehicleArguments* arguments)
{
	if (argc == 1 || vehicleParsed)
	{
		return false;
	}

	(*argumentsParsed)++;
	vehicleParsed = true;

	// Next argument specifies the vehicle
	char*& nextArgument = argv[1];

	if (isArgument(nextArgument, "test"))
	{
		arguments->vehicle = Vehicles::TEST;
	}
	else if (isArgument(nextArgument, "exs"))
	{
		arguments->vehicle = Vehicles::EXS;
	}
	else if (isArgument(nextArgument, "cth"))
	{
		arguments->vehicle = Vehicles::CTH;
	}
	else if (isArgument(nextArgument, "rtl"))
	{
		arguments->vehicle = Vehicles::RTL;
	}
	else
	{
		// An unknown vehicle was provided
		return false;
	}

	return true;
}

bool VehicleArgumentParser::parseOutdoor(size_t argc, char* argv[], size_t* argumentsParsed, VehicleArguments* arguments)
{
	if (outdoorParsed)
	{
		return false;
	}

	arguments->outdoor = true;

	outdoorParsed = true;

	return true;
}