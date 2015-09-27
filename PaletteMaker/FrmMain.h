#pragma once

void exportTiles();

namespace PaletteMaker {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FrmMain
	/// </summary>
	public ref class FrmMain : public System::Windows::Forms::Form
	{
	public:
		FrmMain(void)
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
		~FrmMain()
		{
			if (components)
			{
				delete components;
			}
		}
    protected:

    private: System::Windows::Forms::GroupBox^  groupBox1;
    public: System::Windows::Forms::RadioButton^  palBtn;
    public: System::Windows::Forms::PictureBox^  picPalOverlay;
    private: System::Windows::Forms::GroupBox^  groupBox2;
    public: System::Windows::Forms::PictureBox^  picOverlayNESPal;
    public: System::Windows::Forms::RadioButton^  pPalBtnNES;
    private: System::Windows::Forms::Button^  button1;
    public:
    private:
    public:

    private:


    private:


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
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->picPalOverlay = (gcnew System::Windows::Forms::PictureBox());
            this->palBtn = (gcnew System::Windows::Forms::RadioButton());
            this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
            this->picOverlayNESPal = (gcnew System::Windows::Forms::PictureBox());
            this->pPalBtnNES = (gcnew System::Windows::Forms::RadioButton());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->groupBox1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picPalOverlay))->BeginInit();
            this->groupBox2->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picOverlayNESPal))->BeginInit();
            this->SuspendLayout();
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->picPalOverlay);
            this->groupBox1->Controls->Add(this->palBtn);
            this->groupBox1->Location = System::Drawing::Point(532, 12);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(351, 482);
            this->groupBox1->TabIndex = 1;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"Palettes";
            // 
            // picPalOverlay
            // 
            this->picPalOverlay->BackColor = System::Drawing::Color::Black;
            this->picPalOverlay->Location = System::Drawing::Point(36, 19);
            this->picPalOverlay->Name = L"picPalOverlay";
            this->picPalOverlay->Size = System::Drawing::Size(24, 24);
            this->picPalOverlay->TabIndex = 2;
            this->picPalOverlay->TabStop = false;
            this->picPalOverlay->Visible = false;
            // 
            // palBtn
            // 
            this->palBtn->Appearance = System::Windows::Forms::Appearance::Button;
            this->palBtn->BackColor = System::Drawing::Color::Black;
            this->palBtn->FlatAppearance->BorderColor = System::Drawing::Color::White;
            this->palBtn->FlatAppearance->BorderSize = 3;
            this->palBtn->Location = System::Drawing::Point(6, 19);
            this->palBtn->Name = L"palBtn";
            this->palBtn->Size = System::Drawing::Size(24, 24);
            this->palBtn->TabIndex = 0;
            this->palBtn->UseVisualStyleBackColor = false;
            // 
            // groupBox2
            // 
            this->groupBox2->Controls->Add(this->picOverlayNESPal);
            this->groupBox2->Controls->Add(this->pPalBtnNES);
            this->groupBox2->Location = System::Drawing::Point(532, 500);
            this->groupBox2->Name = L"groupBox2";
            this->groupBox2->Size = System::Drawing::Size(345, 148);
            this->groupBox2->TabIndex = 3;
            this->groupBox2->TabStop = false;
            this->groupBox2->Text = L"NES Pal";
            // 
            // picOverlayNESPal
            // 
            this->picOverlayNESPal->BackColor = System::Drawing::Color::Black;
            this->picOverlayNESPal->Location = System::Drawing::Point(36, 19);
            this->picOverlayNESPal->Name = L"picOverlayNESPal";
            this->picOverlayNESPal->Size = System::Drawing::Size(24, 24);
            this->picOverlayNESPal->TabIndex = 2;
            this->picOverlayNESPal->TabStop = false;
            this->picOverlayNESPal->Visible = false;
            // 
            // pPalBtnNES
            // 
            this->pPalBtnNES->Appearance = System::Windows::Forms::Appearance::Button;
            this->pPalBtnNES->BackColor = System::Drawing::Color::Black;
            this->pPalBtnNES->FlatAppearance->BorderColor = System::Drawing::Color::White;
            this->pPalBtnNES->FlatAppearance->BorderSize = 3;
            this->pPalBtnNES->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->pPalBtnNES->ForeColor = System::Drawing::Color::White;
            this->pPalBtnNES->Location = System::Drawing::Point(6, 19);
            this->pPalBtnNES->Name = L"pPalBtnNES";
            this->pPalBtnNES->Size = System::Drawing::Size(24, 24);
            this->pPalBtnNES->TabIndex = 0;
            this->pPalBtnNES->Text = L"00";
            this->pPalBtnNES->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            this->pPalBtnNES->UseVisualStyleBackColor = false;
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(12, 500);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(75, 23);
            this->button1->TabIndex = 4;
            this->button1->Text = L"Export";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &FrmMain::button1_Click);
            // 
            // FrmMain
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(885, 660);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->groupBox2);
            this->Controls->Add(this->groupBox1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->Name = L"FrmMain";
            this->Text = L"Palette Maker";
            this->groupBox1->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picPalOverlay))->EndInit();
            this->groupBox2->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picOverlayNESPal))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
{
    exportTiles();
}
};
}
