#include "Home.h"
#include <iostream>

using namespace std;
using namespace System;
using namespace System::Windows::Forms;


[STAThreadAttribute]

int main() {

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ClientServer::Home form;
	Application::Run(% form);

	return 0;
}

