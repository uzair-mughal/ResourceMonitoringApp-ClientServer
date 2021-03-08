#pragma

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include <experimental/filesystem>
#include <chrono>
#include <msclr\marshal_cppstd.h>
#include <vector>

#pragma comment (lib, "ws2_32.lib")

using namespace std;
namespace fs = std::experimental::filesystem;
using namespace std::chrono_literals;

namespace ClientServer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Server
	/// </summary>
	public ref class Server : public System::Windows::Forms::Form
	{
	public:

		Form^ obj;

		Server(Form^ obj1)
		{	
			obj = obj1;
			InitializeComponent();
			loadserver();
			refreshFiles();
			//
			//TODO: Add the constructor code here
			//
		}

		vector<string> breakLine(string line, char del) {

			vector<string> vec;

			string str = "";

			for (auto x : line) {
				
				if (x == del) {
					vec.push_back(str);
					str = "";
				}
				str += x;
			}
			vec.push_back(str);
			return vec;
		}

		void loadApps(System::String^ fname) {

			string line;
			ifstream file(msclr::interop::marshal_as<std::string>(fname));

			if (file.is_open()) {

				try
				{

					this->dataGridView2->Rows->Clear();

					int i = 0;
					while (getline(file, line)) {

						vector<string> apps = breakLine(line, '\t');
						this->dataGridView2->Rows->Add(1);

						for (int j = 0; j < 3; j++)
							this->dataGridView2->Rows[i]->Cells[j]->Value = gcnew System::String(apps[j].c_str());

						i++;
					}
				}
				catch (const std::exception&)
				{
					MessageBox::Show("Invalid file or file is corrupt!");
				}

			}
			else
				MessageBox::Show("Error opening " + fname + "file!");

		}

		void loadFeatures(System::String^ fname) {

			string line;
			ifstream file(msclr::interop::marshal_as<std::string>(fname));

			if (file.is_open()) {

				try
				{

					this->dataGridView1->Rows->Clear();

					int i = 0;
					while (getline(file, line)) {

						this->dataGridView1->Rows->Add(1);
						this->dataGridView1->Rows[i]->Cells[0]->Value = gcnew System::String(line.c_str());

						i++;
					}
				}
				catch (const std::exception&)
				{
					MessageBox::Show("Invalid file or file is corrupt!");
				}

			}
			else
				MessageBox::Show("Error opening " + fname + "file!");

		}

		void loadSpecs(System::String^ fname) {
	
			string line;
			ifstream file(msclr::interop::marshal_as<std::string>(fname));

			if (file.is_open()) {
				
				try
				{
					getline(file, line);
					this->build_number->Text = gcnew String(line.c_str());
					getline(file, line);
					this->os_version->Text = gcnew String(line.c_str());
					getline(file, line);
					this->product_name->Text = gcnew String(line.c_str());
					getline(file, line);
					this->sys_name->Text = gcnew String(line.c_str());
					getline(file, line);
					this->ip_address->Text = gcnew String(line.c_str());

				}
				catch (const std::exception&)
				{
					MessageBox::Show("Invalid file or file is corrupt!");
				}
			}
			else
				MessageBox::Show("Error opening " + fname + "file!");


			file.close();
		}

		void loadUpdates(System::String^ fname) {
			string line;
			ifstream file(msclr::interop::marshal_as<std::string>(fname));

			if (file.is_open()) {

				try
				{

					this->dataGridView4->Rows->Clear();

					int i = 0;
					while (getline(file, line)) {

						this->dataGridView4->Rows->Add(1);
						this->dataGridView4->Rows[i]->Cells[0]->Value = gcnew System::String(line.c_str());

						i++;
					}
				}
				catch (const std::exception&)
				{
					MessageBox::Show("Invalid file or file is corrupt!");
				}

			}
			else
				MessageBox::Show("Error opening " + fname + "file!");

		}

		void loadProcs(System::String^ fname) {

			string line;
			ifstream file(msclr::interop::marshal_as<std::string>(fname));

			if (file.is_open()) {

				try
				{
					getline(file, line);
					this->CpuLabel->Text = gcnew String(line.c_str());

					getline(file, line);
					this->MemLabel->Text = gcnew String(line.c_str());

					getline(file, line);
					this->swapLabel->Text = gcnew String(line.c_str());

					int i = 0;

					dataGridView5->Rows->Clear();

					while (getline(file, line)) {
						vector<string> usage = breakLine(line, '\t');
						this->dataGridView5->Rows->Add(1);

						for (int j = 0; j < 5; j++) {

							if (j != 1)
								this->dataGridView5->Rows[i]->Cells[j]->Value = atoi(usage[j].c_str());
							else
								this->dataGridView5->Rows[i]->Cells[j]->Value = gcnew System::String(usage[j].c_str());
						}
						i++;
					}
				}
				catch (const std::exception&)
				{
					MessageBox::Show("Invalid file or file is corrupt!");
				}

			}
			else
				MessageBox::Show("Error opening " + fname + "file!");

		}

		void loadFile(System::String^ str) {
			
			loadApps(str + "_Apps.in");
			loadFeatures(str + "_Features.in");
			loadUpdates(str + "_Updates.in");
			loadSpecs(str + "_Specs.in");
			loadProcs(str + "_Usage.in");
			
			
		}

		void refreshFiles() {

			int i = 0;
			string prev = "";

			dataGridView3->Rows->Clear();

			for (auto& p : fs::directory_iterator(fs::current_path())) {
				
				if (p.path().extension().string() == ".in") {

					vector<string> temp = breakLine(p.path().filename().string(), '_');

					auto ftime = fs::last_write_time(p);
					time_t cftime = decltype(ftime)::clock::to_time_t(ftime);

					if (prev != temp[0])	{
						prev = temp[0];
						this->dataGridView3->Rows->Add(1);
						this->dataGridView3->Rows[i]->Cells[0]->Value = gcnew System::String(temp[0].c_str());
						this->dataGridView3->Rows[i]->Cells[1]->Value = gcnew System::String(asctime(std::localtime(&cftime)));
						i++;
					}
					
				}
			}
		}

		void loadserver() {

			// Initialze winsock
			WSADATA wsData;
			WORD ver = MAKEWORD(2, 2);

			int wsOk = WSAStartup(ver, &wsData);
			if (wsOk != 0)
			{
				cerr << "Can't Initialize winsock! Quitting" << endl;
				return;
			}

			// Create a socket
			SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
			if (listening == INVALID_SOCKET)
			{
				cerr << "Can't create a socket! Quitting" << endl;
				return;
			}

			// Bind the ip address and port to a socket
			sockaddr_in hint;
			hint.sin_family = AF_INET;
			hint.sin_port = htons(27000);
			hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

			char host[256];
			char* IP;
			struct hostent* host_entry;
			int hostname;
			hostname = gethostname(host, sizeof(host)); //find the host name
			host_entry = gethostbyname(host); //find host information
			IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
			String^ s = gcnew String(IP);
			ip_text->Text = s;
			s = gcnew String(host);
			h_name->Text = s;
			port_label->Text = "54000";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Server()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:


















	private: System::Windows::Forms::DataGridView^ dataGridView3;



	private: System::Windows::Forms::Button^ close_button;

	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ h_name;

	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ ip_text;
	private: System::Windows::Forms::TabControl^ Client_tab;
	private: System::Windows::Forms::TabPage^ tabPage2;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Label^ product_name;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ sys_name;
	private: System::Windows::Forms::Label^ build_number;
	private: System::Windows::Forms::Label^ os_version;
	private: System::Windows::Forms::Label^ ip_address;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::DataGridView^ dataGridView2;



	private: System::Windows::Forms::TabPage^ tabPage3;
	private: System::Windows::Forms::DataGridView^ dataGridView4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ windows_updates;
	private: System::Windows::Forms::DataGridView^ dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ pcakages;
	private: System::Windows::Forms::TabPage^ live_usage;
	private: System::Windows::Forms::DataGridView^ dataGridView5;







private: System::Windows::Forms::Button^ allow_button;
private: System::Windows::Forms::Label^ label8;

private: System::Windows::Forms::Timer^ timer1;
private: System::Windows::Forms::Label^ label9;
private: System::Windows::Forms::TextBox^ seconds_text;
private: System::Windows::Forms::Label^ label10;
private: System::Windows::Forms::Label^ label11;
private: System::Windows::Forms::Label^ port_label;



private: System::Windows::Forms::Label^ swapLabel;
private: System::Windows::Forms::Label^ label12;
private: System::Windows::Forms::Label^ label13;
private: System::Windows::Forms::Label^ MemLabel;
private: System::Windows::Forms::Label^ label14;
private: System::Windows::Forms::Label^ CpuLabel;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ pid;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ processes;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ cpu_usage;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ memory_usage;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ swap_usage;



private: System::Windows::Forms::DataGridViewTextBoxColumn^ applicatinons;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ install_date;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ version;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ filesCol;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ dateCol;





















private: System::ComponentModel::IContainer^ components;
















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
			this->dataGridView3 = (gcnew System::Windows::Forms::DataGridView());
			this->filesCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dateCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->close_button = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->h_name = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->ip_text = (gcnew System::Windows::Forms::Label());
			this->Client_tab = (gcnew System::Windows::Forms::TabControl());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->product_name = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->sys_name = (gcnew System::Windows::Forms::Label());
			this->build_number = (gcnew System::Windows::Forms::Label());
			this->os_version = (gcnew System::Windows::Forms::Label());
			this->ip_address = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->applicatinons = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->install_date = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->version = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView4 = (gcnew System::Windows::Forms::DataGridView());
			this->windows_updates = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->pcakages = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->live_usage = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView5 = (gcnew System::Windows::Forms::DataGridView());
			this->pid = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->processes = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->cpu_usage = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->memory_usage = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->swap_usage = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->allow_button = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->seconds_text = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->port_label = (gcnew System::Windows::Forms::Label());
			this->swapLabel = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->MemLabel = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->CpuLabel = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView3))->BeginInit();
			this->Client_tab->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->BeginInit();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->live_usage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView5))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView3
			// 
			this->dataGridView3->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView3->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {
				this->filesCol,
					this->dateCol
			});
			this->dataGridView3->Location = System::Drawing::Point(12, 150);
			this->dataGridView3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->dataGridView3->Name = L"dataGridView3";
			this->dataGridView3->RowHeadersWidth = 51;
			this->dataGridView3->RowTemplate->Height = 24;
			this->dataGridView3->Size = System::Drawing::Size(496, 402);
			this->dataGridView3->TabIndex = 2;
			this->dataGridView3->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Server::dataGridView3_CellContentClick);
			// 
			// filesCol
			// 
			this->filesCol->HeaderText = L"Files";
			this->filesCol->MinimumWidth = 150;
			this->filesCol->Name = L"filesCol";
			this->filesCol->Width = 220;
			// 
			// dateCol
			// 
			this->dateCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->dateCol->HeaderText = L"Date";
			this->dateCol->MinimumWidth = 150;
			this->dateCol->Name = L"dateCol";
			// 
			// close_button
			// 
			this->close_button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->close_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->close_button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->close_button->Location = System::Drawing::Point(12, 567);
			this->close_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->close_button->Name = L"close_button";
			this->close_button->Size = System::Drawing::Size(77, 30);
			this->close_button->TabIndex = 3;
			this->close_button->Text = L"Close";
			this->close_button->UseVisualStyleBackColor = true;
			this->close_button->Click += gcnew System::EventHandler(this, &Server::close_button_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(19, 26);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(71, 23);
			this->label5->TabIndex = 7;
			this->label5->Text = L"HOST:";
			// 
			// h_name
			// 
			this->h_name->AutoSize = true;
			this->h_name->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->h_name->Location = System::Drawing::Point(95, 27);
			this->h_name->Name = L"h_name";
			this->h_name->Size = System::Drawing::Size(17, 22);
			this->h_name->TabIndex = 8;
			this->h_name->Text = L"-";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(19, 49);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(107, 23);
			this->label7->TabIndex = 9;
			this->label7->Text = L"IP Address:";
			// 
			// ip_text
			// 
			this->ip_text->AutoSize = true;
			this->ip_text->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ip_text->Location = System::Drawing::Point(131, 52);
			this->ip_text->Name = L"ip_text";
			this->ip_text->Size = System::Drawing::Size(19, 25);
			this->ip_text->TabIndex = 10;
			this->ip_text->Text = L"-";
			this->ip_text->Click += gcnew System::EventHandler(this, &Server::label8_Click);
			// 
			// Client_tab
			// 
			this->Client_tab->Controls->Add(this->tabPage2);
			this->Client_tab->Controls->Add(this->tabPage3);
			this->Client_tab->Controls->Add(this->live_usage);
			this->Client_tab->Location = System::Drawing::Point(515, 110);
			this->Client_tab->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Client_tab->Name = L"Client_tab";
			this->Client_tab->SelectedIndex = 0;
			this->Client_tab->Size = System::Drawing::Size(1051, 443);
			this->Client_tab->TabIndex = 11;
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
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->product_name);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->sys_name);
			this->groupBox1->Controls->Add(this->build_number);
			this->groupBox1->Controls->Add(this->os_version);
			this->groupBox1->Controls->Add(this->ip_address);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label6);
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
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 155);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(112, 18);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Product Name: ";
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
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::Transparent;
			this->label6->Location = System::Drawing::Point(11, 46);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(87, 18);
			this->label6->TabIndex = 0;
			this->label6->Text = L"IP Address: ";
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
			this->dataGridView2->Size = System::Drawing::Size(598, 382);
			this->dataGridView2->TabIndex = 0;
			// 
			// applicatinons
			// 
			this->applicatinons->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->applicatinons->HeaderText = L"Applications";
			this->applicatinons->MinimumWidth = 50;
			this->applicatinons->Name = L"applicatinons";
			// 
			// install_date
			// 
			this->install_date->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::ColumnHeader;
			this->install_date->HeaderText = L"Install Date";
			this->install_date->MinimumWidth = 10;
			this->install_date->Name = L"install_date";
			this->install_date->Width = 107;
			// 
			// version
			// 
			this->version->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::ColumnHeader;
			this->version->HeaderText = L"Version";
			this->version->MinimumWidth = 20;
			this->version->Name = L"version";
			this->version->Width = 85;
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->dataGridView4);
			this->tabPage3->Controls->Add(this->dataGridView1);
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
			// dataGridView1
			// 
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(1) { this->pcakages });
			this->dataGridView1->Location = System::Drawing::Point(17, 21);
			this->dataGridView1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersWidth = 51;
			this->dataGridView1->RowTemplate->Height = 24;
			this->dataGridView1->Size = System::Drawing::Size(495, 375);
			this->dataGridView1->TabIndex = 0;
			// 
			// pcakages
			// 
			this->pcakages->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->pcakages->HeaderText = L"Windows Packages For Featrues";
			this->pcakages->MinimumWidth = 6;
			this->pcakages->Name = L"pcakages";
			// 
			// live_usage
			// 
			this->live_usage->Controls->Add(this->dataGridView5);
			this->live_usage->Location = System::Drawing::Point(4, 25);
			this->live_usage->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->live_usage->Name = L"live_usage";
			this->live_usage->Size = System::Drawing::Size(1043, 414);
			this->live_usage->TabIndex = 0;
			this->live_usage->Text = L"Live Usage";
			this->live_usage->UseVisualStyleBackColor = true;
			// 
			// dataGridView5
			// 
			this->dataGridView5->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView5->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {
				this->pid, this->processes,
					this->cpu_usage, this->memory_usage, this->swap_usage
			});
			this->dataGridView5->Location = System::Drawing::Point(17, 15);
			this->dataGridView5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->dataGridView5->Name = L"dataGridView5";
			this->dataGridView5->RowHeadersWidth = 51;
			this->dataGridView5->RowTemplate->Height = 24;
			this->dataGridView5->Size = System::Drawing::Size(466, 381);
			this->dataGridView5->TabIndex = 7;
			// 
			// pid
			// 
			this->pid->HeaderText = L"PID";
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
			this->memory_usage->HeaderText = L"Memory Usage";
			this->memory_usage->MinimumWidth = 80;
			this->memory_usage->Name = L"memory_usage";
			this->memory_usage->Width = 80;
			// 
			// swap_usage
			// 
			this->swap_usage->HeaderText = L"Swap Usage";
			this->swap_usage->MinimumWidth = 80;
			this->swap_usage->Name = L"swap_usage";
			this->swap_usage->Width = 80;
			// 
			// allow_button
			// 
			this->allow_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->allow_button->Location = System::Drawing::Point(109, 118);
			this->allow_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->allow_button->Name = L"allow_button";
			this->allow_button->Size = System::Drawing::Size(75, 26);
			this->allow_button->TabIndex = 12;
			this->allow_button->Text = L"Allow";
			this->allow_button->UseVisualStyleBackColor = true;
			this->allow_button->Click += gcnew System::EventHandler(this, &Server::allow_button_Click);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(12, 121);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(92, 18);
			this->label8->TabIndex = 13;
			this->label8->Text = L"File Recieve:";
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Server::timer1_Tick);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(189, 123);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(33, 17);
			this->label9->TabIndex = 14;
			this->label9->Text = L"For:";
			// 
			// seconds_text
			// 
			this->seconds_text->Location = System::Drawing::Point(229, 121);
			this->seconds_text->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->seconds_text->Name = L"seconds_text";
			this->seconds_text->Size = System::Drawing::Size(33, 22);
			this->seconds_text->TabIndex = 15;
			this->seconds_text->Text = L"20";
			this->seconds_text->TextChanged += gcnew System::EventHandler(this, &Server::seconds_text_TextChanged);
			this->seconds_text->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Server::seconds_text_KeyPress);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(269, 123);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(63, 17);
			this->label10->TabIndex = 16;
			this->label10->Text = L"Seconds";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label11->Location = System::Drawing::Point(19, 71);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(54, 23);
			this->label11->TabIndex = 17;
			this->label11->Text = L"Port:";
			// 
			// port_label
			// 
			this->port_label->AutoSize = true;
			this->port_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->port_label->Location = System::Drawing::Point(69, 70);
			this->port_label->Name = L"port_label";
			this->port_label->Size = System::Drawing::Size(19, 25);
			this->port_label->TabIndex = 18;
			this->port_label->Text = L"-";
			// 
			// swapLabel
			// 
			this->swapLabel->AutoSize = true;
			this->swapLabel->Location = System::Drawing::Point(985, 572);
			this->swapLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->swapLabel->Name = L"swapLabel";
			this->swapLabel->Size = System::Drawing::Size(47, 17);
			this->swapLabel->TabIndex = 21;
			this->swapLabel->Text = L"0 MBs";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(883, 572);
			this->label12->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(91, 17);
			this->label12->TabIndex = 22;
			this->label12->Text = L"Swap Usage:";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(696, 572);
			this->label13->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(107, 17);
			this->label13->TabIndex = 24;
			this->label13->Text = L"Memory Usage:";
			// 
			// MemLabel
			// 
			this->MemLabel->AutoSize = true;
			this->MemLabel->Location = System::Drawing::Point(812, 572);
			this->MemLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->MemLabel->Name = L"MemLabel";
			this->MemLabel->Size = System::Drawing::Size(47, 17);
			this->MemLabel->TabIndex = 20;
			this->MemLabel->Text = L"0 MBs";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(552, 572);
			this->label14->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(82, 17);
			this->label14->TabIndex = 23;
			this->label14->Text = L"Cpu Usage:";
			// 
			// CpuLabel
			// 
			this->CpuLabel->AutoSize = true;
			this->CpuLabel->Location = System::Drawing::Point(644, 572);
			this->CpuLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->CpuLabel->Name = L"CpuLabel";
			this->CpuLabel->Size = System::Drawing::Size(28, 17);
			this->CpuLabel->TabIndex = 19;
			this->CpuLabel->Text = L"0%";
			// 
			// Server
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1575, 609);
			this->Controls->Add(this->swapLabel);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->MemLabel);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->CpuLabel);
			this->Controls->Add(this->dataGridView3);
			this->Controls->Add(this->port_label);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->seconds_text);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->allow_button);
			this->Controls->Add(this->Client_tab);
			this->Controls->Add(this->ip_text);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->h_name);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->close_button);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"Server";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Server";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView3))->EndInit();
			this->Client_tab->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->EndInit();
			this->tabPage3->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->live_usage->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView5))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void dataGridView3_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
		
		if (e->ColumnIndex == 0 && this->dataGridView3->Rows[e->RowIndex]->Cells[0]->Value)
			loadFile(this->dataGridView3->Rows[e->RowIndex]->Cells[0]->Value->ToString());
	}

