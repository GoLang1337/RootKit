#include "memory.hpp"

namespace usermem
{
    DWORD GetProcessIdByName(LPCTSTR ProcessName)
    {
        PROCESSENTRY32 pt;
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pt.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hsnap, &pt)) {
            do {
                if (!lstrcmpi(pt.szExeFile, ProcessName)) {
                    CloseHandle(hsnap);
                    return pt.th32ProcessID;
                }
            } while (Process32Next(hsnap, &pt));
        }
        CloseHandle(hsnap);
        return 0;
    }

    template <typename T>
    T read(memory mem, const uint64_t address)
    {
        T buffer{ };
        mem.read_virtual_memory((PVOID)address, &buffer, sizeof(T));
        return buffer;
    }

    template<typename T>
    void write(memory mem, const uint64_t address, T buffer)
    {
        if (address > 0x7FFFFFFFFFFF || address < 1)
            return;

        mem.write_virtual_memory((PVOID)address, &buffer, sizeof(T));
    }
}

int main()
{
    DWORD ProcessId = usermem::GetProcessIdByName(L"System.exe"); if (!ProcessId) return 0;

    printf("[+] process_id: %i", ProcessId);

    memory mem(ProcessId);
    mem.init();

    return 0;
}