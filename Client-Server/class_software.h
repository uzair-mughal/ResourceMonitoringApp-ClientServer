#ifndef CLASS_SOFTWARE
#define CLASS_SOFTWARE

#include <string>
#include <vector>
#include <Windows.h>
#include "RegistryKey.h"

class Software {

public:

	wstring DisplayName;
	wstring Version;
	wstring Date;
	Software(wstring, wstring, wstring);
};

#endif