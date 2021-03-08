#pragma once
#include <windows.h>
#include <tlhelp32.h>

class Processes
{
public:

    PROCESSENTRY32 pe32;
    DWORD pid;
    WCHAR* pname;

    // Void Constructor with initialization of times at 0
    Processes() :mCpuUsage(-1), runCount(0) {
        pe32.dwSize = sizeof(PROCESSENTRY32);
        ZeroMemory(&prevSysUT, sizeof(FILETIME));
        ZeroMemory(&prevSysKT, sizeof(FILETIME));
        ZeroMemory(&prevProcUT, sizeof(FILETIME));
        ZeroMemory(&prevProcKT, sizeof(FILETIME));
    }

    Processes(PROCESSENTRY32 pe) :mCpuUsage(-1), runCount(0), pe32(pe) {
        pe32.dwSize = sizeof(PROCESSENTRY32);
        ZeroMemory(&prevSysUT, sizeof(FILETIME));
        ZeroMemory(&prevSysKT, sizeof(FILETIME));
        ZeroMemory(&prevProcUT, sizeof(FILETIME));
        ZeroMemory(&prevProcKT, sizeof(FILETIME));
    }

    inline void setPid() { pid = pe32.th32ProcessID; };
    inline void setPName() { pname = pe32.szExeFile; };
    inline BOOL operator==(Processes p) { return this->pid = p.pid; };

    // returns the percent of the CPU a process has used.
    // (-1 is returned in case of failure)
    short GetUsage() {

        short cpuCopy = mCpuUsage;

        HANDLE hProcess;

        FILETIME procCT, procET, procKT, procUT;
        FILETIME sysIT, sysKT, sysUT;

        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, pid);

        //cout << GetProcessTimes(hProcess, &procCT, &procET, &procKT, &procUT) << " " << GetSystemTimes(&sysIT, &sysKT, &sysUT) << "\t";

        if (!GetProcessTimes(hProcess, &procCT, &procET, &procKT, &procUT) ||
            !GetSystemTimes(&sysIT, &sysKT, &sysUT)) {

            runCount--;
            return cpuCopy;

        }

        // cout << runCount << " ";

        if (!IsFirstRun()) {

            ULONGLONG totalSys = Subtract(sysKT, prevSysKT) +
                Subtract(sysUT, prevSysUT);

            ULONGLONG totalProc = Subtract(procKT, prevProcKT) +
                Subtract(procUT, prevProcUT);

            if (totalSys > 0)
                mCpuUsage = (short)((100.0 * totalProc) / totalSys);
        }

        prevSysKT = sysKT;
        prevSysUT = sysUT;
        prevProcKT = procKT;
        prevProcUT = procUT;

        cpuCopy = mCpuUsage;

        runCount++;

        return mCpuUsage;
    }

private:

    // These variables are Process UserTime, KernelTime and 
    // System UserTime, KernelTime used for calculating cpu usage.
    FILETIME prevSysUT;
    FILETIME prevSysKT;
    FILETIME prevProcUT;
    FILETIME prevProcKT;

    short mCpuUsage;
    volatile LONG runCount;

    inline bool IsFirstRun() const { return !runCount; }

    ULONGLONG Subtract(const FILETIME& ftA, const FILETIME& ftB) {

        ULARGE_INTEGER a, b;
        a.LowPart = ftA.dwLowDateTime;
        a.HighPart = ftA.dwHighDateTime;

        b.LowPart = ftB.dwLowDateTime;
        b.HighPart = ftB.dwHighDateTime;

        return a.QuadPart - b.QuadPart;
    }
};

