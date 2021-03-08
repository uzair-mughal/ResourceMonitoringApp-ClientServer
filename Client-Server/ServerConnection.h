#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <WS2tcpip.h>
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#include<fstream>
#include "Registrykey.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#include <msclr\marshal_cppstd.h>


namespace ClientServer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ServerConnection
	/// </summary>
	public ref class ServerConnection : public System::Windows::Forms::Form
	{
	public:
		ServerConnection(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ServerConnection()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ send_button;
	protected:

	protected:

	private: System::Windows::Forms::TextBox^ port_text;

	private: System::Windows::Forms::TextBox^ ip_address;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ canecl_button;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->send_button = (gcnew System::Windows::Forms::Button());
			this->canecl_button = (gcnew System::Windows::Forms::Button());
			this->port_text = (gcnew System::Windows::Forms::TextBox());
			this->ip_address = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// send_button
			// 
			this->send_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->send_button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->send_button->Location = System::Drawing::Point(287, 90);
			this->send_button->Name = L"send_button";
			this->send_button->Size = System::Drawing::Size(75, 23);
			this->send_button->TabIndex = 0;
			this->send_button->Text = L"Send";
			this->send_button->UseVisualStyleBackColor = true;
			this->send_button->Click += gcnew System::EventHandler(this, &ServerConnection::button1_Click);
			// 
			// canecl_button
			// 
			this->canecl_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->canecl_button->Location = System::Drawing::Point(23, 91);
			this->canecl_button->Name = L"canecl_button";
			this->canecl_button->Size = System::Drawing::Size(75, 23);
			this->canecl_button->TabIndex = 1;
			this->canecl_button->Text = L"Cancel";
			this->canecl_button->UseVisualStyleBackColor = true;
			this->canecl_button->Click += gcnew System::EventHandler(this, &ServerConnection::canecl_button_Click);
			// 
			// port_text
			// 
			this->port_text->Location = System::Drawing::Point(292, 38);
			this->port_text->Name = L"port_text";
			this->port_text->Size = System::Drawing::Size(70, 22);
			this->port_text->TabIndex = 2;
			this->port_text->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ServerConnection::port_text_KeyPress);
			// 
			// ip_address
			// 
			this->ip_address->Location = System::Drawing::Point(109, 38);
			this->ip_address->Name = L"ip_address";
			this->ip_address->Size = System::Drawing::Size(131, 22);
			this->ip_address->TabIndex = 3;
			this->ip_address->TextChanged += gcnew System::EventHandler(this, &ServerConnection::ip_address_TextChanged);
			this->ip_address->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ServerConnection::ip_address_KeyPress);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(20, 39);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(83, 18);
			this->label1->TabIndex = 4;
			this->label1->Text = L"IP Address:";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label1->Click += gcnew System::EventHandler(this, &ServerConnection::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(246, 39);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(40, 18);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Port:";
			// 
			// ServerConnection
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(418, 138);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->ip_address);
			this->Controls->Add(this->port_text);
			this->Controls->Add(this->canecl_button);
			this->Controls->Add(this->send_button);
			this->Name = L"ServerConnection";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"ServerConnection";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {

		string ipAddress = msclr::interop::marshal_as<std::string>(ip_address->Text); // IP Address of the server
		int port = Convert::ToInt32(port_text->Text);			// Listening port # on the server

		// Initialize WinSock
		WSAData data;
		WORD ver = MAKEWORD(2, 2);
		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0)
		{
			cerr << "Can't start Winsock, Err #" << wsResult << endl;
			return;
		}

		// Create socket
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
			WSACleanup();
			return;
		}

		// Fill in a hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		// Connect to server
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			MessageBox::Show("Error Connecting to Server!!");
			cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
			closesocket(sock);
			WSACleanup();
			return;
		}

		string files[5] = { "_Specs.txt","_Features.txt","_Updates.txt","_Apps.txt","_Usage.txt" };

		RegistryKey* Key = RegistryKey::HKLM().OpenSubKey64(L"SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName");
		wstring PCName = Key->GetValue(L"ComputerName");
		
		
		// Sending 

		for (int i = 0; i < 5; i++) {

			string filename(PCName.begin(), PCName.end());
			filename.append(files[i]);

			char rec[32] = "";

			send(sock, "FileSend", strlen("FileSend"), 0);
			int sz = recv(sock, rec, 32, 0);
			rec[sz] = '\0';

			ifstream myFile(filename.c_str(), ios::in | ios::binary | ios::ate);
			int size = (int)myFile.tellg();
			myFile.close();

			char filesize[10];
			_itoa(size, filesize, 10);

			send(sock, filename.c_str(), strlen(filename.c_str()), 0);
			char rec1[32] = "";
			recv(sock, rec1, 32, 0);

			send(sock, filesize, strlen(filesize), 0);
			recv(sock, rec1, 32, 0);


			FILE* fr = fopen(filename.c_str(), "rb");

			while (size > 0)
			{
				char buffer[1030];

				if (size >= 1024)
				{
					fread(buffer, 1024, 1, fr);
					send(sock, buffer, 1024, 0);
					recv(sock, rec1, 32, 0);

				}
				else
				{
					fread(buffer, size, 1, fr);
					buffer[size] = '\0';
					send(sock, buffer, size, 0);
					recv(sock, rec1, 32, 0);
				}
				size -= 1024;
			}

			fclose(fr);

			// Send Close Connection Signal
			send(sock, "EndConnection", strlen("EndConnection"), 0);
			sz = recv(sock, rec1, 32, 0);
			rec1[sz] = '\0';
		}

		// Gracefully close down everything
		closesocket(sock);
		WSACleanup();

		MessageBox::Show("File was Sucessfully sent!");
	}
private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void canecl_button_Click(System::Object^ sender, System::EventArgs^ e) {

	this->Hide();
}
private: System::Void ip_address_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void ip_address_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {

	char ch = e->KeyChar;

	if (!Char::IsDigit(ch) && e->KeyChar != 0x08  && ch != 46) {

		e->Handled = true;
	}
}
private: System::Void port_text_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {

	char ch = e->KeyChar;

	if (!Char::IsDigit(ch) && e->KeyChar != 0x08) {

		e->Handled = true;
	}
}
};
}