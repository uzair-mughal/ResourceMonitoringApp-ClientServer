#ifndef INSTALLED_PROGRAMS
#define INSTALLED_PROGRAMS
#include <vector>
#include <Windows.h>
#include "RegistryKey.h"
#include "class_software.h"

using namespace std;

class InstalledPrograms {

private:
	static vector<Software>* GetInstalledProgramsImp(bool IncludeUpdates);
	static vector<Software>* GetUninstallKeyPrograms(RegistryKey* UninstallKey, RegistryKey* ClassesKey, vector<Software>*, bool IncludeUpdates);
	static vector<Software>* GetUserInstallerKeyPrograms(RegistryKey* uInstallerKey, vector<Software>* ExistingProgramList);

public:

	InstalledPrograms(void);
	static vector<Software>* GetInstalledPrograms(bool IncludeUpdates);

};

#endif