#include "Semaphore.h"

#include <limits>
#include "Utilities/ConversionUtil.h"
#include "Utilities/StringUtil.h"

#ifdef __linux__

#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#endif // __linux__

Semaphore::Semaphore()
{
	reset();
}

Semaphore::~Semaphore()
{
	if (isOpened)
	{
		Close();
	}
}

Semaphore_Return Semaphore::Create(const char* name, long initialValue)
{
	// Check if the semaphore_ is already opened
	if (isOpened)
	{
		return SEMAPHORE_RETURN_ERROR_ALREADY_OPENED;
	}

	// Assign the semaphore_'s name
	assignSemaphoreName(name);

	// Create the semaphore_
#ifdef _WIN32
	internSemaphore = CreateSemaphore(
		NULL,                               // Default security
		initialValue,                       // Initial value of the semaphore_
		std::numeric_limits<LONG>::max(),   // Maximum value of the semaphore_
#ifdef UNICODE
		internName                          // Name of the semaphore_
#else
		name                                // Name of the semaphore_
#endif // UNICODE
	);

	// Store the potential error that CreateSemaphore(...) could have caused as this value is overridden by createStopHandle() later on
	int lastCreationError = GetLastError();

	// Check if the handle is valid
	if (internSemaphore == INVALID_HANDLE_VALUE)
	{
		reset();

		return SEMAPHORE_RETURN_ERROR_COULD_NOT_CREATE_SEMAPHORE;
	}

	isOpened = true;

	// Create the stop handle and check for errors
	if (!createStopHandle())
	{
		Destroy();

		return SEMAPHORE_RETURN_ERROR_COULD_NOT_CREATE_SEMAPHORE;
	}

	// No error occurred. Check if the semaphore_ existed before.
	if (lastCreationError == ERROR_ALREADY_EXISTS)
	{
		return SEMAPHORE_RETURN_SUCCESS_ALREADY_EXISTED;
	}

	return SEMAPHORE_RETURN_SUCCESS;
#elif __linux__
	bool success;

	if (name)
	{
		internSemaphore = sem_open(internName, O_CREAT | O_EXCL, DEFFILEMODE, initialValue);
		success = internSemaphore != SEM_FAILED;
	}
	else
	{
		internSemaphore = new sem_t();
		success = !sem_init(internSemaphore, 0, initialValue);
	}
	isOpened = true;

	// Check if there was an error
	if (success)
	{
		return SEMAPHORE_RETURN_SUCCESS;
	}

	// Check if the semaphore_ exists already
	if (errno == EEXIST)
	{
		internSemaphore = sem_open(internName, O_CREAT, DEFFILEMODE, initialValue);

		if (internSemaphore != SEM_FAILED)
		{
			return SEMAPHORE_RETURN_SUCCESS_ALREADY_EXISTED;
		}
	}

	// There was an error
	Destroy();

	return SEMAPHORE_RETURN_ERROR_COULD_NOT_CREATE_SEMAPHORE;
#endif // __linux__
}

Semaphore_Return Semaphore::Open(const char* name)
{
	// Check if the semaphore_ is already opened
	if (isOpened)
	{
		return SEMAPHORE_RETURN_ERROR_ALREADY_OPENED;
	}

	// Assign the semaphore_'s name
	assignSemaphoreName(name);

#ifdef _WIN32
	// Open the semaphore_
	internSemaphore = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,   // Modification rights
		TRUE,                   // Sub processes will inherit this semaphore_'s handle
#ifdef UNICODE
		internName              // Name of the semaphore_
#else
		name                    // Name of the semaphore_
#endif // UNICODE
	);

	// Check if the handle is valid
	if (!internSemaphore)
	{
		reset();

		// Check if the semaphore_ exists
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			return SEMAPHORE_RETURN_ERROR_NOT_FOUND;
		}

		// An unknown error occurred
		return SEMAPHORE_RETURN_ERROR_COULD_NOT_OPEN_SEMAPHORE;
	}

	isOpened = true;

	// Create the stop handle and check for errors
	if (!createStopHandle())
	{
		Close();

		return SEMAPHORE_RETURN_ERROR_COULD_NOT_OPEN_SEMAPHORE;
	}

	// No error occurred
	return SEMAPHORE_RETURN_SUCCESS;
