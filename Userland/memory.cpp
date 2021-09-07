#include "memory.hpp"

void memory::init()
{
	this->DriverHandle = CreateFile(L"\\\\.\\\RootKit", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (DriverHandle == INVALID_HANDLE_VALUE)
		this->DriverHandle = nullptr;
}

void memory::read_virtual_memory(PVOID Base, PVOID Buffer, DWORD Size)
{
	_copy_memory req = { 0 };

	req.address = (ULONGLONG)Base;
	req.buffer = (ULONGLONG)Buffer;
	req.size = Size;
	
	req.pid = ProcessId;
	req.write = FALSE;

	DeviceIoControl(DriverHandle, io_copy_memory, &req, sizeof(req), nullptr, 0, NULL, NULL);
}

void memory::write_virtual_memory(PVOID Base, PVOID Buffer, DWORD Size)
{
	_copy_memory req = { 0 };

	req.address = (ULONGLONG)Base;
	req.buffer = (ULONGLONG)Buffer;
	req.size = Size;

	req.pid = ProcessId;
	req.write = TRUE;

	DeviceIoControl(DriverHandle, io_copy_memory, &req, sizeof(req), nullptr, 0, NULL, NULL);
}

void memory::protect_virtual_memory(PVOID Base, DWORD Size, DWORD Protection)
{
	_protect_memory req = { 0 };

	req.address = (ULONGLONG)Base;
	req.size = Size;
	req.new_protect = Protection;

	req.pid = ProcessId;

	DeviceIoControl(DriverHandle, io_protect_memory, &req, sizeof(req), nullptr, 0, NULL, NULL);
}

PVOID memory::allocate_virtual_memory(DWORD Size, DWORD Protection)
{
	_allocate_memory req = { 0 };

	req.size = Size;
	req.protect = Protection;

	req.pid = ProcessId;

	DeviceIoControl(DriverHandle, io_allocate_memory, &req, sizeof(req), &req, sizeof(req), NULL, NULL);

	return (PVOID)req.address;
}

void memory::free_virtual_memory(PVOID Address)
{
	_free_memory req = { 0 };

	req.address = (ULONGLONG)Address;
	req.pid = ProcessId;

	DeviceIoControl(DriverHandle, io_free_memory, &req, sizeof(req), nullptr, 0, NULL, NULL);
}