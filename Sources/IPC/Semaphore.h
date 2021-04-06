#pragma once

#include <limits>

#ifdef _WIN32

#include <Windows.h>

typedef DWORD time_interval_t;

#elif __linux__

#include <condition_variable>
#include <list>
#include <mutex>
#include <semaphore.h>

typedef unsigned long time_interval_t;

#endif // __linux__

/**
 * @brief	Enumerates all possible return values of the functions of Semaphore.
 */
enum Semaphore_Return
{
	SEMAPHORE_RETURN_SUCCESS,
	SEMAPHORE_RETURN_SUCCESS_ALREADY_EXISTED,
	SEMAPHORE_RETURN_ERROR_ALREADY_OPENED,
	SEMAPHORE_RETURN_ERROR_NOT_OPENED,
	SEMAPHORE_RETURN_ERROR_COULD_NOT_CREATE_SEMAPHORE,
	SEMAPHORE_RETURN_ERROR_COULD_NOT_OPEN_SEMAPHORE,
	SEMAPHORE_RETURN_ERROR_NOT_FOUND,
	SEMAPHORE_RETURN_ERROR_COULD_NOT_INCREASE_SEMAPHORE,
	SEMAPHORE_RETURN_ERROR_COULD_NOT_DECREASE_SEMAPHORE,
	SEMAPHORE_RETURN_ERROR_OPERATION_ABORTED,
	SEMAPHORE_RETURN_ERROR_OPERATION_TIMEOUT
};

/**
 * @brief	A semaphore class that supports Windows and Linux.
 */
class Semaphore
{
private:

#ifdef __linux__
	constexpr static const char SEMAPHORE_NAME_PREFIX[] = "/";
#endif // __linux__

#ifdef _WIN32
#ifdef UNICODE
	WCHAR* internName = nullptr;
#endif // UNICODE

	HANDLE internSemaphore;
	HANDLE stopHandle;
#elif __linux__
	char* internName = nullptr;

	sem_t* internSemaphore;

	std::mutex downThreadsMutex;
	std::condition_variable downThreadsCondition;
	std::list<pthread_t> downThreadsIDs;
#endif

	char* name = nullptr;
	bool isOpened;

	/**
	 * @brief	Assigns the semaphore's names.
	 *
	 * @param[in]	name	The desired name of the semaphore.
	 */
	void assignSemaphoreName(const char* name);

#ifdef _WIN32
	/**
	 * @brief	Creates and assigns the stop handle.
	 *
	 * @return	True if the stop handle could be created; False otherwise
	 */
	bool createStopHandle();
#endif

	/**
	 * @brief	Resets the semaphore to its initialization state.
	 */
	void reset();

public:

	constexpr static const time_interval_t NO_TIMEOUT = std::numeric_limits<time_interval_t>::max();

	/**
	 * @brief	The constructor of Semaphore
	 */
	Semaphore();

	/**
	 * @brief	The destructor of Semaphore
	 */
	~Semaphore();

	/**
	 * @brief	Creates or opens a (new) semaphore. The semaphore has no maximum value. The semaphore is closed upon closing the semaphore object that created it.
	 * 			Any other process holding the same semaphore may continue using it but new process can not access the semaphore anymore.
	 *
	 * @param[in]	name			The name of the semaphore. If NULL, a nameless semaphore is created.
	 * @param 		initialValue	The initial value of the semaphore. Only used if the semaphore was created.
	 *
	 * @return	SEMAPHORE_RETURN_SUCCESS								The operation was performed without any error. The semaphore didn't exist before and was created.
	 *			SEMAPHORE_RETURN_SUCCESS_ALREADY_EXISTED				The operation was performed without any error. The semaphore existed before and was opened.
	 *			SEMAPHORE_RETURN_ERROR_ALREADY_OPENED					The semaphore is already opened.
	 *			SEMAPHORE_RETURN_ERROR_COULD_NOT_CREATE_SEMAPHORE		Could not create the semaphore.
	 */
	Semaphore_Return Create(const char* name, long initialValue);