#elif __linux__
	// Check for a name
	if (!name)
	{
		reset();

		return SEMAPHORE_RETURN_ERROR_COULD_NOT_OPEN_SEMAPHORE;
	}

	internSemaphore = sem_open(internName, 0);
	isOpened = true;

	// Check if there was an error
	if (internSemaphore != SEM_FAILED)
	{
		return SEMAPHORE_RETURN_SUCCESS;
	}

	// There was an error
	// Check if the semaphore_ exists
	if (errno == ENOENT)
	{
		Close();

		return SEMAPHORE_RETURN_ERROR_NOT_FOUND;
	}

	Close();

	return SEMAPHORE_RETURN_ERROR_COULD_NOT_OPEN_SEMAPHORE;
#endif // __linux__
}

Semaphore_Return Semaphore::Close()
{
	// Check if the semaphore_ is opened
	if (!isOpened)
	{
		return SEMAPHORE_RETURN_ERROR_NOT_OPENED;
	}

#ifdef _WIN32
	CloseHandle(internSemaphore);
	SetEvent(stopHandle);
#elif __linux__
	if (name)
	{
		sem_close(internSemaphore);
	}
	else
	{
		sem_destroy(internSemaphore);
		delete internSemaphore;
	}

	// Signal all blocked thread and wait for them to be unblocked
	{
		std::unique_lock<std::mutex> downThreadUnique(downThreadsMutex);

		while (downThreadsIDs.size() > 0)
		{
			pthread_t threadID = downThreadsIDs.front();

			pthread_kill(threadID, SIGUSR1);

			while (downThreadsIDs.front() == threadID)
			{
				downThreadsCondition.wait(downThreadUnique);
			}
		}
	}
#endif // __linux__

	reset();

	return SEMAPHORE_RETURN_SUCCESS;
}

Semaphore_Return Semaphore::Destroy()
{
	// Check if the semaphore_ is opened
	if (!isOpened)
	{
		return SEMAPHORE_RETURN_ERROR_NOT_OPENED;
	}

#ifdef __linux__
	if (name)
	{
		sem_unlink(internName);
	}
#endif // __linux__

	return Close();
}

Semaphore_Return Semaphore::Up()
{
	// Check if the semaphore_ is opened
	if (!isOpened)
	{
		return SEMAPHORE_RETURN_ERROR_NOT_OPENED;
	}

	// Increase the semaphore_
	bool success;

#ifdef _WIN32
	success = ReleaseSemaphore(
		internSemaphore,    // Semaphore handle
		1,                  // Increase the semaphore_ by 1
		NULL                // Do not store the previous value
	);
#elif __linux__
	success = !sem_post(internSemaphore);
#endif // __linux__

	// Check for errors
	if (!success)
	{
		return SEMAPHORE_RETURN_ERROR_COULD_NOT_INCREASE_SEMAPHORE;
	}

	// No error occurred
	return SEMAPHORE_RETURN_SUCCESS;
}

Semaphore_Return Semaphore::Down()
{
	return DownExtended(NO_TIMEOUT);
}

