#pragma once

#include "ArgumentParser.h"
#include "MavlinkAbstractionArguments.h"

/**
 * @brief	Parser of the Mavlink Abstraction arguments
 */
class MavlinkAbstractionArgumentParser : public ArgumentParser
{
private:

	bool mavlinkConnectionTypeParsed;
	bool portParsed;
	bool baudRateParsed;

	bool ownSystemIDParsed;
	bool targetSystemIDParsed;

	bool loggerModeParsed;

	/**
	 * @brief	Parses the MAVLink connection type.
	 *
	 * @param		argc				The amount of arguments provided.
	 * @param		argv				The array containing all remaining arguments. The first element is the already parsed flag
	 * @param[out]	argumentsParsed		After this function returns, contains the amount of arguments that were parsed.
	 * @param[out]	arguments			After this function returns, contains the data of the parsed argument if there was any
	 *
	 * @return	True if the argument was parsed without any errors; False if any arguments were missing
	 */
	bool parseMAVLinkConnectionType(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments);

	/**
	 * @brief	Parses the serial port name or the socket port.
	 *
	 * @param		argc				The amount of arguments provided.
	 * @param		argv				The array containing all remaining arguments. The first element is the already parsed flag
	 * @param[out]	argumentsParsed		After this function returns, contains the amount of arguments that were parsed.
	 * @param[out]	arguments			After this function returns, contains the data of the parsed argument if there was any
	 *
	 * @return	True if the argument was parsed without any errors; False if any arguments were missing
	 */
	bool parsePort(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments);

	/**
	 * @brief	Parses the baud rate.
	 *
	 * @param		argc				The amount of arguments provided.
	 * @param		argv				The array containing all remaining arguments. The first element is the already parsed flag
	 * @param[out]	argumentsParsed		After this function returns, contains the amount of arguments that were parsed.
	 * @param[out]	arguments			After this function returns, contains the data of the parsed argument if there was any
	 *
	 * @return	True if the argument was parsed without any errors; False if any arguments were missing
	 */
	bool parseBaudRate(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments);

	/**
	 * @brief	Parses the own MAVLink system ID.
	 *
	 * @param		argc				The amount of arguments provided.
	 * @param		argv				The array containing all remaining arguments. The first element is the already parsed flag
	 * @param[out]	argumentsParsed		After this function returns, contains the amount of arguments that were parsed.
	 * @param[out]	arguments			After this function returns, contains the data of the parsed argument if there was any
	 *
	 * @return	True if the argument was parsed without any errors; False if any arguments were missing
	 */
	bool parseOwnSystemID(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments);

	/**
	 * @brief	Parses the target's MAVLink system ID.
	 *
	 * @param		argc				The amount of arguments provided.
	 * @param		argv				The array containing all remaining arguments. The first element is the already parsed flag
	 * @param[out]	argumentsParsed		After this function returns, contains the amount of arguments that were parsed.
	 * @param[out]	arguments			After this function returns, contains the data of the parsed argument if there was any
	 *
	 * @return	True if the argument was parsed without any errors; False if any arguments were missing
	 */
	bool parseTargetSystemID(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments);

	/**
	 * @brief	Parses the logger mode.
	 *
	 * @param		argc				The amount of arguments provided.
	 * @param		argv				The array containing all remaining arguments. The first element is the already parsed flag
	 * @param[out]	argumentsParsed		After this function returns, contains the amount of arguments that were parsed.
	 * @param[out]	arguments			After this function returns, contains the data of the parsed argument if there was any
	 *
	 * @return	True if the argument was parsed without any errors; False if any arguments were missing
	 */
	bool parseLoggerMode(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments);

	/**
	 * @brief	Checks whether the MAVLink connection type was parsed and contains valid data.
	 *
	 * @param[in]	arguments	Parsed arguments
	 *
	 * @return	True if the MAVLink connection type was parsed and contains valid data; False otherwise
	 */
	bool parsedValidMAVLinkConnectionType(MavlinkAbstractionArguments* arguments);

	/**
	 * @brief	Checks whether the port was parsed and contains valid data.
	 *
	 * @param[in]	arguments	Parsed arguments
	 *
	 * @return	True if the port was parsed and contains valid data; False otherwise
	 */
	bool parsedValidPort(MavlinkAbstractionArguments* arguments);

	/**
	 * @brief	Checks whether the baud rate was parsed and contains valid data.
	 *
	 * @param[in]	arguments	Parsed arguments
	 *
	 * @return	True if the baud rate was parsed and contains valid data; False otherwise
	 */
	bool parsedValidBaudRate(MavlinkAbstractionArguments* arguments);

protected:

	virtual void preParse() override;

	virtual bool postParse(Arguments* arguments) override;

	virtual bool parseArgument(size_t argc, char* argv[], size_t* argumentsParsed, Arguments* arguments) override;

public:

	virtual void PrintHelp() override;
};