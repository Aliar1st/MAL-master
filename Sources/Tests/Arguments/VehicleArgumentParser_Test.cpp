#include <gtest/gtest.h>

#include "ArgumentParser_Test.h"
#include "Arguments/VehicleArgumentParser.h"
#include "Utilities/StringUtil.h"

namespace Arguments_Tests
{
	class VehicleArgumentParser_Test : public ArgumentParser_Test
	{
	protected:

		VehicleArgumentParser parser;

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
	TEST_F(VehicleArgumentParser_Test, NoArguments)
	{
		setupArguments({});

		VehicleArguments arguments;

		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));

		ASSERT_EQ(arguments.vehicle, Vehicles::TEST);
		ASSERT_FALSE(arguments.outdoor);
	}

	TEST_F(VehicleArgumentParser_Test, Vehicle)
	{
		setupArguments({"--vehicle", "test"});

		VehicleArguments arguments;

		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));

		ASSERT_EQ(arguments.vehicle, Vehicles::TEST);
		ASSERT_EQ(argc, 0);

		setupArguments({"-v", "exs"});

		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));

		ASSERT_EQ(arguments.vehicle, Vehicles::EXS);
		ASSERT_EQ(argc, 0);

		setupArguments({"-v", "cth"});

		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));

		ASSERT_EQ(arguments.vehicle, Vehicles::CTH);
		ASSERT_EQ(argc, 0);

		setupArguments({"-v", "rtl"});

		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));

		ASSERT_EQ(arguments.vehicle, Vehicles::RTL);
		ASSERT_EQ(argc, 0);
	}

	TEST_F(VehicleArgumentParser_Test, Outdoor)
	{
		setupArguments({"--outdoor"});

		VehicleArguments arguments;

		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));

		ASSERT_TRUE(arguments.outdoor);
		ASSERT_EQ(argc, 0);

		setupArguments({"-o"});

		arguments = VehicleArguments();

		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));

		ASSERT_TRUE(arguments.outdoor);
		ASSERT_EQ(argc, 0);
	}

	TEST_F(VehicleArgumentParser_Test, DuplicateArgument)
	{
		setupArguments({"-o", "-o"});

		VehicleArguments arguments;

		ASSERT_FALSE(parser.Parse(&argc, &argv, &arguments));
	}

	TEST_F(VehicleArgumentParser_Test, NonParsedArgument)
	{
		setupArguments({"--not_parsed_argument"});

		VehicleArguments arguments;

		ASSERT_TRUE(parser.Parse(&argc, &argv, &arguments));

		ASSERT_EQ(arguments.vehicle, Vehicles::TEST);
		ASSERT_FALSE(arguments.outdoor);
		ASSERT_EQ(argc, 1);
	}
}