#include "Thread.h"

#include <cassert>

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <sys/prctl.h>
#endif // __linux__

Thread::Thread()
{
	running = false;
}

Thread::~Thread()
{
	assert(!IsJoinable()); // The thread is still running but should be joined by now. Use Join() in the subclass' destructor
}

void Thread::PrepareJoin()
{
	joining = true;

	prepareJoin();
}

void Thread::Join()
{
	// Only join if the thread is actually running
	if (internThread.joinable())
	{
		// Check if this thread should be prepared to join
		if (!joining)
		{
			PrepareJoin();
		}

		internThread.join();

		joined();
	}
}

void Thread::Start()
{
	running = true;
	joining = false;

	// Start the thread
	internThread = std::thread(&Thread::internalThreadRoutine, this);
}

bool Thread::IsRunning()
{
	return running;
}

bool Thread::IsJoinable()
{
	return internThread.joinable();
}

void Thread::prepareJoin()
{
}

void Thread::joined()
{
}

bool Thread::isJoining()
{
	return joining;
}

bool Thread::isRunningOnInternalThread()
{
	return std::this_thread::get_id() == internThread.get_id();
}

void Thread::setName(const char* threadName)
{
#ifdef _WIN32
	/**
	 * Declare the needed structure
	 */
	const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)

	struct THREADNAME_INFO
	{
		DWORD dwType; // Must be 0x1000.
		LPCSTR szName; // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags; // Reserved for future use, must be zero.
	};

#pragma pack(pop)

	// Prepare the thread struct
	THREADNAME_INFO info;

	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = GetThreadId(static_cast<HANDLE>(internThread.native_handle()));
	info.dwFlags = 0;

	// Set the name
#pragma warning(push)
#pragma warning(disable: 6320 6322)
	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*) &info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

#pragma warning(pop)
#elif __linux__
	prctl(PR_SET_NAME, threadName, 0, 0, 0);
#endif // __linux__
}

void Thread::internalThreadRoutine()
{
	// Set the thread's name
	setName(GetName());

	threadRoutine();

	running = false;
}