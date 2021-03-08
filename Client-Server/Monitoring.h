#pragma once
#include "Processes.h"
#include <vector>
#include <psapi.h>
#include <iterator>
#include <iostream>

using namespace std;

class Monitoring
{
public:
    // Vector pointing to vector containing processes
    vector<Processes> procs;
    vector<Processes>::iterator it;

    // It contains all memory usages for a particular process
    // Call getMemInfo for each process id.
    PROCESS_MEMORY_COUNTERS pmc;

    //BOOL GetProcessList();
    //Monitoring(vector<Processes>* procs) {
    //	this->procs = procs;
    //}

    Monitoring() {

    }

    void removeProcs(int i, int j) {
        // Remove processes from vector processes
        while (--j + 1) {
            it = procs.begin() + i + j;
            procs.erase(it);
        }
    }

    void setProc(PROCESSENTRY32 pe) {
        Processes proc(pe);
        proc.setPName();
        proc.setPid();
        procs.push_back(proc);
    }

    BOOL FillProcesses() {

        HANDLE hProcessSnap;
        PROCESSENTRY32 pe32;
        int i = 0, j;
        int total = 0;

        hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (hProcessSnap == INVALID_HANDLE_VALUE)
            return FALSE;

        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (!Process32First(hProcessSnap, &pe32)) {
            CloseHandle(hProcessSnap);
            return FALSE;
        }

        do {

            j = 0;

            if (i < procs.size()) {

                if (pe32.th32ProcessID != procs.at(i).pid) {
                    cout << "In";

                    while (j >= 0) {

                        if ((checkOutOfBounds(&procs.at(i + j)) && pe32.th32ProcessID == procs.at(i + j).pid) ||
                            (pe32.th32ProcessID == procs.at(i + j).pid)) {

                            removeProcs(i, j);
                            break;

                        }
                        else {

                            removeProcs(i, j + 1);
                            setProc(pe32);
                            break;

                        }

                        j++;
                    }
                }

            }
            else
                setProc(pe32);

            int usage = procs.at(i + j).GetUsage();
            //cout << i << "\t" << procs.at(i).pid << "\t-\t" << procs.at(i).pname << "\t" << usage << "%\t";
            //cout << endl;

            if (usage > 0) total = total + usage;
            i++;

        } while (Process32Next(hProcessSnap, &pe32));

        while (i < procs.size() - 1)
            procs.pop_back();

        cout << "Total Cpu Usage: " << total << endl;

        CloseHandle(hProcessSnap);
        return TRUE;
    }

    void GetMemInfo(DWORD pid) {

        HANDLE hprocess;
        pmc = {};

        hprocess = OpenProcess(PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, pid);

        if (!hprocess)
            return;

        if (GetProcessMemoryInfo(hprocess, &pmc, sizeof(pmc))) {
            CloseHandle(hprocess);
            return;
        }

        CloseHandle(hprocess);
    }

    inline BOOL checkOutOfBounds(Processes* p) { if (p != &procs.back())  return true;    return false; }

};