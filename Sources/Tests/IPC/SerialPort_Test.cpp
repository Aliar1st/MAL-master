#include <gtest/gtest.h>

#include "Connection_Test.h"
#include "IPC/SerialPort.h"

namespace IPC_Tests
{
	class SerialPort_Test : public Connection_Test
	{
	protected:

		constexpr static const char SERIAL_PORT_NAME[] = "TEST_SERIAL_PORT_NAME";

		SerialPort serialPort;

		virtual Connection* getConnection() override
		{
			return &serialPort;
		}

		/**
		 * The reading and writing on serial ports cannot be tested as that requires a phyiscal connection
		 */
		virtual Connection* mainThreadStart() override
		{
			return NULL;
		}

		virtual void mainThreadPreRead(Connection* mainThreadConnection) override
		{
		}

		virtual void mainThreadPostRead(Connection* mainThreadConnection) override
		{
		}

		virtual void mainThreadPostWrite(Connection* mainThreadConnection) override
		{
		}

		virtual void mainThreadEnd(Connection* mainThreadConnection) override
		{
		}

		virtual Connection* secondaryThreadStart(Connection* mainThreadConnection) override
		{
			return NULL;
		}

		virtual void secondaryThreadPostWrite(Connection* secondaryThreadConnection, Connection* mainThreadConnection) override
		{
		}

		virtual void secondaryThreadPostRead(Connection* secondaryThreadConnection, Connection* mainThreadConnection) override
		{
		}

		virtual void secondaryThreadEnd(Connection* secondaryThreadConnection, Connection* mainThreadConnection) override
		{
		}

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
	TEST_F(SerialPort_Test, InitialValues)
	{
		Connection_Test::InitialValues();

		ASSERT_EQ(NULL, serialPort.GetName());
	}

	TEST_F(SerialPort_Test, ManipulateUnopened)
	{
		Connection_Test::ManipulateUnopened();
	}

	TEST_F(SerialPort_Test, OpenNonExistent)
	{
		ASSERT_EQ(SERIAL_PORT_RETURN_ERROR_COULD_NOT_OPEN_PORT, serialPort.Open(SERIAL_PORT_NAME, 57600));
	}

	TEST_F(SerialPort_Test, IsError)
	{
		Connection_Test::IsError();
		ASSERT_TRUE(serialPort.IsError(SERIAL_PORT_RETURN_ERROR_COULD_NOT_OPEN_PORT));
		ASSERT_TRUE(serialPort.IsError(SERIAL_PORT_RETURN_ERROR_COULD_NOT_GET_PORTS_STATE));
		ASSERT_TRUE(serialPort.IsError(SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_STATE));
		ASSERT_TRUE(serialPort.IsError(SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_TIMEOUTS));
	}
}