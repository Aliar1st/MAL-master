#pragma once

#include "Connection_Test.h"

namespace IPC_Tests
{
	class Socket_Test : public Connection_Test
	{
	public:

		// #####
		// TESTS
		// #####
		virtual void IsError() override;
	};
}