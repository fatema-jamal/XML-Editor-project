#pragma once

#include "huffman.h"
#include "Tree.h"



namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
    using namespace std;
	 
	
	/// <summary>
	/// Summary for MyForm
	/// </summary>
    //Tree tr;
    const string WHITESPACE = " \n\r\t\f\v";
    string name;
     void MarshalString(String^ s, string& os) {
        using namespace Runtime::InteropServices;
        const char* chars =
            (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
        os = chars;
        Marshal::FreeHGlobal(IntPtr((void*)chars));
    }

   
    
    public ref class MyForm : public System::Windows::Forms::Form
    {

    public:

        MyForm(void)
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
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }
    private: System::Windows::Forms::ListBox^ lBOutput;
    protected:
    private: System::Windows::Forms::ListBox^ lBInput;
    private: System::Windows::Forms::Button^ btnImport;
    private: System::Windows::Forms::Button^ btnFormat;
    private: System::Windows::Forms::Button^ btnToJson;
    private: System::Windows::Forms::Button^ btnCheck;
    private: System::Windows::Forms::Button^ btnCompress;
    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::Button^ btnMin;


    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->lBOutput = (gcnew System::Windows::Forms::ListBox());
            this->lBInput = (gcnew System::Windows::Forms::ListBox());
            this->btnImport = (gcnew System::Windows::Forms::Button());
            this->btnFormat = (gcnew System::Windows::Forms::Button());
            this->btnToJson = (gcnew System::Windows::Forms::Button());
            this->btnCheck = (gcnew System::Windows::Forms::Button());
            this->btnCompress = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->btnMin = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // lBOutput
            // 
            this->lBOutput->FormattingEnabled = true;
            this->lBOutput->Location = System::Drawing::Point(394, 73);
            this->lBOutput->Name = L"lBOutput";
            this->lBOutput->Size = System::Drawing::Size(349, 212);
            this->lBOutput->TabIndex = 0;
            // 
            // lBInput
            // 
            this->lBInput->FormattingEnabled = true;
            this->lBInput->Location = System::Drawing::Point(12, 73);
            this->lBInput->Name = L"lBInput";
            this->lBInput->Size = System::Drawing::Size(339, 212);
            this->lBInput->TabIndex = 1;
            // 
            // btnImport
            // 
            this->btnImport->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->btnImport->Location = System::Drawing::Point(30, 301);
            this->btnImport->Name = L"btnImport";
            this->btnImport->Size = System::Drawing::Size(93, 36);
            this->btnImport->TabIndex = 2;
            this->btnImport->Text = L"Import";
            this->btnImport->UseVisualStyleBackColor = true;
            this->btnImport->Click += gcnew System::EventHandler(this, &MyForm::btnImport_Click);
            // 
            // btnFormat
            // 
            this->btnFormat->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->btnFormat->Location = System::Drawing::Point(153, 301);
            this->btnFormat->Name = L"btnFormat";
            this->btnFormat->Size = System::Drawing::Size(82, 36);
            this->btnFormat->TabIndex = 3;
            this->btnFormat->Text = L"Format";
            this->btnFormat->UseVisualStyleBackColor = true;
            this->btnFormat->Click += gcnew System::EventHandler(this, &MyForm::btnFormat_Click);
            // 
            // btnToJson
            // 
            this->btnToJson->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->btnToJson->Location = System::Drawing::Point(258, 301);
            this->btnToJson->Name = L"btnToJson";
            this->btnToJson->Size = System::Drawing::Size(159, 36);
            this->btnToJson->TabIndex = 4;
            this->btnToJson->Text = L"Convert to JSON";
            this->btnToJson->UseVisualStyleBackColor = true;
            this->btnToJson->Click += gcnew System::EventHandler(this, &MyForm::btnToJson_Click);
            // 
            // btnCheck
            // 
            this->btnCheck->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->btnCheck->Location = System::Drawing::Point(451, 301);
            this->btnCheck->Name = L"btnCheck";
            this->btnCheck->Size = System::Drawing::Size(94, 36);
            this->btnCheck->TabIndex = 5;
            this->btnCheck->Text = L"Check";
            this->btnCheck->UseVisualStyleBackColor = true;
            this->btnCheck->Click += gcnew System::EventHandler(this, &MyForm::btnCheck_Click);
            // 
            // btnCompress
            // 
            this->btnCompress->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->btnCompress->Location = System::Drawing::Point(690, 301);
            this->btnCompress->Name = L"btnCompress";
            this->btnCompress->Size = System::Drawing::Size(79, 36);
            this->btnCompress->TabIndex = 6;
            this->btnCompress->Text = L"Compress";
            this->btnCompress->UseVisualStyleBackColor = true;
            this->btnCompress->Click += gcnew System::EventHandler(this, &MyForm::btnCompress_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->label1->Location = System::Drawing::Point(282, 9);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(194, 43);
            this->label1->TabIndex = 7;
            this->label1->Text = L"XML Editor";
            // 
            // btnMin
            // 
            this->btnMin->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->btnMin->Location = System::Drawing::Point(574, 301);
            this->btnMin->Name = L"btnMin";
            this->btnMin->Size = System::Drawing::Size(79, 36);
            this->btnMin->TabIndex = 8;
            this->btnMin->Text = L"Minify";
            this->btnMin->UseVisualStyleBackColor = true;
            this->btnMin->Click += gcnew System::EventHandler(this, &MyForm::btnMin_Click);
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(781, 366);
            this->Controls->Add(this->btnMin);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->btnCompress);
            this->Controls->Add(this->btnCheck);
            this->Controls->Add(this->btnToJson);
            this->Controls->Add(this->btnFormat);
            this->Controls->Add(this->btnImport);
            this->Controls->Add(this->lBInput);
            this->Controls->Add(this->lBOutput);
            this->Name = L"MyForm";
            this->Text = L"XML_Editor";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void btnImport_Click(System::Object^ sender, System::EventArgs^ e) {
        OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
        String^ line = "";
        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {

            StreamReader^ sr = gcnew StreamReader(openFileDialog1->FileName);
            auto file = openFileDialog1->FileName;
            MarshalString(file, name);
             auto tr = Tree(name);
             lBInput->HorizontalScrollbar = true;
            lBInput->Items->Clear();
            fstream inFile(name, ios::in);
            if (inFile.good())
            {
                while (line != nullptr)
                {
                    line = sr->ReadLine();
                    if (line != nullptr)
                    {
                        lBInput->Items->Add(line);
                    }
                }
                sr->Close();
                lBOutput->HorizontalScrollbar = true;
            }
            else { line = "Data can't be previwed";
                   lBInput->Items->Add(line);
            }
        }
    }
    private: System::Void btnFormat_Click(System::Object^ sender, System::EventArgs^ e) {
        auto tr = Tree(name);
        string frmt;
        String^ file;
        int i = 0;

       /* string formated = name;
        formated.append("//formated.xml");*/
        //tr.formatingFile(formated);
        String^ line = "";
        OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {   

              
               file = openFileDialog1->FileName;
              MarshalString(file,frmt);
              fstream createdFile(frmt, ios::out);
              tr.formatingFile(frmt);
              i++;
              
        }
        if (i == 1)
        {
          StreamReader^ sr = gcnew StreamReader(file);
            lBOutput->Items->Clear();
            while (line != nullptr)
            {
                line = sr->ReadLine();
                if (line != nullptr)
                {
                    lBOutput->Items->Add(line);
                }
            }
            sr->Close();
        }
        
        
       

       
       


    }



    private: System::Void btnToJson_Click(System::Object^ sender, System::EventArgs^ e) {
        auto tr = Tree(name);
        string frmt;
        String^ file;
        int i = 0;

        /* string formated = name;
         formated.append("//formated.xml");*/
         //tr.formatingFile(formated);
        String^ line = "";
        OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {


            file = openFileDialog1->FileName;
            MarshalString(file, frmt);
            fstream createdFile(frmt, ios::out);
            tr.convertToJson(frmt);
            i++;

        }
        if (i == 1)
        {
            StreamReader^ sr = gcnew StreamReader(file);
            lBOutput->Items->Clear();
            while (line != nullptr)
            {
                line = sr->ReadLine();
                if (line != nullptr)
                {
                    lBOutput->Items->Add(line);
                }
            }
            sr->Close();
        }

        lBOutput->HorizontalScrollbar = true;



    }
private: System::Void btnCheck_Click(System::Object^ sender, System::EventArgs^ e) {
    auto tr = Tree(name);
    string frmt;
    String^ file;
    int i = 0;

    /* string formated = name;
     formated.append("//formated.xml");*/
     //tr.formatingFile(formated);
    String^ line = "";
    OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
    if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {


        file = openFileDialog1->FileName;
        MarshalString(file, frmt);
        fstream createdFile(frmt, ios::out);
        tr.printErrors(frmt);
        i++;

    }
    if (i == 1)
    {
        StreamReader^ sr = gcnew StreamReader(file);
        lBOutput->Items->Clear();
        while (line != nullptr)
        {
            line = sr->ReadLine();
            if (line != nullptr)
            {
                lBOutput->Items->Add(line);
            }
        }
        sr->Close();
    }

    lBOutput->HorizontalScrollbar = true;


}
private: System::Void btnCompress_Click(System::Object^ sender, System::EventArgs^ e) {

    auto tr = Tree(name);
    string frmt;
    String^ file;
    int i = 0;

    /* string formated = name;
     formated.append("//formated.xml");*/
     //tr.formatingFile(formated);
    String^ line = "";
    OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
    if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {


        file = openFileDialog1->FileName;
        MarshalString(file, frmt);
        fstream createdFile(frmt, ios::out);
        huffman h(name, frmt);

        h.create_pq();

        h.create_huffman_tree();

        h.calculate_huffman_codes();

        h.coding_save();
       /* huffman h(name, frmt);

        h.recreate_huffman_tree();

        h.decoding_save();*/
        i++;

    }
    if (i == 1)
    {
        StreamReader^ sr = gcnew StreamReader(file);
        lBOutput->Items->Clear();
        while (line != nullptr)
        {
            line = sr->ReadLine();
            if (line != nullptr)
            {
                lBOutput->Items->Add(line);
            }
        }
        sr->Close();
    }


}
private: System::Void btnMin_Click(System::Object^ sender, System::EventArgs^ e) {
    auto tr = Tree(name);
    string frmt;
    String^ file;
    int i = 0;

    /* string formated = name;
     formated.append("//formated.xml");*/
     //tr.formatingFile(formated);
    String^ line = "";
    OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
    if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {


        file = openFileDialog1->FileName;
        MarshalString(file, frmt);
        fstream createdFile(frmt, ios::out);
        tr.Minifying(frmt);
        i++;

    }
    
    
        StreamReader^ sr = gcnew StreamReader(file);
        lBOutput->Items->Clear();
        while (line != nullptr)
        {
            line = sr->ReadLine();
            if (line != nullptr)
            {
                lBOutput->Items->Add(line);
            }
        }
        lBOutput->HorizontalScrollbar = true;

       
    
    sr->Close();
    

}
};


}
