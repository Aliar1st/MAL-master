#include "Timer.h"

Timer::Timer()
{
	semaphore.Create(NULL, 0);
}

Timer::~Timer()
{
	Join();
}

const char* Timer::GetName()
{
	return "Timer";
}

void Timer::InitializeData(time_interval_t timerInterval, bool looping, std::function<void(bool timePassed, void* data)> delayedFunction, void* functionData)
{
	this->timerInterval = timerInterval;
	this->looping = looping;
	this->delayedFunction = delayedFunction;
	this->functionData = functionData;
}

void Timer::prepareJoin()
{
	semaphore.Up();
}

void Timer::threadRoutine()
{
	bool timePassed;

	// If required, loop until the timer is joining.
	do
	{
		// Wait for either a timeout or a cancel
		timePassed = (semaphore.DownExtended(timerInterval) == SEMAPHORE_RETURN_ERROR_OPERATION_TIMEOUT);

		// Execute the given function
		delayedFunction(timePassed, functionData);
	}
	while (looping && timePassed);
}