#pragma once

#include <stddef.h>

#ifdef _WIN32

#include <Windows.h>
#include <conio.h>
#include <stdio.h>

#endif // _WIN32

/**
 * @brief	Enumerates all possible return values of the functions of SharedMemory.
 */
enum SharedMemory_Return
{
	SHARED_MEMORY_RETURN_SUCCESS,
	SHARED_MEMORY_RETURN_SUCCESS_ALREADY_EXISTED,
	SHARED_MEMORY_RETURN_ERROR_ALREADY_OPENED,
	SHARED_MEMORY_RETURN_ERROR_NOT_OPENED,
	SHARED_MEMORY_RETURN_ERROR_COULD_NOT_CREATE_SHARED_MEMORY,
	SHARED_MEMORY_RETURN_ERROR_COULD_NOT_OPEN_SHARED_MEMORY,
	SHARED_MEMORY_RETURN_ERROR_COULD_NOT_MAP_SHARED_MEMORY
};

/**
 * @brief	A Shared Memory class that supports Windows and Linux.
 */
class SharedMemory
{
private:

#ifdef _WIN32
#ifdef UNICODE
	WCHAR* internName = nullptr;
#endif // UNICODE

	HANDLE internSharedMemory;
#elif __linux__
	int internSharedMemory;
#endif // __linux__

	void* dataPointer;

	char* name = nullptr;
	bool isOpened;

	/**
	 * @brief	Assigns the shared memory's name according to its naming scheme.
	 *
	 * @param[in]	name	The desired name of the shared memory.
	 */
	void assignSharedMemoryName(const char* name);

	/**
	 * @brief	Maps the shared memory to the data pointer
	 *
	 * @return	True if the mapping is successfull; False otherwise
	 */
	bool mapSharedMemory(size_t size);

	/**
	 * @brief	Resets the Shared Memory to its initialization state.
	 */
	void reset();

public:

	/**
	 * @brief	The constructor of SharedMemory
	 */
	SharedMemory();

	/**
	 * @brief	The destructor of SharedMemory
	 */
	~SharedMemory();

	/**
	 * @brief	Creates a shared memory.
	 *
	 * @param[in]	name	The name of the shared memory.
	 * @param		size	The size of the shared memory.
	 *
	 * @return	SHARED_MEMORY_RETURN_SUCCESS								The operation was performed without any error. The shared memory didn't exist before and was created.
	 *			SHARED_MEMORY_RETURN_SUCCESS_ALREADY_EXISTED				The operation was performed without any error. The shared memory existed before and was opened.
	 *			SHARED_MEMORY_RETURN_ERROR_ALREADY_OPENED					The shared memory is already opened.
	 *			SHARED_MEMORY_RETURN_ERROR_COULD_NOT_CREATE_SHARED_MEMORY	Could not create the shared memory.
	 *			SHARED_MEMORY_RETURN_ERROR_COULD_NOT_MAP_SHARED_MEMORY		Could not map the shared memory
	 */
	SharedMemory_Return Create(const char* name, size_t size);

	/**
	 * @brief	Opens a shared memory.
	 *
	 * @param[in]	name	The name of the shared memory.
	 * @param		size	The size of the shared memory.
	 *
	 * @return	SHARED_MEMORY_RETURN_SUCCESS								The operation was performed without any error.
	 *			SHARED_MEMORY_RETURN_ERROR_ALREADY_OPENED					The shared memory is already opened.
	 *			SHARED_MEMORY_RETURN_ERROR_COULD_NOT_OPEN_SHARED_MEMORY		Could not open the shared memory.
	 *			SHARED_MEMORY_RETURN_ERROR_COULD_NOT_MAP_SHARED_MEMORY		Could not map the shared memory
	 */
	SharedMemory_Return Open(const char* name, size_t size);

	/**
	 * @brief	Closes the shared memory.
	 *
	 * @return	SHARED_MEMORY_RETURN_SUCCESS								The operation was performed without any error.
	 *			SHARED_MEMORY_RETURN_ERROR_NOT_OPENED						The shared memory is not opened but needs to be for this operation.
	 */
	SharedMemory_Return Close();

	/**
	 * @brief	Destroys the shared memory. This has to be called while the shared memory is still opened as it also closes the shared memory.
	 * 			On Linux it disallows other processes to create the shared memory as it destroys the kernel references.
	 * 			On Windows this function has the same effect as Close() but for portability it should still be used.
	 *
	 * @return	SHARED_MEMORY_RETURN_SUCCESS								The operation was performed without any error.
	 *			SHARED_MEMORY_RETURN_ERROR_NOT_OPENED						The shared memory is not opened but needs to be for this operation.
	 */
	SharedMemory_Return Destroy();

	/**
	 * @brief	Returns the data pointer of this shared memory object
	 *
	 * @return	Data pointer
	 */
	void* GetDataPointer();

	/**
	 * @brief	Returns whether this objects is opened.
	 *
	 * @return	True if this shared memory object is opened; False if not.
	 */
	bool IsOpened();

	/**
	 * @brief	Returns the name of the shared memory.
	 *
	 * @return	The name of the shared memory. NULL if the shared memory is not opened.
	 */
	char* GetName();

	/**
	 * @brief	Returns whether the given SharedMemory_Return value represents an error.
	 *
	 * @return	True if the value represents an error; False otherwise.
	 */
	static bool IsError(SharedMemory_Return returnValue);
};