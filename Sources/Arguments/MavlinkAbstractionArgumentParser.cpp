#include "MavlinkAbstractionArgumentParser.h"

#include <iostream>
#include "Utilities/ConversionUtil.h"
#include "Utilities/StringUtil.h"

void MavlinkAbstractionArgumentParser::PrintHelp()
{
	// MAVLink Connection type
	std::cout << INDENT << "--mavconntype, -m" << std::endl;
	std::cout << INDENT << INDENT << "The next argument specifies the type of the MAVLink connection used" << std::endl;
	std::cout << INDENT << INDENT << "This argument is mandatory" << std::endl;
	std::cout << INDENT << INDENT << "Possible argument values are:" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "serial" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "A serial port is used for the MAVLink connection" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "Mandatory additional arguments:" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << "--port, -p" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << INDENT << "The next argument specifies the name of the serial port" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << "--baudrate, -b" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << INDENT << "The next argument specifies the baud rate of the serial port" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << INDENT << "Must be greater than 0" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "udp" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "A UDP socket is used for the MAVLink connection" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "Mandatory additional arguments:" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << "--port, -p" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << INDENT << "The next argument specifies the port of the UDP socket" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << INDENT << "Must be in between 0 (excluding) and 65535 (including)" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "tcp" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "A TCP socket is used for the MAVLink connection" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "Mandatory additional arguments:" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << "--port, -p" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << INDENT << "The next argument specifies the port of the TCP socket" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << INDENT << INDENT << "Must be in between 0 (excluding) and 65535 (including)" << std::endl;

	// SystemID
	std::cout << INDENT << "--systemid, -s" << std::endl;
	std::cout << INDENT << INDENT << "The next argument specifies the own MAVLink system ID." << std::endl;
	std::cout << INDENT << INDENT << "Must be in between 0 (excluding) and 255 (including)" << std::endl;
	std::cout << INDENT << INDENT << "The default value is 17" << std::endl;

	// TargetID
	std::cout << INDENT << "--targetid, -t" << std::endl;
	std::cout << INDENT << INDENT << "The next argument specifies the target's MAVLink system ID." << std::endl;
	std::cout << INDENT << INDENT << "Must be in between 0 (excluding) and 255 (including)" << std::endl;
	std::cout << INDENT << INDENT << "The default value is 1" << std::endl;

	// Logger mode
	std::cout << INDENT << "--log, -l" << std::endl;
	std::cout << INDENT << INDENT << "The next argument specifies the logging mode" << std::endl;
	std::cout << INDENT << INDENT << "The default value is none" << std::endl;
	std::cout << INDENT << INDENT << "Possible argument values are:" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "none" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "No logging" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "error" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "Errors will be logged" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "status" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "Errors and status messages will be logged" << std::endl;
	std::cout << INDENT << INDENT << INDENT << "info" << std::endl;
	std::cout << INDENT << INDENT << INDENT << INDENT << "Everything will be logged" << std::endl;
}

void MavlinkAbstractionArgumentParser::preParse()
{
	mavlinkConnectionTypeParsed = false;
	portParsed = false;
	baudRateParsed = false;

	ownSystemIDParsed = false;
	targetSystemIDParsed = false;

	loggerModeParsed = false;
}

bool MavlinkAbstractionArgumentParser::postParse(Arguments* arguments)
{
	MavlinkAbstractionArguments* mavlinkAbstractionArguments = static_cast<MavlinkAbstractionArguments*>(arguments);

	return parsedValidMAVLinkConnectionType(mavlinkAbstractionArguments) &&
		   parsedValidPort(mavlinkAbstractionArguments) &&
		   (mavlinkAbstractionArguments->mavlinkConnectionType != MAVLink_Connection_Type::SERIAL || parsedValidBaudRate(mavlinkAbstractionArguments));
}

bool MavlinkAbstractionArgumentParser::parseArgument(size_t argc, char* argv[], size_t* argumentsParsed, Arguments* arguments)
{
	MavlinkAbstractionArguments* mavlinkAbstractionArguments = static_cast<MavlinkAbstractionArguments*>(arguments);

	*argumentsParsed = 0;

	if (isArgument(argv[0], {"--mavconntype", "-m"}))
	{
		(*argumentsParsed)++;

		return parseMAVLinkConnectionType(argc, argv, argumentsParsed, mavlinkAbstractionArguments);
	}
	if (isArgument(argv[0], {"--port", "-p"}))
	{
		(*argumentsParsed)++;

		return parsePort(argc, argv, argumentsParsed, mavlinkAbstractionArguments);
	}
	if (isArgument(argv[0], {"--baudrate", "-b"}))
	{
		(*argumentsParsed)++;

		return parseBaudRate(argc, argv, argumentsParsed, mavlinkAbstractionArguments);
	}
	if (isArgument(argv[0], {"--systemid", "-s"}))
	{
		(*argumentsParsed)++;

		return parseOwnSystemID(argc, argv, argumentsParsed, mavlinkAbstractionArguments);
	}
	if (isArgument(argv[0], {"--targetid", "-t"}))
	{
		(*argumentsParsed)++;

		return parseTargetSystemID(argc, argv, argumentsParsed, mavlinkAbstractionArguments);
	}
	if (isArgument(argv[0], {"--log", "-l"}))
	{
		(*argumentsParsed)++;

		return parseLoggerMode(argc, argv, argumentsParsed, mavlinkAbstractionArguments);
	}

	return true;
}

