#include "SharedMemory.h"

#include <climits>
#include "Utilities/ConversionUtil.h"
#include "Utilities/StringUtil.h"

#ifdef __linux__

#include <fcntl.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#endif // __linux__

SharedMemory::SharedMemory()
{
	reset();
}

SharedMemory::~SharedMemory()
{
	if (isOpened)
	{
		Close();
	}
}

SharedMemory_Return SharedMemory::Create(const char* name, size_t size)
{
	// Check if the shared memory is already opened
	if (isOpened)
	{
		return SHARED_MEMORY_RETURN_ERROR_ALREADY_OPENED;
	}

	assignSharedMemoryName(name);

#ifdef _WIN32
	// Create shared memory
	internSharedMemory = CreateFileMapping(
		INVALID_HANDLE_VALUE,                                   // Virtual file
		NULL,                                                   // Security attributes (default)
		PAGE_READWRITE,                                         // Read and write access
		static_cast<DWORD>(size >> sizeof(DWORD) * CHAR_BIT),   // Size (high-order DWORD)
		static_cast<DWORD>(size),                               // Size (low-order DWORD)
#ifdef UNICODE
		internName                                              // Name of the shared memory
#else
		name                                                    // Name of the shared memory
#endif // UNICODE
	);

	// Check if shared memory exists already
	bool alreadyExisted = GetLastError() == ERROR_ALREADY_EXISTS;

	// Check for errors
	if (internSharedMemory == NULL)
	{
		reset();

		return SHARED_MEMORY_RETURN_ERROR_COULD_NOT_CREATE_SHARED_MEMORY;
	}

	isOpened = true;
#elif __linux__
	// Create shared memory
	internSharedMemory = shm_open(
		name,                       // Name of the shared memory
		O_CREAT | O_EXCL | O_RDWR,  // Create shared memory
		0666                        // File mode
	);

	// Check if shared memory exists already
	bool alreadyExisted = errno == EEXIST;

	// Check if the shared memory exists already
	if (alreadyExisted)
	{
		internSharedMemory = shm_open(
			name,               // Name of the shared memory
			O_CREAT | O_RDWR,   // Create shared memory
			0666                // File mode
		);
	}

	// Check for errors
	if (internSharedMemory == -1)
	{
		reset();

		return SHARED_MEMORY_RETURN_ERROR_COULD_NOT_CREATE_SHARED_MEMORY;
	}

	isOpened = true;

	// Set size
	if (ftruncate(internSharedMemory, static_cast<off_t>(size)) == -1)
	{
		Destroy();

		return SHARED_MEMORY_RETURN_ERROR_COULD_NOT_CREATE_SHARED_MEMORY;
	}
#endif // __linux__

	// Map
	if (!mapSharedMemory(size))
	{
		Destroy();

		return SHARED_MEMORY_RETURN_ERROR_COULD_NOT_MAP_SHARED_MEMORY;
	}

	// No error occurred. Check if the semaphore_ existed before.
	if (alreadyExisted)
	{
		return SHARED_MEMORY_RETURN_SUCCESS_ALREADY_EXISTED;
	}

	return SHARED_MEMORY_RETURN_SUCCESS;
}

SharedMemory_Return SharedMemory::Open(const char* name, size_t size)
{
	// Check if the shared memory is already opened
	if (isOpened)
	{
		return SHARED_MEMORY_RETURN_ERROR_ALREADY_OPENED;
	}

	assignSharedMemoryName(name);

#ifdef _WIN32
	// Open shared memory
	internSharedMemory = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,    // Read and write access
		FALSE,                  // do not inherit the name
#ifdef UNICODE
		internName              // Name of the shared memory
#else
		name                    // Name of the shared memory
#endif // UNICODE
	);

	if (internSharedMemory == NULL)
	{
		reset();

		return SHARED_MEMORY_RETURN_ERROR_COULD_NOT_OPEN_SHARED_MEMORY;
	}
#elif __linux__
	// Open shared memory
	internSharedMemory = shm_open(
		name,       // Name of the shared memory
		O_RDWR,     // Open shared memory
		0666        // File mode
	);

	// Check for errors
	if (internSharedMemory == -1)
	{
		reset();

		return SHARED_MEMORY_RETURN_ERROR_COULD_NOT_CREATE_SHARED_MEMORY;
	}
#endif // __linux__

	isOpened = true;

	// Map
	if (!mapSharedMemory(size))
	{
		Close();

		return SHARED_MEMORY_RETURN_ERROR_COULD_NOT_MAP_SHARED_MEMORY;
	}

	// No error
	return SHARED_MEMORY_RETURN_SUCCESS;
}

SharedMemory_Return SharedMemory::Close()
{
	// Check if the shared memory is opened
	if (!isOpened)
	{
		return SHARED_MEMORY_RETURN_ERROR_NOT_OPENED;
	}

#ifdef _WIN32
	// Unmap and close
	UnmapViewOfFile(dataPointer);

	CloseHandle(internSharedMemory);
#endif // _WIN32

	reset();

	return SHARED_MEMORY_RETURN_SUCCESS;
}

SharedMemory_Return SharedMemory::Destroy()
{
	// Check if the shared memory is opened
	if (!isOpened)
	{
		return SHARED_MEMORY_RETURN_ERROR_NOT_OPENED;
	}

#ifdef __linux__
	shm_unlink(name);
#endif // __linux__

	return Close();
}

void SharedMemory::assignSharedMemoryName(const char* name)
{
	// Assign the name(s)
	this->name = StringUtil::Strcpy(name);

#if defined(_WIN32) && defined(UNICODE)
	internName = ConversionUtil::StrToWStr(name);
#endif // defined(_WIN32) && defined(UNICODE)
}

void* SharedMemory::GetDataPointer()
{
	return dataPointer;
}

bool SharedMemory::IsOpened()
{
	return isOpened;
}

char* SharedMemory::GetName()
{
	return name;
}

bool SharedMemory::IsError(SharedMemory_Return returnValue)
{
	return returnValue != SHARED_MEMORY_RETURN_SUCCESS && returnValue != SHARED_MEMORY_RETURN_SUCCESS_ALREADY_EXISTED;
}

bool SharedMemory::mapSharedMemory(size_t size)
{
#ifdef _WIN32
	// Map
	dataPointer = MapViewOfFile(
		internSharedMemory,     // Handle
		FILE_MAP_ALL_ACCESS,    // Read and write access
		0,                      // No offset
		0,                      // No offset
		size                    // Size
	);
#elif __linux__
	dataPointer = mmap(
		NULL,                   // Automatically select address
		size,                   // Size
		PROT_READ | PROT_WRITE, // Read and write access
		MAP_SHARED,             // Shared memory
		internSharedMemory,     // File descriptor
		0                       // No offset
	);
#endif // __linux__

	return dataPointer != nullptr;
}

void SharedMemory::reset()
{
	isOpened = false;
	dataPointer = nullptr;

#if defined(_WIN32) && defined(UNICODE)
	delete[] internName;
	internName = nullptr;
#endif // defined(_WIN32) && defined(UNICODE)
}