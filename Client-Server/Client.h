#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>

// Headers for sockets
#include <WS2tcpip.h>
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#include<fstream>

// Headers for processes
#include "Monitoring.h"

// Headers for registry
#include "InstalledPrograms.h"
#include "WindowsUpdates.h"
#include "Registrykey.h"
#include "ServerConnection.h"
#include <msclr\marshal_cppstd.h>

namespace ClientServer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	// Monitoring Obj to generate processes
	static Monitoring mon;
	// Totals
	static DWORD tCpu;
	static SIZE_T tMem;
	static SIZE_T tSwap;


	/// <summary>
	/// Summary for Client1
	/// </summary>
	public ref class Client1 : public System::Windows::Forms::Form
	{
	public:
	private: System::Windows::Forms::TabPage^ tabPage3;
	public:
	private: System::Windows::Forms::DataGridView^ dataGridView4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ windows_updates;
	private: System::Windows::Forms::DataGridView^ dataGridView3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ pcakages;
	private: System::Windows::Forms::TabPage^ tabPage2;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Label^ product_name;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ sys_name;
	private: System::Windows::Forms::Label^ build_number;
	private: System::Windows::Forms::Label^ os_version;
	private: System::Windows::Forms::Label^ ip_address;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::DataGridView^ dataGridView2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ applicatinons;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ install_date;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ version;
	private: System::Windows::Forms::TabControl^ Client_tab;
	private: System::Windows::Forms::TabPage^ live_usage;
	private: System::Windows::Forms::DataGridView^ dataGridView1;
	private: System::Windows::Forms::Button^ StartStopBut;
	private: System::Windows::Forms::Timer^ timer2;
	private: System::Windows::Forms::Label^ CpuLabel;
	private: System::Windows::Forms::Label^ swapLabel;
	private: System::Windows::Forms::Label^ MemLabel;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ pid;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ processes;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ cpu_usage;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ memory_usage;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ swap_usage;









	public:

	Form^ obj;

		Client1(Form^ obj1)
		{
			//
			//TODO: Add the constructor code here
			//
			obj = obj1;
			InitializeComponent();
			loadApps();
			loadPackages();
			loadSpecs();
			
		}

		void createUsageFile() {
			
			RegistryKey* Key = RegistryKey::HKLM().OpenSubKey64(L"SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName");
			wstring PCName = Key->GetValue(L"ComputerName");
			ofstream file;
			string filename(PCName.begin(), PCName.end());
			filename.append("_Usage.in");
			file.open(filename.c_str());
			
			int rows = dataGridView1->Rows->Count - 1;

			file << msclr::interop::marshal_as<std::string>(CpuLabel->Text->ToString()) << "\n";
			file << msclr::interop::marshal_as<std::string>(MemLabel->Text->ToString()) << "\n";
			file << msclr::interop::marshal_as<std::string>(swapLabel->Text->ToString()) << "\n";

			for (int i = 0; i < rows; i++) {
				
				for (int j = 0; j < 5; j++)
					file << msclr::interop::marshal_as<std::string>(dataGridView1->Rows[i]->Cells[j]->Value->ToString()) << "\t";
					
				file << "\n";
			}

			file.close();
		}

		void loadProcs() {

			//creates a static variable to convert Bytes to Megabytes
			const int MB = 1048576;
			int i = 0;
			short usage = 0;
			
			dataGridView1->Rows->Clear();
			dataGridView1->Rows->Add(mon.procs.size());

			for (vector<Processes>::iterator iter = mon.procs.begin(); iter != mon.procs.end(); iter++) {
				// Fill pmc struct with memory info
				mon.GetMemInfo(iter->pid);
				
				// Set Cpu Usage to usage var and calculate total cpu, memory and swap usage
				usage = iter->GetUsage();
				if (usage < 0) usage = 0;	else	tCpu += usage;
				
				tMem += mon.pmc.WorkingSetSize;
				tSwap += mon.pmc.PagefileUsage;

				dataGridView1->Rows[i]->Cells[0]->Value = iter->pid;
				dataGridView1->Rows[i]->Cells[1]->Value = gcnew System::String(iter->pe32.szExeFile);
				dataGridView1->Rows[i]->Cells[2]->Value = usage;
				dataGridView1->Rows[i]->Cells[3]->Value = mon.pmc.WorkingSetSize / MB;
				dataGridView1->Rows[i]->Cells[4]->Value = mon.pmc.PagefileUsage / MB;
				i++;
			}

			// Setting Totals to labels
			this->CpuLabel->Text = gcnew String(tCpu.ToString() + "%");
			this->MemLabel->Text = gcnew String((tMem/MB).ToString() + " MBs");
			this->swapLabel->Text = gcnew String((tSwap/MB).ToString() + " MBs");

		}

		void loadApps() {

			RegistryKey* Key = RegistryKey::HKLM().OpenSubKey64(L"SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName");
			wstring PCName = Key->GetValue(L"ComputerName");
			ofstream file;
			string filename(PCName.begin(), PCName.end());
			filename.append("_Apps.in");
			file.open(filename.c_str());

			string text;

			int i = 0;
			vector<Software>* list = InstalledPrograms::GetInstalledPrograms(false);
			dataGridView2->Rows->Clear();
			dataGridView2->Rows->Add(list->size());

			for (vector<Software>::iterator iter = list->begin(); iter != list->end(); iter++) {

				// Name
				String^ s = gcnew String(iter->DisplayName.c_str());
				text = msclr::interop::marshal_as<std::string>(s);
				file << text << "\t";
				dataGridView2->Rows[i]->Cells[0]->Value = s;
				
				// Date
				s = gcnew String(iter->Date.c_str());
				if (s == "")	s = "-";
				text = msclr::interop::marshal_as<std::string>(s);
				file << text << "\t";
				dataGridView2->Rows[i]->Cells[1]->Value = s;
				
				// Version
				s = gcnew String(iter->Version.c_str());
				if (s == "")	s = "-";
				text = msclr::interop::marshal_as<std::string>(s);
				file << text << endl;
				dataGridView2->Rows[i]->Cells[2]->Value = s;

				i++;
			}

			file.close();
		}

		void loadPackages() {

			RegistryKey* Key = RegistryKey::HKLM().OpenSubKey64(L"SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName");
			wstring PCName = Key->GetValue(L"ComputerName");
			ofstream file,file1;
			string filename(PCName.begin(), PCName.end());
			filename.append("_Features.in");
			file.open(filename.c_str());
			string filename1(PCName.begin(), PCName.end());
			filename1.append("_Updates.in");
			file1.open(filename1.c_str());

			int i = 0, j = 0;
			vector<Software>* list = InstalledUpdates::GetInstalledUpdates(false);
			dataGridView3->Rows->Clear();
			dataGridView3->Rows->Add(list->size());

			dataGridView4->Rows->Clear();
			dataGridView4->Rows->Add(list->size());

			for (vector<Software>::iterator iter = list->begin(); iter != list->end(); iter++) {
				
				String^ s = gcnew String(iter->DisplayName.c_str());
				string text= msclr::interop::marshal_as<std::string>(s);

				if (s[12].ToString() == "K") {

					dataGridView4->Rows[i]->Cells[0]->Value = s;
					i++;
					file1 << text << endl;
				}
				else {

					dataGridView3->Rows[j]->Cells[0]->Value = s;
					j++;
					file << text << endl;
				}
			}

			file.close();
		}

		void loadSpecs() {

			build_number->Text ="";
			os_version->Text = "";
			product_name->Text = "";
			sys_name->Text = "";
			ip_address->Text = "";

			ofstream file;
		
			RegistryKey* Key = RegistryKey::HKLM().OpenSubKey64(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
			wstring BuildNumber = Key->GetValue(L"CurrentBuildNumber");
			wstring Version = Key->GetValue(L"CurrentVersion");
			wstring Product = Key->GetValue(L"ProductName");

			String ^ s = gcnew String(BuildNumber.c_str());
			build_number->Text = s;
			s= gcnew String(Version.c_str());
			os_version->Text = s;
			s = gcnew String(Product.c_str());
			product_name->Text = s;

			string bnum(BuildNumber.begin(), BuildNumber.end());
			string ver(Version.begin(), Version.end());
			string prod(Product.begin(), Product.end());

			Key = RegistryKey::HKLM().OpenSubKey64(L"SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName");
			wstring PCName = Key->GetValue(L"ComputerName");
			s = gcnew String(PCName.c_str());
			sys_name->Text = s;

			string filename(PCName.begin(), PCName.end());
			filename.append("_Specs.in");
			file.open(filename.c_str());


			Key = RegistryKey::HKLM().OpenSubKey64(L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces");
			
			vector<wstring> subkeys = Key->GetSubKeyNames();


			string IP;

			for (vector<wstring>::iterator it = subkeys.begin(); it != subkeys.end(); it++) {

				RegistryKey* temp = Key->OpenSubKey(*it, Key->KeyArch);
				wstring IPAddress = temp->GetValue(L"DhcpIPAddress");

				if (IPAddress.compare(L"") != 0) {
					IP = "";
					IP.append(IPAddress.begin(), IPAddress.end());


				}
			}

			ip_address->Text = gcnew String(IP.c_str());

			file << IP << endl;

			/*for (vector<wstring>::iterator it = subkeys.begin(); it != subkeys.end(); it++) {

				RegistryKey* temp = Key->OpenSubKey(*it, Key->KeyArch);
				wstring IPAddress = temp->GetValue(L"DhcpIPAddress");

				if (IPAddress.compare(L"") != 0) {

					string IP(IPAddress.begin(), IPAddress.end());
					s = gcnew String(IPAddress.c_str());
					ip_address->Text = s;
					file << IP << endl;
				}
			}*/
			
			
			file << ver << endl;
			file << bnum << endl;
			string filename1(PCName.begin(), PCName.end());
			file << filename1 << endl;
			file << prod << endl;
			file.close();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Client1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:
	private: System::Windows::Forms::Button^ back_button;
	private: System::Windows::Forms::Button^ export_button;
	private: System::Windows::Forms::Button^ refresh;
	private: System::ComponentModel::IContainer^ components;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->back_button = (gcnew System::Windows::Forms::Button());
			this->export_button = (gcnew System::Windows::Forms::Button());
			this->refresh = (gcnew System::Windows::Forms::Button());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView4 = (gcnew System::Windows::Forms::DataGridView());
			this->windows_updates = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridView3 = (gcnew System::Windows::Forms::DataGridView());
			this->pcakages = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->product_name = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->sys_name = (gcnew System::Windows::Forms::Label());
			this->build_number = (gcnew System::Windows::Forms::Label());
			this->os_version = (gcnew System::Windows::Forms::Label());
			this->ip_address = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->applicatinons = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->install_date = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->version = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Client_tab = (gcnew System::Windows::Forms::TabControl());
			this->live_usage = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->pid = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->processes = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->cpu_usage = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->memory_usage = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->swap_usage = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->swapLabel = (gcnew System::Windows::Forms::Label());
			this->MemLabel = (gcnew System::Windows::Forms::Label());
			this->CpuLabel = (gcnew System::Windows::Forms::Label());
			this->StartStopBut = (gcnew System::Windows::Forms::Button());
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView3))->BeginInit();
			this->tabPage2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->BeginInit();
			this->Client_tab->SuspendLayout();
			this->live_usage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// back_button
			// 
			this->back_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->back_button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->back_button->Location = System::Drawing::Point(12, 470);
			this->back_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->back_button->Name = L"back_button";
			this->back_button->Size = System::Drawing::Size(92, 31);
			this->back_button->TabIndex = 7;
			this->back_button->Text = L"Back";
			this->back_button->UseVisualStyleBackColor = true;
			this->back_button->Click += gcnew System::EventHandler(this, &Client1::disconnect_button_Click);
			// 
			// export_button
			// 
			this->export_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->export_button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->export_button->Location = System::Drawing::Point(888, 470);
			this->export_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->export_button->Name = L"export_button";
			this->export_button->Size = System::Drawing::Size(175, 31);
			this->export_button->TabIndex = 6;
			this->export_button->Text = L"Export to a server";
			this->export_button->UseVisualStyleBackColor = true;
			this->export_button->Click += gcnew System::EventHandler(this, &Client1::export_button_Click);
			// 
			// refresh
			// 
			this->refresh->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->refresh->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->refresh->Location = System::Drawing::Point(109, 470);
			this->refresh->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->refresh->Name = L"refresh";
			this->refresh->Size = System::Drawing::Size(92, 31);
			this->refresh->TabIndex = 10;
			this->refresh->Text = L"Refresh Page";
			this->refresh->UseVisualStyleBackColor = true;
			this->refresh->Click += gcnew System::EventHandler(this, &Client1::refresh_Click);
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->dataGridView4);
			this->tabPage3->Controls->Add(this->dataGridView3);
			this->tabPage3->Location = System::Drawing::Point(4, 25);
			this->tabPage3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPage3->Size = System::Drawing::Size(1043, 414);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Windows Updates";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// dataGridView4
			// 
			this->dataGridView4->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView4->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(1) { this->windows_updates });
			this->dataGridView4->Location = System::Drawing::Point(528, 21);
			this->dataGridView4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->dataGridView4->Name = L"dataGridView4";
			this->dataGridView4->RowHeadersWidth = 51;
			this->dataGridView4->RowTemplate->Height = 24;
			this->dataGridView4->Size = System::Drawing::Size(495, 375);
			this->dataGridView4->TabIndex = 1;
			// 
			// windows_updates
			// 
			this->windows_updates->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->windows_updates->HeaderText = L"Windows Packages For Updates And Security";
			this->windows_updates->MinimumWidth = 6;
			this->windows_updates->Name = L"windows_updates";
			// 
			// dataGridView3
			// 
			this->dataGridView3->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView3->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(1) { this->pcakages });
			this->dataGridView3->Location = System::Drawing::Point(17, 21);
			this->dataGridView3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->dataGridView3->Name = L"dataGridView3";
			this->dataGridView3->RowHeadersWidth = 51;
			this->dataGridView3->RowTemplate->Height = 24;
			this->dataGridView3->Size = System::Drawing::Size(495, 375);
			this->dataGridView3->TabIndex = 0;
			this->dataGridView3->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Client1::dataGridView3_CellContentClick);
			// 
			// pcakages
			// 
			this->pcakages->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->pcakages->HeaderText = L"Windows Packages For Featrues";
			this->pcakages->MinimumWidth = 6;
			this->pcakages->Name = L"pcakages";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->groupBox1);
			this->tabPage2->Controls->Add(this->dataGridView2);
			this->tabPage2->Location = System::Drawing::Point(4, 25);
			this->tabPage2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPage2->Size = System::Drawing::Size(1043, 414);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"System Specs";
			this->tabPage2->UseVisualStyleBackColor = true;
			this->tabPage2->Click += gcnew System::EventHandler(this, &Client1::tabPage2_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->product_name);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->sys_name);
			this->groupBox1->Controls->Add(this->build_number);
			this->groupBox1->Controls->Add(this->os_version);
			this->groupBox1->Controls->Add(this->ip_address);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->groupBox1->Location = System::Drawing::Point(19, 15);
			this->groupBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox1->Size = System::Drawing::Size(384, 194);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Details";
			this->groupBox1->Enter += gcnew System::EventHandler(this, &Client1::groupBox1_Enter);
			// 
			// product_name
			// 
			this->product_name->AutoSize = true;
			this->product_name->Location = System::Drawing::Point(127, 155);
			this->product_name->Name = L"product_name";
			this->product_name->Size = System::Drawing::Size(13, 18);
			this->product_name->TabIndex = 9;
			this->product_name->Text = L"-";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(9, 155);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(112, 18);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Product Name: ";
			// 
			// sys_name
			// 
			this->sys_name->AutoSize = true;
			this->sys_name->Location = System::Drawing::Point(125, 128);
			this->sys_name->Name = L"sys_name";
			this->sys_name->Size = System::Drawing::Size(13, 18);
			this->sys_name->TabIndex = 7;
			this->sys_name->Text = L"-";
			// 
			// build_number
			// 
			this->build_number->AutoSize = true;
			this->build_number->Location = System::Drawing::Point(121, 101);
			this->build_number->Name = L"build_number";
			this->build_number->Size = System::Drawing::Size(13, 18);
			this->build_number->TabIndex = 6;
			this->build_number->Text = L"-";
			// 
			// os_version
			// 
			this->os_version->AutoSize = true;
			this->os_version->Location = System::Drawing::Point(108, 73);
			this->os_version->Name = L"os_version";
			this->os_version->Size = System::Drawing::Size(13, 18);
			this->os_version->TabIndex = 5;
			this->os_version->Text = L"-";
			// 
			// ip_address
			// 
			this->ip_address->AutoSize = true;
			this->ip_address->Location = System::Drawing::Point(103, 46);
			this->ip_address->Name = L"ip_address";
			this->ip_address->Size = System::Drawing::Size(13, 18);
			this->ip_address->TabIndex = 4;
			this->ip_address->Text = L"-";
			this->ip_address->Click += gcnew System::EventHandler(this, &Client1::ip_address_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(11, 128);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(110, 18);
			this->label4->TabIndex = 3;
			this->label4->Text = L"System Name: ";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(11, 101);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(105, 18);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Build Number: ";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(11, 73);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(92, 18);
			this->label2->TabIndex = 1;
			this->label2->Text = L"OS Version: ";
			this->label2->Click += gcnew System::EventHandler(this, &Client1::label2_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Location = System::Drawing::Point(11, 46);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(87, 18);
			this->label1->TabIndex = 0;
			this->label1->Text = L"IP Address: ";
			this->label1->Click += gcnew System::EventHandler(this, &Client1::label1_Click);
			// 
			// dataGridView2
			// 
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView2->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {
				this->applicatinons,
					this->install_date, this->version
			});
			this->dataGridView2->Location = System::Drawing::Point(424, 15);
			this->dataGridView2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->RowHeadersWidth = 51;
			this->dataGridView2->RowTemplate->Height = 24;
			this->dataGridView2->Size = System::Drawing::Size(609, 382);
			this->dataGridView2->TabIndex = 0;
			this->dataGridView2->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Client1::dataGridView2_CellContentClick);
			// 
			// applicatinons
			// 
			this->applicatinons->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->applicatinons->HeaderText = L"Applications";
			this->applicatinons->MinimumWidth = 6;
			this->applicatinons->Name = L"applicatinons";
			// 
			// install_date
			// 
			this->install_date->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::ColumnHeader;
			this->install_date->HeaderText = L"Install Date";
			this->install_date->MinimumWidth = 6;
			this->install_date->Name = L"install_date";
			this->install_date->Width = 107;
			// 
			// version
			// 
			this->version->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::None;
			this->version->HeaderText = L"Version";
			this->version->MinimumWidth = 6;
			this->version->Name = L"version";
			this->version->Width = 125;
			// 
			// Client_tab
			// 
			this->Client_tab->Controls->Add(this->tabPage2);
			this->Client_tab->Controls->Add(this->tabPage3);
			this->Client_tab->Controls->Add(this->live_usage);
			this->Client_tab->Location = System::Drawing::Point(12, 12);
			this->Client_tab->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Client_tab->Name = L"Client_tab";
			this->Client_tab->SelectedIndex = 0;
			this->Client_tab->Size = System::Drawing::Size(1051, 443);
			this->Client_tab->TabIndex = 0;
			// 
			// live_usage
			// 
			this->live_usage->Controls->Add(this->dataGridView1);
			this->live_usage->Location = System::Drawing::Point(4, 25);
			this->live_usage->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->live_usage->Name = L"live_usage";
			this->live_usage->Size = System::Drawing::Size(1043, 414);
			this->live_usage->TabIndex = 0;
			this->live_usage->Text = L"Live Usage";
			this->live_usage->UseVisualStyleBackColor = true;
			// 
			// dataGridView1
			// 
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {
				this->pid, this->processes,
					this->cpu_usage, this->memory_usage, this->swap_usage
			});
			this->dataGridView1->Location = System::Drawing::Point(29, 44);
			this->dataGridView1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersWidth = 51;
			this->dataGridView1->RowTemplate->Height = 24;
			this->dataGridView1->Size = System::Drawing::Size(468, 346);
			this->dataGridView1->TabIndex = 7;
			// 
			// pid
			// 
			this->pid->HeaderText = L"Pid";
			this->pid->MinimumWidth = 60;
			this->pid->Name = L"pid";
			this->pid->Width = 60;
			// 
			// processes
			// 
			this->processes->HeaderText = L"Processes";
			this->processes->MinimumWidth = 100;
			this->processes->Name = L"processes";
			this->processes->Width = 125;
			// 
			// cpu_usage
			// 
			this->cpu_usage->HeaderText = L"CPU Usage";
			this->cpu_usage->MinimumWidth = 70;
			this->cpu_usage->Name = L"cpu_usage";
			this->cpu_usage->Width = 70;
			// 
			// memory_usage
			// 
			this->memory_usage->HeaderText = L"Working Set (Memory)";
			this->memory_usage->MinimumWidth = 80;
			this->memory_usage->Name = L"memory_usage";
			this->memory_usage->Width = 80;
			// 
			// swap_usage
			// 
			this->swap_usage->HeaderText = L"Private Bytes (Swap)";
			this->swap_usage->MinimumWidth = 80;
			this->swap_usage->Name = L"swap_usage";
			this->swap_usage->Width = 80;
			// 
			// swapLabel
			// 
			this->swapLabel->AutoSize = true;
			this->swapLabel->Location = System::Drawing::Point(741, 479);
			this->swapLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->swapLabel->Name = L"swapLabel";
			this->swapLabel->Size = System::Drawing::Size(47, 17);
			this->swapLabel->TabIndex = 10;
			this->swapLabel->Text = L"0 MBs";
			// 
			// MemLabel
			// 
			this->MemLabel->AutoSize = true;
			this->MemLabel->Location = System::Drawing::Point(568, 479);
			this->MemLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->MemLabel->Name = L"MemLabel";
			this->MemLabel->Size = System::Drawing::Size(47, 17);
			this->MemLabel->TabIndex = 9;
			this->MemLabel->Text = L"0 MBs";
			// 
			// CpuLabel
			// 
			this->CpuLabel->AutoSize = true;
			this->CpuLabel->Location = System::Drawing::Point(400, 479);
			this->CpuLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->CpuLabel->Name = L"CpuLabel";
			this->CpuLabel->Size = System::Drawing::Size(28, 17);
			this->CpuLabel->TabIndex = 8;
			this->CpuLabel->Text = L"0%";
			// 
			// StartStopBut
			// 
			this->StartStopBut->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->StartStopBut->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->StartStopBut->Location = System::Drawing::Point(208, 470);
			this->StartStopBut->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->StartStopBut->Name = L"StartStopBut";
			this->StartStopBut->Size = System::Drawing::Size(92, 31);
			this->StartStopBut->TabIndex = 11;
			this->StartStopBut->Text = L"Stop";
			this->StartStopBut->UseVisualStyleBackColor = true;
			this->StartStopBut->Click += gcnew System::EventHandler(this, &Client1::StartStopBut_Click);
			// 
			// timer2
			// 
			this->timer2->Enabled = true;
			this->timer2->Interval = 2000;
			this->timer2->Tick += gcnew System::EventHandler(this, &Client1::timer2_Tick);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(308, 479);
			this->label6->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(82, 17);
			this->label6->TabIndex = 12;
			this->label6->Text = L"Cpu Usage:";
			this->label6->Click += gcnew System::EventHandler(this, &Client1::label6_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(452, 479);
			this->label7->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(107, 17);
			this->label7->TabIndex = 13;
			this->label7->Text = L"Memory Usage:";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(639, 479);
			this->label8->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(91, 17);
			this->label8->TabIndex = 12;
			this->label8->Text = L"Swap Usage:";
			// 
			// Client1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1075, 521);
			this->Controls->Add(this->swapLabel);
			this->Controls->Add(this->StartStopBut);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->refresh);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->back_button);
			this->Controls->Add(this->Client_tab);
			this->Controls->Add(this->MemLabel);
			this->Controls->Add(this->export_button);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->CpuLabel);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"Client1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Client";
			this->Load += gcnew System::EventHandler(this, &Client1::Client1_Load);
			this->tabPage3->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView3))->EndInit();
			this->tabPage2->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->EndInit();
			this->Client_tab->ResumeLayout(false);
			this->live_usage->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void back_button_Click(System::Object^ sender, System::EventArgs^ e) {

		
	}
