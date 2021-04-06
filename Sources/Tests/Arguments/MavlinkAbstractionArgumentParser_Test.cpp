#include <gtest/gtest.h>

#include "ArgumentParser_Test.h"
#include "Arguments/MavlinkAbstractionArgumentParser.h"
#include "Utilities/StringUtil.h"

namespace Arguments_Tests
{
	class MavlinkAbstractionArgumentParser_Test : public ArgumentParser_Test
	{
	protected:

		MavlinkAbstractionArgumentParser parser;

	public:

		virtual void SetUp() override
		{
		}

		virtual void TearDown() override
		{
		}
	};

	// #####
	// TESTS
	// #####
	TEST_F(MavlinkAbstractionArgumentParser_Test, NoArguments)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({});
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(MavlinkAbstractionArgumentParser_Test, Serial)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({"--mavconntype", "serial", "--port", "SERIAL_PORT_NAME", "--baudrate", "1234"});
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.mavlinkConnectionType, MAVLink_Connection_Type::SERIAL);
		ASSERT_STREQ(arguments.serialPortName, "SERIAL_PORT_NAME");
		ASSERT_EQ(arguments.baudRate, 1234);
		ASSERT_EQ(argc, 0);

		setupArguments({"-m", "serial", "-p", "SERIAL_PORT_NAME", "-b", "1234"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.mavlinkConnectionType, MAVLink_Connection_Type::SERIAL);
		ASSERT_STREQ(arguments.serialPortName, "SERIAL_PORT_NAME");
		ASSERT_EQ(arguments.baudRate, 1234);
		ASSERT_EQ(argc, 0);

		setupArguments({"-m", "serial", "-p", "SERIAL_PORT_NAME", "-b", "0"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(MavlinkAbstractionArgumentParser_Test, UDP)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({"--mavconntype", "udp", "--port", "1234"});
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.mavlinkConnectionType, MAVLink_Connection_Type::UDP);
		ASSERT_EQ(arguments.socketPort, 1234);
		ASSERT_EQ(argc, 0);

		setupArguments({"-m", "udp", "-p", "1234"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.mavlinkConnectionType, MAVLink_Connection_Type::UDP);
		ASSERT_EQ(arguments.socketPort, 1234);
		ASSERT_EQ(argc, 0);

		setupArguments({"-m", "udp", "-p", "0"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));

		setupArguments({"-m", "udp", "-p", "100000"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(MavlinkAbstractionArgumentParser_Test, TCP)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({"--mavconntype", "tcp", "--port", "1234"});
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.mavlinkConnectionType, MAVLink_Connection_Type::TCP);
		ASSERT_EQ(arguments.socketPort, 1234);
		ASSERT_EQ(argc, 0);

		setupArguments({"-m", "tcp", "-p", "1234"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.mavlinkConnectionType, MAVLink_Connection_Type::TCP);
		ASSERT_EQ(arguments.socketPort, 1234);
		ASSERT_EQ(argc, 0);

		setupArguments({"-m", "tcp", "-p", "0"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));

		setupArguments({"-m", "tcp", "-p", "100000"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(MavlinkAbstractionArgumentParser_Test, OwnSystemID)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({"-m", "tcp", "-p", "1234", "--systemid", "19"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.ownSystemID, 19);

		setupArguments({"-m", "tcp", "-p", "1234", "-s", "23"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.ownSystemID, 23);

		setupArguments({"-m", "tcp", "-p", "1234", "-s", "hello", });
		arguments = MavlinkAbstractionArguments();
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(MavlinkAbstractionArgumentParser_Test, TargetSystemID)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({"-m", "tcp", "-p", "1234", "--targetid", "19"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.targetSystemID, 19);

		setupArguments({"-m", "tcp", "-p", "1234", "-t", "23"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.targetSystemID, 23);

		setupArguments({"-m", "tcp", "-p", "1234", "-t", "hello", });
		arguments = MavlinkAbstractionArguments();
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(MavlinkAbstractionArgumentParser_Test, Log)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({"-m", "tcp", "-p", "1234", "--log", "none"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.loggerMode, Logger_Mode::NONE);

		setupArguments({"-m", "tcp", "-p", "1234", "-l", "error"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.loggerMode, Logger_Mode::ERRORS);

		setupArguments({"-m", "tcp", "-p", "1234", "-l", "status"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.loggerMode, Logger_Mode::STATUS);

		setupArguments({"-m", "tcp", "-p", "1234", "-l", "info"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(arguments.loggerMode, Logger_Mode::INFO);

		setupArguments({"-m", "tcp", "-p", "1234", "-l", "info", "-l", "info"});
		arguments = MavlinkAbstractionArguments();
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(MavlinkAbstractionArgumentParser_Test, NotEnoughArguments)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({"--mavconntype", "serial"});
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(MavlinkAbstractionArgumentParser_Test, DuplicateArgument)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({"-m", "tcp", "-p", "1234", "-p", "1234"});
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(MavlinkAbstractionArgumentParser_Test, NonParsedArgument)
	{
		MavlinkAbstractionArguments arguments;

		setupArguments({"--not_parsed_argument"});
		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
		ASSERT_EQ(argc, 1);
	}
}