#include <gtest/gtest.h>

#include "IPC/TCPSocket.h"
#include "Socket_Test.h"

namespace IPC_Tests
{
	class TCPSocket_Test : public Socket_Test
	{
	protected:

		constexpr static const int TCP_SOCKET_PORT = 7524;

		TCPSocket socket;

		virtual Connection* getConnection() override
		{
			return &socket;
		}

		virtual Connection* mainThreadStart() override
		{
			[&]()->void {ASSERT_EQ(CONNECTION_RETURN_SUCCESS, socket.CreateServer(TCP_SOCKET_PORT));}();

			return new TCPSocket();
		}

		virtual void mainThreadPreRead(Connection* mainThreadConnection) override
		{
			ASSERT_EQ(CONNECTION_RETURN_SUCCESS, socket.AcceptClient(static_cast<TCPSocket*>(mainThreadConnection), NULL));
		}

		virtual void mainThreadPostRead(Connection* mainThreadConnection) override
		{
		}

		virtual void mainThreadPostWrite(Connection* mainThreadConnection) override
		{
		}

		virtual void mainThreadEnd(Connection* mainThreadConnection) override
		{
			delete mainThreadConnection;
			ASSERT_EQ(CONNECTION_RETURN_SUCCESS, socket.Close());
		}

		virtual Connection* secondaryThreadStart(Connection* mainThreadConnection) override
		{
			TCPSocket* secondaryThreadSocket = new TCPSocket();

			[&]()->void {ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadSocket->Connect("127.0.0.1", TCP_SOCKET_PORT, NULL));}();

			return secondaryThreadSocket;
		}

		virtual void secondaryThreadPostWrite(Connection* secondaryThreadConnection, Connection* mainThreadConnection) override
		{
		}

		virtual void secondaryThreadPostRead(Connection* secondaryThreadConnection, Connection* mainThreadConnection) override
		{
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
	TEST_F(TCPSocket_Test, InitialValues)
	{
		Socket_Test::InitialValues();
	}

	TEST_F(TCPSocket_Test, ManipulateUnopened)
	{
		Socket_Test::ManipulateUnopened();

		TCPSocket client;

		ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, socket.AcceptClient(&client, NULL));
		ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, socket.AcceptClientExtended(&client, 0));
	}

	TEST_F(TCPSocket_Test, CreateServer)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, socket.CreateServer(TCP_SOCKET_PORT));
		ASSERT_TRUE(socket.IsOpened());
	}

	TEST_F(TCPSocket_Test, ReadTimeout)
	{
		Socket_Test::ReadTimeout();
	}

	TEST_F(TCPSocket_Test, ReadCancel)
	{
		Socket_Test::ReadCancel();
	}

	TEST_F(TCPSocket_Test, ReadWriteArray)
	{
		Socket_Test::ReadWriteArray();
	}

	TEST_F(TCPSocket_Test, ReadWriteSingle)
	{
		Socket_Test::ReadWriteSingle();
	}

	TEST_F(TCPSocket_Test, ReadWriteString)
	{
		Socket_Test::ReadWriteString();
	}

	TEST_F(TCPSocket_Test, IsError)
	{
		Socket_Test::IsError();
	}
}