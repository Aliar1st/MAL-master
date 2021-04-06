#pragma once

#include <condition_variable>
#include <gtest/gtest.h>
#include <mutex>
#include <thread>
#include "IPC/Connection.h"

namespace IPC_Tests
{
	class Connection_Test : public ::testing::Test
	{
	private:

		bool mainThreadAsyncOpStarted;
		std::mutex mainThreadAsyncOpStartedMutex;
		std::condition_variable mainThreadAsyncOpStartedCondition;

		void readWriteSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*, ConnectionAsynchronousOperation*)> secondaryThreadWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*, ConnectionAsynchronousOperation*)> secondaryThreadRead, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd);

		void readTimeoutSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd);

		void readCancelSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd);

		void readWriteArraySecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd);

		void readWriteSingleSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd);

		void readWriteStringSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd);

		template<class _Fn>
		void readWrite(_Fn&& secondaryThreadFunction, const std::function<Connection* ()> mainThreadStart, const std::function<void(Connection*)> mainThreadPreRead, const std::function<void(Connection*, ConnectionAsynchronousOperation*)> mainThreadRead, const std::function<void(Connection*)> mainThreadPostRead, const std::function<void(Connection*, ConnectionAsynchronousOperation*)> mainThreadWrite, const std::function<void(Connection*)> mainThreadPostWrite, const std::function<void(Connection*)> mainThreadEnd, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd)
		{
			Connection* mainThreadConnection = mainThreadStart();
			ConnectionAsynchronousOperation* mainThreadAsyncOp = mainThreadConnection->CreateAsynchronousOperation();

			mainThreadAsyncOpStarted = false;

			std::thread thread(secondaryThreadFunction, this, mainThreadConnection, mainThreadAsyncOp, secondaryThreadStart, secondaryThreadPostWrite, secondaryThreadPostRead, secondaryThreadEnd);

			// Read
			mainThreadPreRead(mainThreadConnection);
			mainThreadRead(mainThreadConnection, mainThreadAsyncOp);
			mainThreadPostRead(mainThreadConnection);

			// Write
			mainThreadWrite(mainThreadConnection, mainThreadAsyncOp);
			mainThreadPostWrite(mainThreadConnection);

			thread.join();
			ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->Close());

			mainThreadEnd(mainThreadConnection);
			delete mainThreadAsyncOp;
		}

		void waitForAsyncOpStarted();

		void signalAsyncOpStarted();

	protected:

		virtual Connection* getConnection() = 0;

		virtual Connection* mainThreadStart() = 0;

		virtual void mainThreadPreRead(Connection* mainThreadConnection) = 0;

		virtual void mainThreadPostRead(Connection* mainThreadConnection) = 0;

		virtual void mainThreadPostWrite(Connection* mainThreadConnection) = 0;

		virtual void mainThreadEnd(Connection* mainThreadConnection) = 0;

		virtual Connection* secondaryThreadStart(Connection* mainThreadConnection) = 0;

		virtual void secondaryThreadPostWrite(Connection* secondaryThreadConnection, Connection* mainThreadConnection) = 0;

		virtual void secondaryThreadPostRead(Connection* secondaryThreadConnection, Connection* mainThreadConnection) = 0;

		virtual void secondaryThreadEnd(Connection* secondaryThreadConnection, Connection* mainThreadConnection) = 0;

	public:

		// #####
		// TESTS
		// #####
		void InitialValues();

		void ManipulateUnopened();

		void ReadTimeout();

		void ReadCancel();

		void ReadWriteArray();

		void ReadWriteSingle();

		void ReadWriteString();

		virtual void IsError();
	};
}