private: System::Void close_button_Click(System::Object^ sender, System::EventArgs^ e) {
	
	this->Hide();
	obj->Show();
}
private: System::Void label8_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void allow_button_Click(System::Object^ sender, System::EventArgs^ e) {

	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);


	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	int i = Convert::ToInt32(seconds_text->Text);

	timeval timeout = { i, 0 };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(listening, &fds);

	select(listening + 1, &fds, NULL, NULL, &timeout);

	if (FD_ISSET(listening, &fds)) {

		SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

		char host[NI_MAXHOST];		// Client's remote name
		char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

		memset(host, 0, NI_MAXHOST);
		//ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
		memset(service, 0, NI_MAXSERV);
		//ZeroMemory(service, NI_MAXSERV);

		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			cout << host << " connected on port " << service << endl;
		}
		else
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			cout << host << " connected on port " <<
				ntohs(client.sin_port) << endl;
		}

		// Close listening socket
		closesocket(listening);
		
		for (int i = 0; i < 5; i++) {

			// Work with client
			while (TRUE)
			{
				char rec[50] = "";
				int sz = recv(clientSocket, rec, 32, 0);
				rec[sz] = '\0';
				send(clientSocket, "OK", strlen("OK"), 0);

				if (strcmp(rec, "FileSend") == 0)
				{
					char fname[32] = "";
					char rec[50] = "";


					recv(clientSocket, fname, 32, 0);
					send(clientSocket, "OK", strlen("OK"), 0);

					FILE* fw = fopen(fname, "wb");

					int recs = recv(clientSocket, rec, 32, 0);
					send(clientSocket, "OK", strlen("OK"), 0);

					rec[recs] = '\0';
					int size = atoi(rec);


					while (size > 0)
					{
						char buffer[1030];

						if (size >= 1024)
						{
							recv(clientSocket, buffer, 1024, 0);
							send(clientSocket, "OK", strlen("OK"), 0);
							fwrite(buffer, 1024, 1, fw);

						}
						else
						{
							recv(clientSocket, buffer, size, 0);
							send(clientSocket, "OK", strlen("OK"), 0);
							buffer[size] = '\0';
							fwrite(buffer, size, 1, fw);
						}


						size -= 1024;

					}

					fclose(fw);
				}
				if (strcmp(rec, "EndConnection") == 0)break;
			}
		}

		MessageBox::Show("Data Recieved From A Client!");

		refreshFiles();

		// Close the socket
		closesocket(clientSocket);

		// Cleanup winsock
		WSACleanup();
	}
	else {

		MessageBox::Show("No Data Recieved From Any Client!");
		closesocket(listening);
		WSACleanup();
		return;
	}
}
private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {

	
}
private: System::Void seconds_text_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void seconds_text_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {

	char ch = e->KeyChar;

	if (!Char::IsDigit(ch) && e->KeyChar != 0x08) {

		e->Handled = true;
	}
}
};
}
