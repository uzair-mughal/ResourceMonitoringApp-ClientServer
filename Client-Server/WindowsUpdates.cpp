#include "WindowsUpdates.h"
#include <algorithm>
#include <regex>
#include<iostream>

static wstring GetInstallerKeyNameFromGuid(wstring GuidName);
static void AddToList(vector<Software>* TheList, Software software);

InstalledUpdates::InstalledUpdates(void) {}

vector<Software>* InstalledUpdates::GetInstalledUpdates(bool IncludeUpdates) {

	return GetInstalledUpdatesImp(IncludeUpdates);
}

vector<Software>* InstalledUpdates::GetInstalledUpdatesImp(bool IncludeUpdates) {

	vector<Software>* SoftwareList = new vector<Software>();

	RegistryKey* Key = RegistryKey::HKLM().OpenSubKey64(L"Software\\Microsoft\\Windows\\CurrentVersion\\Component Based Servicing\\Packages");

	vector<wstring> subkeys = Key->GetSubKeyNames();

	for (vector<wstring>::iterator it = subkeys.begin(); it != subkeys.end(); it++) {

		RegistryKey* temp = Key->OpenSubKey(*it, Key->KeyArch);
		wstring Visibility = temp->GetValue(L"Visibility");

		if (Visibility.compare(L"1") == 0) {

			wstring Name = temp->GetValue(L"InstallName");
			AddToList(SoftwareList, Software(Name, L"", L""));

		}
	}

	return SoftwareList;
}

// Util Functions

static void AddToList(vector<Software>* TheList, Software software) {

	int index = -1;
	for (int i = 0; i < TheList->size(); i++) {

		if (TheList->at(i).DisplayName.compare(software.DisplayName) == 0) {

			index = i;
			break;
		}
	}
	if (index == -1)
		TheList->push_back(software);
	else {

		Software duplicate = TheList->at(index);

		// Merge Version
		if (software.Version.compare(L"") != 0 && duplicate.Version.compare(L"") != 0 && software.Version.compare(duplicate.Version) != 0) {

			TheList->push_back(software);
			return;
		}
		else {

			if (software.Version.compare(L"") == 0)
				software.Version = duplicate.Version;
		}
		TheList->erase(TheList->begin() + index);
		TheList->push_back(software);
	}
}