Semaphore_Return Semaphore::DownExtended(time_interval_t timeoutInterval)
{
	// Check if the semaphore_ is opened
	if (!isOpened)
	{
		return SEMAPHORE_RETURN_ERROR_NOT_OPENED;
	}

	// Decrease the semaphore_
#ifdef _WIN32
	HANDLE handles[2] = {internSemaphore, stopHandle};
	DWORD timeToWait = timeoutInterval == NO_TIMEOUT ? INFINITE : timeoutInterval;

	BOOL success = WaitForMultipleObjects(
		2,          // Amount of handles to wait for
		handles,    // Handles to wait for
		FALSE,      // Do not wait for all handles to be set
		timeToWait  // Wait until either the semaphore_ could be decreased or the interval elapsed.
	);

	// Check for errors
	if (success == WAIT_FAILED)
	{
		return SEMAPHORE_RETURN_ERROR_COULD_NOT_DECREASE_SEMAPHORE;
	}

	// Check if the operation was aborted
	if (success == WAIT_OBJECT_0 + 1)
	{
		CloseHandle(stopHandle);

		return SEMAPHORE_RETURN_ERROR_OPERATION_ABORTED;
	}

	// Check if the timeout interval has elapsed
	if (success == WAIT_TIMEOUT)
	{
		return SEMAPHORE_RETURN_ERROR_OPERATION_TIMEOUT;
	}
#elif __linux__
	bool success;

	// Setup the signal
	struct sigaction action;
	struct sigaction prevAction;

	action.sa_handler = [](int signum)
	{
	};
	action.sa_flags = 0;
	sigaction(SIGUSR1, &action, &prevAction);

	// Mark this thread as blocked
	downThreadsMutex.lock();
	downThreadsIDs.push_back(pthread_self());
	downThreadsMutex.unlock();

	// Wait
	if (timeoutInterval != NO_TIMEOUT)
	{
		/**
		 * Setup a timeout
		 */

		// Get the current time
		struct timespec absTimeout;

		if (clock_gettime(CLOCK_REALTIME, &absTimeout))
		{
			return SEMAPHORE_RETURN_ERROR_COULD_NOT_DECREASE_SEMAPHORE;
		}

		time_t timeoutIntervalSeconds = timeoutInterval / 1000;
		time_t timeoutIntervalNanoseconds = (timeoutInterval % 1000) * 1000000;

		// Add the interval and normalize the time structure
		absTimeout.tv_nsec += timeoutIntervalNanoseconds;
		absTimeout.tv_sec += timeoutIntervalSeconds + absTimeout.tv_nsec / 1000000000;
		absTimeout.tv_nsec %= 1000000000;

		success = !sem_timedwait(internSemaphore, &absTimeout);
	}
	else
	{
		success = !sem_wait(internSemaphore);
	}

	// Reset the signal
	sigaction(SIGUSR1, &prevAction, NULL);

	// Unmark this thread as blocked
	downThreadsMutex.lock();
	downThreadsIDs.remove(pthread_self());
	downThreadsCondition.notify_all();
	downThreadsMutex.unlock();

	// Check for errors
	if (!success)
	{
		// Check for a timeout
		if (errno == ETIMEDOUT)
		{
			return SEMAPHORE_RETURN_ERROR_OPERATION_TIMEOUT;
		}

		// Check if the operation was aborted
		if (errno == EINTR)
		{
			return SEMAPHORE_RETURN_ERROR_OPERATION_ABORTED;
		}

		return SEMAPHORE_RETURN_ERROR_COULD_NOT_DECREASE_SEMAPHORE;
	}
#endif // __linux__

	// No error occurred
	return SEMAPHORE_RETURN_SUCCESS;
}

bool Semaphore::IsOpened()
{
	return isOpened;
}

bool Semaphore::IsError(Semaphore_Return returnValue)
{
	return returnValue != SEMAPHORE_RETURN_SUCCESS && returnValue != SEMAPHORE_RETURN_SUCCESS_ALREADY_EXISTED;
}

void Semaphore::assignSemaphoreName(const char* name)
{
	// Check if this semaphore_ should be unnamed
	if (!name)
	{
		this->name = 0;
	}
	else
	{
		// Assign the name(s)
		this->name = StringUtil::Strcpy(name);

#ifdef _WIN32
#ifdef UNICODE
		internName = ConversionUtil::StrToWStr(name);
#endif // UNICODE
#elif __linux__
		internName = StringUtil::Strcat(SEMAPHORE_NAME_PREFIX, name);
#endif // __linux__
	}
}

#ifdef _WIN32

bool Semaphore::createStopHandle()
{
	stopHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

	return stopHandle;
}

#endif // _WIN32

void Semaphore::reset()
{
	isOpened = false;

	// Free the memory allocated for the name(s)
	delete[] name;
	name = 0;

#if defined(__linux__) || defined(UNICODE)
	delete[] internName;
	internName = 0;
#endif // defined(__linux__) || defined(UNICODE)
}