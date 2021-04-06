#pragma once

#include <gtest/gtest.h>

namespace Arguments_Tests
{
	class ArgumentParser_Test : public ::testing::Test
	{
	protected:

		size_t argc = 0;
		char** argv = nullptr;

		void setupArguments(std::vector<const char*> argumentVector);

	public:

		virtual void SetUp() override;

		virtual void TearDown() override;
	};
}