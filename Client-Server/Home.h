#pragma once
#include"Client.h"
#include "Server.h"
namespace ClientServer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Home
	/// </summary>
	public ref class Home : public System::Windows::Forms::Form
	{
	public:
		Home(void)
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
		~Home()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ client_button;
	private: System::Windows::Forms::Button^ server_button;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	protected:


	protected:

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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Home::typeid));
			this->client_button = (gcnew System::Windows::Forms::Button());
			this->server_button = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// client_button
			// 
			this->client_button->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->client_button->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"client_button.BackgroundImage")));
			this->client_button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->client_button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->client_button->Location = System::Drawing::Point(54, 54);
			this->client_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->client_button->Name = L"client_button";
			this->client_button->Size = System::Drawing::Size(71, 72);
			this->client_button->TabIndex = 0;
			this->client_button->UseVisualStyleBackColor = false;
			this->client_button->Click += gcnew System::EventHandler(this, &Home::client_button_Click);
			// 
			// server_button
			// 
			this->server_button->BackColor = System::Drawing::SystemColors::ControlText;
			this->server_button->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"server_button.BackgroundImage")));
			this->server_button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->server_button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->server_button->ForeColor = System::Drawing::SystemColors::ControlText;
			this->server_button->Location = System::Drawing::Point(164, 54);
			this->server_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->server_button->Name = L"server_button";
			this->server_button->Size = System::Drawing::Size(73, 72);
			this->server_button->TabIndex = 1;
			this->server_button->UseVisualStyleBackColor = false;
			this->server_button->Click += gcnew System::EventHandler(this, &Home::server_button_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(73, 34);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(38, 15);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Client";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(183, 34);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(42, 15);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Server";
			this->label2->Click += gcnew System::EventHandler(this, &Home::label2_Click);
			// 
			// Home
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(288, 176);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->server_button);
			this->Controls->Add(this->client_button);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"Home";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Home";
			this->Load += gcnew System::EventHandler(this, &Home::Home_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void client_button_Click(System::Object^ sender, System::EventArgs^ e) {

		this->Hide();
		Client1^ obj = gcnew Client1(this);
		obj->Show();
	}

	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void server_button_Click(System::Object^ sender, System::EventArgs^ e) {
		
		this->Hide();
		Server^ obj = gcnew Server(this);
		obj->Show();
	}
private: System::Void Home_Load(System::Object^ sender, System::EventArgs^ e) {
}
};
}
