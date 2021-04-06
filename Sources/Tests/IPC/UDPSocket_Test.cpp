#include <gtest/gtest.h>

#include "IPC/UDPSocket.h"
#include "Socket_Test.h"

namespace IPC_Tests
{
	class UDPSocket_Test : public Socket_Test
	{
	protected:

		constexpr static const int UDP_SOCKET_PORT = 7523;

		UDPSocket socket;

		virtual Connection* getConnection() override
		{
			return &socket;
		}

		virtual Connection* mainThreadStart() override
		{
			[&]()->void {ASSERT_EQ(CONNECTION_RETURN_SUCCESS, socket.OpenBound(UDP_SOCKET_PORT));}();

			return &socket;
		}

		virtual void mainThreadPreRead(Connection* mainThreadConnection) override
		{
		}

		virtual void mainThreadPostRead(Connection* mainThreadConnection) override
		{
			char* sourceAddress;
			socket_port_t sourcePort;

			socket.GetSourceAddress(&sourceAddress, &sourcePort);

			ASSERT_NE(nullptr, sourceAddress);
			ASSERT_NE(0, sourcePort);

			ASSERT_EQ(CONNECTION_RETURN_SUCCESS, socket.SetTargetAddress(sourceAddress, sourcePort));

			delete[] sourceAddress;
		}

		virtual void mainThreadPostWrite(Connection* mainThreadConnection) override
		{
		}

		virtual void mainThreadEnd(Connection* mainThreadConnection) override
		{
		}

		virtual Connection* secondaryThreadStart(Connection* mainThreadConnection) override
		{
			UDPSocket* secondaryThreadSocket = new UDPSocket;

			[&]()->void {ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadSocket->Open());}();
			[&]()->void {ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadSocket->SetTargetAddress("127.0.0.1", UDP_SOCKET_PORT));}();

			return secondaryThreadSocket;
		}

		virtual void secondaryThreadPostWrite(Connection* secondaryThreadConnection, Connection* mainThreadConnection) override
		{
		}

		virtual void secondaryThreadPostRead(Connection* secondaryThreadConnection, Connection* mainThreadConnection) override
		{
			UDPSocket* secondaryThreadSocket = static_cast<UDPSocket*>(secondaryThreadConnection);

			char* sourceAddress;
			socket_port_t sourcePort;

			secondaryThreadSocket->GetSourceAddress(&sourceAddress, &sourcePort);

			ASSERT_NE(nullptr, sourceAddress);
			ASSERT_NE(0, sourcePort);

			delete[] sourceAddress;
		}

		virtual void secondaryThreadEnd(Connection* secondaryThreadConnection, Connection* mainThreadConnection) override
		{
			delete secondaryThreadConnection;
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
	TEST_F(UDPSocket_Test, InitialValues)
	{
		Socket_Test::InitialValues();

		char* sourceAddress;
		socket_port_t sourcePort;

		socket.GetSourceAddress(&sourceAddress, &sourcePort);
		ASSERT_STREQ("", sourceAddress);
		ASSERT_EQ(static_cast<socket_port_t>(0), sourcePort);

		delete[] sourceAddress;
	}

	TEST_F(UDPSocket_Test, ManipulateUnopened)
	{
		Socket_Test::ManipulateUnopened();

		ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, socket.SetTargetAddress("", 0));
	}

	TEST_F(UDPSocket_Test, Open)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, socket.Open());
		ASSERT_TRUE(socket.IsOpened());
	}

	TEST_F(UDPSocket_Test, OpenBound)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, socket.OpenBound(UDP_SOCKET_PORT));
		ASSERT_TRUE(socket.IsOpened());
	}

	TEST_F(UDPSocket_Test, ReadTimeout)
	{
		Socket_Test::ReadTimeout();
	}

	TEST_F(UDPSocket_Test, ReadCancel)
	{
		Socket_Test::ReadCancel();
	}

	TEST_F(UDPSocket_Test, ReadWriteArray)
	{
		Socket_Test::ReadWriteArray();
	}

	TEST_F(UDPSocket_Test, ReadWriteSingle)
	{
		Socket_Test::ReadWriteSingle();
	}

	TEST_F(UDPSocket_Test, ReadWriteString)
	{
		Socket_Test::ReadWriteString();
	}

	TEST_F(UDPSocket_Test, IsError)
	{
		Socket_Test::IsError();
	}
}