bool MavlinkAbstractionArgumentParser::parseMAVLinkConnectionType(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments)
{
	if (argc == 1 || mavlinkConnectionTypeParsed)
	{
		return false;
	}

	(*argumentsParsed)++;
	mavlinkConnectionTypeParsed = true;

	// Next argument specifies the MAVLink Connection type
	char*& nextArgument = argv[1];

	if (isArgument(nextArgument, "serial"))
	{
		arguments->mavlinkConnectionType = MAVLink_Connection_Type::SERIAL;
	}
	else if (isArgument(nextArgument, "udp"))
	{
		arguments->mavlinkConnectionType = MAVLink_Connection_Type::UDP;
	}
	else if (isArgument(nextArgument, "tcp"))
	{
		arguments->mavlinkConnectionType = MAVLink_Connection_Type::TCP;
	}
	else
	{
		// An unknown type was provided
		return false;
	}

	return true;
}

bool MavlinkAbstractionArgumentParser::parsePort(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments)
{
	if (argc == 1 || portParsed)
	{
		return false;
	}

	(*argumentsParsed)++;
	portParsed = true;

	// Next argument specifies the port
	char*& nextArgument = argv[1];

	// We may not know the MAVLink Connection type yet so we set both ports
	arguments->SetSerialPortName(nextArgument);
	arguments->socketPort = StringUtil::IsUnsignedInt(nextArgument) ? ConversionUtil::StrToUnsignedInt(nextArgument) : 0;

	return true;
}

bool MavlinkAbstractionArgumentParser::parseBaudRate(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments)
{
	if (argc == 1 || !mavlinkConnectionTypeParsed || arguments->mavlinkConnectionType != MAVLink_Connection_Type::SERIAL || baudRateParsed)
	{
		return false;
	}

	(*argumentsParsed)++;
	baudRateParsed = true;

	// Next argument specifies the baud rate
	char*& nextArgument = argv[1];

	arguments->baudRate = StringUtil::IsUnsignedInt(nextArgument) ? ConversionUtil::StrToUnsignedInt(nextArgument) : 0;

	return true;
}

bool MavlinkAbstractionArgumentParser::parseOwnSystemID(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments)
{
	if (argc == 1 || ownSystemIDParsed)
	{
		return false;
	}

	(*argumentsParsed)++;
	ownSystemIDParsed = true;

	// Next argument specifies the own system ID
	char*& nextArgument = argv[1];

	if (StringUtil::IsInt(nextArgument))
	{
		arguments->ownSystemID = static_cast<uint8_t>(ConversionUtil::StrToInt(nextArgument));

		return arguments->ownSystemID > 0 && arguments->ownSystemID <= 255;
	}

	return false;
}

bool MavlinkAbstractionArgumentParser::parseTargetSystemID(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments)
{
	if (argc == 1 || targetSystemIDParsed)
	{
		return false;
	}

	(*argumentsParsed)++;
	targetSystemIDParsed = true;

	// Next argument specifies the target's system ID
	char*& nextArgument = argv[1];

	if (StringUtil::IsInt(nextArgument))
	{
		arguments->targetSystemID = static_cast<uint8_t>(ConversionUtil::StrToInt(nextArgument));

		return arguments->targetSystemID > 0 && arguments->targetSystemID <= 255;
	}

	return false;
}

bool MavlinkAbstractionArgumentParser::parseLoggerMode(size_t argc, char* argv[], size_t* argumentsParsed, MavlinkAbstractionArguments* arguments)
{
	if (argc == 1 || loggerModeParsed)
	{
		return false;
	}

	// Next argument specifies the logger mode
	char*& nextArgument = argv[1];

	if (isArgument(nextArgument, "none"))
	{
		arguments->loggerMode = Logger_Mode::NONE;
	}
	else if (isArgument(nextArgument, "error"))
	{
		arguments->loggerMode = Logger_Mode::ERRORS;
	}
	else if (isArgument(nextArgument, "status"))
	{
		arguments->loggerMode = Logger_Mode::STATUS;
	}
	else if (isArgument(nextArgument, "info"))
	{
		arguments->loggerMode = Logger_Mode::INFO;
	}
	else
	{
		// An unknown logging mode was provided
		return false;
	}

	(*argumentsParsed)++;
	loggerModeParsed = true;

	return true;
}

bool MavlinkAbstractionArgumentParser::parsedValidMAVLinkConnectionType(MavlinkAbstractionArguments* arguments)
{
	return mavlinkConnectionTypeParsed;
}

bool MavlinkAbstractionArgumentParser::parsedValidPort(MavlinkAbstractionArguments* arguments)
{
	if (!portParsed)
	{
		return false;
	}

	if (arguments->mavlinkConnectionType != MAVLink_Connection_Type::SERIAL)
	{
		// UDP/TCP
		return arguments->socketPort > 0 && arguments->socketPort <= 65535;
	}

	// We do not check the serial port's name
	return true;
}

bool MavlinkAbstractionArgumentParser::parsedValidBaudRate(MavlinkAbstractionArguments* arguments)
{
	if (!baudRateParsed)
	{
		return false;
	}

	if (arguments->mavlinkConnectionType == MAVLink_Connection_Type::SERIAL)
	{
		// Serial
		return arguments->baudRate > 0;
	}

	return true;
}