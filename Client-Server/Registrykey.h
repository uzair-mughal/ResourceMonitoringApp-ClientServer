#ifndef REGISTRY_KEY
#define REGISTRY_KEY

#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

enum Arch { //constants

	UnKnown = 0, X86 = 1, X64 = 2
};

class RegistryKey {

private:

	HKEY hkey; //Handle to Registry Key
	RegistryKey(HKEY, Arch); // The constructor is private, not explicit initilization is available.

public:

	Arch KeyArch; // Indicate whether the Key is a 64 bit or 32 bit Key.

	//functions
	RegistryKey* OpenSubKey64(wstring subkey); //subkey to Access the Windows Registry
	RegistryKey* OpenSubKey32(wstring subkey);
	RegistryKey* OpenSubKey(wstring subkey);
	RegistryKey* OpenSubKey(wstring subkey, Arch a);

	vector<wstring> GetSubKeyNames();
	wstring GetValue(wstring query);

	static RegistryKey& HKLM();
	static RegistryKey& HKU();
};

#endif