	/**
	 * @brief	Opens a named semaphore with the given name.
	 *
	 * @param[in]	name	The name of the semaphore.
	 *
	 * @return	SEMAPHORE_RETURN_SUCCESS								The operation was performed without any error.
	 *			SEMAPHORE_RETURN_ERROR_ALREADY_OPENED					The semaphore is already opened.
	 *			SEMAPHORE_RETURN_ERROR_NOT_FOUND						Could not open a semaphore. No semaphore exists with the given name.
	 *			SEMAPHORE_RETURN_ERROR_COULD_NOT_OPEN_SEMAPHORE			Could not open a semaphore.
	 */
	Semaphore_Return Open(const char* name);

	/**
	 * @brief	Closes the semaphore. Also aborts all Down() and DownExtended(...) calls.
	 *
	 * @return	SEMAPHORE_RETURN_SUCCESS								The operation was performed without any error.
	 *			SEMAPHORE_RETURN_ERROR_NOT_OPENED						The semaphore is not opened but needs to be for this operation.
	 */
	Semaphore_Return Close();

	/**
	 * @brief	Destroys the semaphore. This has to be called while the semaphore is still opened as it also closes the semaphore.
	 *			Only named semaphores need to be destroyed. For unnamed semaphores it is sufficient to call Close() or just destroy them
	 *			(The destructor calls Close()).
	 * 			On Linux it disallows other processes to create the semaphore as it destroys the kernel references.
	 * 			On Windows this function has the same effect as Close() but for portability it should still be used.
	 *
	 * @return	SEMAPHORE_RETURN_SUCCESS								The operation was performed without any error.
	 *			SEMAPHORE_RETURN_ERROR_NOT_OPENED						The semaphore is not opened but needs to be for this operation.
	 */
	Semaphore_Return Destroy();

	/**
	 * @brief	Increases the semaphore.
	 *
	 * @return	SEMAPHORE_RETURN_SUCCESS								The operation was performed without any error.
	 *			SEMAPHORE_RETURN_ERROR_NOT_OPENED						The semaphore is not opened but needs to be for this operation.
	 *			SEMAPHORE_RETURN_ERROR_COULD_NOT_INCREASE_SEMAPHORE		Could not increase the semaphore.
	 */
	Semaphore_Return Up();

	/**
	 * @brief	Decreases the semaphore. If the semaphore's value is 0, block until the value is
	 * 			increased by another source.
	 *
	 * @return	SEMAPHORE_RETURN_SUCCESS								The operation was performed without any error.
	 *			SEMAPHORE_RETURN_ERROR_NOT_OPENED						The semaphore is not opened but needs to be for this operation.
	 *			SEMAPHORE_RETURN_ERROR_COULD_NOT_DECREASE_SEMAPHORE		Could not decrease the semaphore.
	 *			SEMAPHORE_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 */
	Semaphore_Return Down();

	/**
	 * @brief	Decreases the semaphore. If the semaphore's value is 0, block until the value is
	 * 			increased by another source or until the timeout interval has elapsed.
	 *
	 * @param	timeoutInterval	The timeout interval in milliseconds. If this is 0, this call returns immediately. If Semaphore::NO_TIMEOUT, this call blocks until the semaphore could be decreased.
	 *
	 * @return	SEMAPHORE_RETURN_SUCCESS								The operation was performed without any error.
	 *			SEMAPHORE_RETURN_ERROR_NOT_OPENED						The semaphore is not opened but needs to be for this operation.
	 *			SEMAPHORE_RETURN_ERROR_COULD_NOT_DECREASE_SEMAPHORE		Could not decrease the semaphore.
	 *			SEMAPHORE_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			SEMAPHORE_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed. if timeoutInterval is 0, this indicates that the semaphore's value is 0.
	 */
	Semaphore_Return DownExtended(time_interval_t timeoutInterval);

	/**
	 * @brief	Returns whether this objects is opened.
	 *
	 * @return	True if this semaphore object is opened; False if not.
	 */
	bool IsOpened();

	/**
	 * @brief	Returns whether the given Semaphore_Return value represents an error.
	 *
	 * @return	True if the value represents an error; False otherwise.
	 */
	static bool IsError(Semaphore_Return returnValue);
};