#pragma once

#include <functional>
#include "IPC/Semaphore.h"
#include "Threading/Thread.h"

/**
 * @brief	A class to execute a function on another thread after a given time interval.
 */
class Timer : public Thread
{
private:

	Semaphore semaphore;

	time_interval_t timerInterval;
	bool looping;
	std::function<void(bool timePassed, void* data)> delayedFunction;
	void* functionData;

protected:

	virtual void prepareJoin() override;

	virtual void threadRoutine() override;

public:

	/**
	 * @brief	The constructor of Timer.
	 */
	Timer();

	/**
	 * @brief	The destructor of Timer.
	 */
	virtual ~Timer();

	virtual const char* GetName() override;

	/**
	 * @brief	Initializes all data for this thread.
	 *
	 * @param		timerInterval		The time in milliseconds after which the function shall be called
	 * @param		looping				If true, periodically calls the delayedFunction until the timer is canceled. Otherwise, the function is called only once.
	 * @param		delayedFunction		The function to call after the time has passed. It is only called if the Timer is not canceled. Its bool parameter is true if the time has passed and false if the timer was canceled.
	 * @param[in]	functionData		A data pointer that is passed to the delayed function.
	 */
	void InitializeData(time_interval_t timerInterval, bool looping, std::function<void(bool timePassed, void* data)> delayedFunction, void* functionData);
};