private: System::Void Client1_Load(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void disconnect_button_Click(System::Object^ sender, System::EventArgs^ e) {

	this->Hide();
	obj->Show();
}


private: System::Void dataGridView2_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
		
}

private: System::Void dataGridView3_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
}
private: System::Void groupBox1_Enter(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void ip_address_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void refresh_Click(System::Object^ sender, System::EventArgs^ e) {

	loadApps();
	loadPackages();
	loadSpecs();
}
private: System::Void tabPage2_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void connect_button_Click(System::Object^ sender, System::EventArgs^ e) {

	
}

private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}

private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
}

private: System::Void disconnect_Click(System::Object^ sender, System::EventArgs^ e) {

	
}
private: System::Void export_button_Click(System::Object^ sender, System::EventArgs^ e) {
	
	if (!this->timer2->Enabled) {

		createUsageFile();
		ServerConnection^ obj = gcnew ServerConnection();
		obj->Show();

	}
	else {

		MessageBox::Show("Please Stop the Live Usage to Export the Data!");
	}
}

private: System::Void StartStopBut_Click(System::Object^ sender, System::EventArgs^ e) {

	if (this->timer2->Enabled) {
		this->timer2->Stop();
		this->StartStopBut->Text = L"Start";
	} else {
		this->timer2->Start();
		this->StartStopBut->Text = L"Stop";
	}
}
private: System::Void timer2_Tick(System::Object^ sender, System::EventArgs^ e) {
	
	tCpu = 0;
	tMem = 0;
	tSwap = 0;
	
	if (mon.FillProcesses())
		loadProcs();
}
private: System::Void label6_Click(System::Object^ sender, System::EventArgs^ e) {
}
};
}
