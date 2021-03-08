#ifndef WINDOWS_UPDATES
#define WINDOWS_UPDATES

#include <vector>
#include <Windows.h>
#include "RegistryKey.h"
#include "class_software.h"

using namespace std;

class InstalledUpdates {

private:

	static vector<Software>* GetInstalledUpdatesImp(bool IncludeUpdates);
	static vector<Software>* GetUninstallKeyUpdates(RegistryKey* Key, RegistryKey* ClassesKey, vector<Software>*, bool IncludeUpdates);
	static vector<Software>* GetUserInstallerKeyUpdates(RegistryKey* uInstallerKey, vector<Software>* ExistingProgramList);

public:

	InstalledUpdates(void);
	static vector<Software>* GetInstalledUpdates(bool IncludeUpdates);
};

#endif