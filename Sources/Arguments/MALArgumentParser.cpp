#include "MALArgumentParser.h"

#include <iostream>
#include "MavlinkAbstractionArgumentParser.h"
#include "Utilities/StringUtil.h"
#include "VehicleArgumentParser.h"

bool MALArgumentParser::Parse(size_t argc, char* argv[], MALArguments* arguments)
{
	MavlinkAbstractionArgumentParser mavlinkAbstractionArgumentParser;
	VehicleArgumentParser vehicleArgumentParser;

	// The first argument is (almost) always the program path itself so we ignore it
	argc--;
	argv++;

	// Copy the arguments
	size_t argcCopy = argc;
	char** argvCopy = new char*[argcCopy];

	for (size_t i = 0; i < argcCopy; i++)
	{
		argvCopy[i] = StringUtil::Strcpy(argv[i]);
	}

	// Parse
	bool success = false;

	if (mavlinkAbstractionArgumentParser.Parse(&argcCopy, &argvCopy, &arguments->mavlinkAbstractionArguments) &&
		vehicleArgumentParser.Parse(&argcCopy, &argvCopy, &arguments->vehicleArguments))
	{
		success = argcCopy == 0;
	}

	if (!success)
	{
		// Print help
		std::cout << "Arguments:" << std::endl;

		mavlinkAbstractionArgumentParser.PrintHelp();
		vehicleArgumentParser.PrintHelp();
	}

	// Delete the arguments
	for (size_t i = 0; i < argcCopy; i++)
	{
		delete[] argvCopy[i];
	}
	delete[] argvCopy;

	return success;
}