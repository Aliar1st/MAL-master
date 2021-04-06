#pragma once

#include "ArgumentParser.h"
#include "VehicleArguments.h"

/**
 * @brief	Parser of the vehicle arguments
 */
class VehicleArgumentParser : public ArgumentParser
{
private:

	bool vehicleParsed = false;
	bool outdoorParsed = false;

	/**
	 * @brief	Parses the vehicle.
	 *
	 * @param		argc				The amount of arguments provided.
	 * @param		argv				The array containing all remaining arguments. The first element is the already parsed flag
	 * @param[out]	argumentsParsed		After this function returns, contains the amount of arguments that were parsed.
	 * @param[out]	arguments			After this function returns, contains the data of the parsed argument if there was any
	 *
	 * @return	True if the argument was parsed without any errors; False if any arguments were missing
	 */
	bool parseVehicle(size_t argc, char* argv[], size_t* argumentsParsed, VehicleArguments* arguments);

	/**
	 * @brief	Parses the outdoor flag
	 *
	 * @param		argc				The amount of arguments provided.
	 * @param		argv				The array containing all remaining arguments. The first element is the already parsed flag
	 * @param[out]	argumentsParsed		After this function returns, contains the amount of arguments that were parsed.
	 * @param[out]	arguments			After this function returns, contains the data of the parsed argument if there was any
	 *
	 * @return	True if the argument was parsed without any errors; False if any arguments were missing
	 */
	bool parseOutdoor(size_t argc, char* argv[], size_t* argumentsParsed, VehicleArguments* arguments);

protected:

	virtual void preParse() override;

	virtual bool postParse(Arguments* arguments) override;

	virtual bool parseArgument(size_t argc, char* argv[], size_t* argumentsParsed, Arguments* arguments) override;

public:

	virtual void PrintHelp() override;
};