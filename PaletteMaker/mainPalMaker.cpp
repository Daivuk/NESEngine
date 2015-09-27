#include <Windows.h>
#include "FrmMain.h"
#include <vector>
#include <cinttypes>
#include <unordered_map>

using namespace PaletteMaker;
using namespace std;

uint16_t NESpalette8[] = {
    0x1D << 2, 0x1D << 2, 0x1D << 2, /* Value 0 */
    0x09 << 2, 0x06 << 2, 0x23 << 2, /* Value 1 */
    0x00 << 2, 0x00 << 2, 0x2A << 2, /* Value 2 */
    0x11 << 2, 0x00 << 2, 0x27 << 2, /* Value 3 */
    0x23 << 2, 0x00 << 2, 0x1D << 2, /* Value 4 */
    0x2A << 2, 0x00 << 2, 0x04 << 2, /* Value 5 */
    0x29 << 2, 0x00 << 2, 0x00 << 2, /* Value 6 */
    0x1F << 2, 0x02 << 2, 0x00 << 2, /* Value 7 */
    0x10 << 2, 0x0B << 2, 0x00 << 2, /* Value 8 */
    0x00 << 2, 0x11 << 2, 0x00 << 2, /* Value 9 */
    0x00 << 2, 0x14 << 2, 0x00 << 2, /* Value 10 */
    0x00 << 2, 0x0F << 2, 0x05 << 2, /* Value 11 */
    0x06 << 2, 0x0F << 2, 0x17 << 2, /* Value 12 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 13 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 14 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 15 */
    0x2F << 2, 0x2F << 2, 0x2F << 2, /* Value 16 */
    0x00 << 2, 0x1C << 2, 0x3B << 2, /* Value 17 */
    0x08 << 2, 0x0E << 2, 0x3B << 2, /* Value 18 */
    0x20 << 2, 0x00 << 2, 0x3C << 2, /* Value 19 */
    0x2F << 2, 0x00 << 2, 0x2F << 2, /* Value 20 */
    0x39 << 2, 0x00 << 2, 0x16 << 2, /* Value 21 */
    0x36 << 2, 0x0A << 2, 0x00 << 2, /* Value 22 */
    0x32 << 2, 0x13 << 2, 0x03 << 2, /* Value 23 */
    0x22 << 2, 0x1C << 2, 0x00 << 2, /* Value 24 */
    0x00 << 2, 0x25 << 2, 0x00 << 2, /* Value 25 */
    0x00 << 2, 0x2A << 2, 0x00 << 2, /* Value 26 */
    0x00 << 2, 0x24 << 2, 0x0E << 2, /* Value 27 */
    0x00 << 2, 0x20 << 2, 0x22 << 2, /* Value 28 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 29 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 30 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 31 */
    0x3F << 2, 0x3F << 2, 0x3F << 2, /* Value 32 */
    0x0F << 2, 0x2F << 2, 0x3F << 2, /* Value 33 */
    0x17 << 2, 0x25 << 2, 0x3F << 2, /* Value 34 */
    0x33 << 2, 0x22 << 2, 0x3F << 2, /* Value 35 */
    0x3D << 2, 0x1E << 2, 0x3F << 2, /* Value 36 */
    0x3F << 2, 0x1D << 2, 0x2D << 2, /* Value 37 */
    0x3F << 2, 0x1D << 2, 0x18 << 2, /* Value 38 */
    0x3F << 2, 0x26 << 2, 0x0E << 2, /* Value 39 */
    0x3C << 2, 0x2F << 2, 0x0F << 2, /* Value 40 */
    0x20 << 2, 0x34 << 2, 0x04 << 2, /* Value 41 */
    0x13 << 2, 0x37 << 2, 0x12 << 2, /* Value 42 */
    0x16 << 2, 0x3E << 2, 0x26 << 2, /* Value 43 */
    0x00 << 2, 0x3A << 2, 0x36 << 2, /* Value 44 */
    0x1E << 2, 0x1E << 2, 0x1E << 2, /* Value 45 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 46 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 47 */
    0x3F << 2, 0x3F << 2, 0x3F << 2, /* Value 48 */
    0x2A << 2, 0x39 << 2, 0x3F << 2, /* Value 49 */
    0x31 << 2, 0x35 << 2, 0x3F << 2, /* Value 50 */
    0x35 << 2, 0x32 << 2, 0x3F << 2, /* Value 51 */
    0x3F << 2, 0x31 << 2, 0x3F << 2, /* Value 52 */
    0x3F << 2, 0x31 << 2, 0x36 << 2, /* Value 53 */
    0x3F << 2, 0x2F << 2, 0x2C << 2, /* Value 54 */
    0x3F << 2, 0x36 << 2, 0x2A << 2, /* Value 55 */
    0x3F << 2, 0x39 << 2, 0x28 << 2, /* Value 56 */
    0x38 << 2, 0x3F << 2, 0x28 << 2, /* Value 57 */
    0x2A << 2, 0x3C << 2, 0x2F << 2, /* Value 58 */
    0x2C << 2, 0x3F << 2, 0x33 << 2, /* Value 59 */
    0x27 << 2, 0x3F << 2, 0x3C << 2, /* Value 60 */
    0x31 << 2, 0x31 << 2, 0x31 << 2, /* Value 61 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 62 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 63 */
};

uint32_t NESpalette[] = {
    0xFF666765, 0xFF001f9d, 0xFF210dad, 0xFF45049c,
    0xFF6b036e, 0xFF72031e, 0xFF651100, 0xFF451f00, 
    0xFF232e00, 0xFF003900, 0xFF003d00, 0xFF003821, 
    0xFF003266, 0xFF000000, 0xFF000000, 0xFF000000,

    0xFFb1b1af, 0xFF0855ea, 0xFF473dff, 0xFF7730fe,
    0xFFad2cce, 0xFFbd2a64, 0xFFb53a00, 0xFF8f4c00,
    0xFF636000, 0xFF1b7000, 0xFF007700, 0xFF00743c,
    0xFF006d99, 0xFF000000, 0xFF000000, 0xFF000000,

    0xFFffffff, 0xFF4dadff, 0xFF8795ff, 0xFFb986ff,
    0xFFf180ff, 0xFFff7ad3, 0xFFff875f, 0xFFef9812,
    0xFFc9ab00, 0xFF7fbe00, 0xFF47c820, 0xFF2cc870,
    0xFF2fc4cc, 0xFF51514f, 0xFF000000, 0xFF000000,

    0xFFffffff, 0xFFbae5ff, 0xFFd1dbff, 0xFFe6d5ff,
    0xFFfdd2ff, 0xFFffcff5, 0xFFffd5c5, 0xFFffdba3,
    0xFFefe391, 0xFFd0ec8f, 0xFFb9f0a6, 0xFFaef0c7,
    0xFFafeeee, 0xFFbbbcba, 0xFF000000, 0xFF000000
};

uint8_t chr[4096] = {0};
uint8_t nam[960] = {0};
uint8_t chrFull[65536] = {0};
uint8_t screen[61440] = {0};
uint8_t screenReal[61440 * 4] = {0};

void OnPalBtnChanged(System::Object ^sender, System::EventArgs ^e);
void OnPalBtnChangedNES(System::Object ^sender, System::EventArgs ^e);
void UpdateScreen();

ref class ColorBind
{
public:
    System::Windows::Forms::RadioButton^ pBtn = nullptr;
    int colorId = 0;
};

ref class MiniPal
{
public:
    array<System::Windows::Forms::RadioButton^>^ colorBtns = gcnew array<System::Windows::Forms::RadioButton ^>(4);
};

ref class PictureBoxWithInterpolationMode : public PictureBox
{
public:
protected:
    void OnPaint(PaintEventArgs^ paintEventArgs) override
    {
        paintEventArgs->Graphics->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
        PictureBox::OnPaint(paintEventArgs);
    }
};

ref class Program
{
public:
    static FrmMain ^pFrm = nullptr;
    static System::Windows::Forms::RadioButton ^pSelected = nullptr;
    static array<System::Windows::Forms::RadioButton^>^ NESPal = gcnew array<System::Windows::Forms::RadioButton ^>(64);
    static System::Collections::Generic::Dictionary<System::Windows::Forms::RadioButton^, ColorBind^>^ colorBinds = gcnew System::Collections::Generic::Dictionary<System::Windows::Forms::RadioButton^, ColorBind^>();
    static array<MiniPal^>^ miniPals = gcnew array<MiniPal ^>(32);
    static PictureBoxWithInterpolationMode^ picNam;
};

int getColorID(uint32_t color)
{
    int id = 0;
    for (auto &c : NESpalette)
    {
        if (c == color) return id;
        ++id;
    }
    return 0;
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    // Build palette
    //for (int i = 0; i < 64; ++i)
    //{
    //    auto r = (uint32_t)NESpalette8[i * 3 + 0];
    //    auto g = (uint32_t)NESpalette8[i * 3 + 1];
    //    auto b = (uint32_t)NESpalette8[i * 3 + 2];
    //    uint32_t color = 0xFF000000 | (b << 16) | (g << 8) | r;
    //    NESpalette[i + 0 * 64] = color;
    //    NESpalette[i + 1 * 64] = color;
    //    NESpalette[i + 2 * 64] = color;
    //    NESpalette[i + 3 * 64] = color;
    //}

    Program::pFrm = gcnew FrmMain();

    Program::picNam = (gcnew PictureBoxWithInterpolationMode());
    Program::picNam->BackColor = System::Drawing::Color::Black;
    Program::picNam->Location = System::Drawing::Point(12, 12);
    Program::picNam->Name = L"picNam";
    Program::picNam->Size = System::Drawing::Size(514, 482);
    Program::picNam->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
    Program::picNam->TabIndex = 0;
    Program::picNam->TabStop = false;
    Program::pFrm->Controls->Add(Program::picNam);

    // Generate NES palette
    auto pPalBtnNES = Program::pFrm->pPalBtnNES;
    for (int i = 0; i < 56; ++i)
    {
        auto k = (i / 14) * 16 + i % 14;
        auto color = NESpalette[k];
        auto x = i % 14;
        auto y = i / 14;

        auto pNewBtn = gcnew System::Windows::Forms::RadioButton();

        pNewBtn->Appearance = System::Windows::Forms::Appearance::Button;
        pNewBtn->BackColor = System::Drawing::Color::FromArgb(color);
        pNewBtn->FlatAppearance->CheckedBackColor = pNewBtn->BackColor;
        pNewBtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        pNewBtn->FlatAppearance->BorderColor = System::Drawing::Color::White;
        pNewBtn->FlatAppearance->BorderSize = 0;
        pNewBtn->Location = System::Drawing::Point(
            pPalBtnNES->Location.X + x * 22,
            pPalBtnNES->Location.Y + y * 22);
        pNewBtn->Size = System::Drawing::Size(20, 20);
        pNewBtn->TabIndex = 0;
        pNewBtn->UseVisualStyleBackColor = false;
        pNewBtn->CheckedChanged += gcnew System::EventHandler(&OnPalBtnChangedNES);
        pNewBtn->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 5.0F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        pNewBtn->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
        pNewBtn->ForeColor = System::Drawing::Color::White;
        pNewBtn->Text = System::Int32(k).ToString("X2");

        auto pColorBind = gcnew ColorBind();
        pColorBind->colorId = k;
        pColorBind->pBtn = pNewBtn;
        Program::colorBinds[pColorBind->pBtn] = pColorBind;

        Program::NESPal[k] = pNewBtn;
        pPalBtnNES->Parent->Controls->Add(pNewBtn);
    }
    pPalBtnNES->Hide();

    // Generate our palette btns
    FILE *pFic = nullptr;
    fopen_s(&pFic, "../palettes.bin", "rb");

    auto pPalBtn = Program::pFrm->palBtn;
    for (int j = 0; j < 32; ++j)
    {
        auto pMiniPad = gcnew MiniPal();
        for (int i = 0; i < 4; ++i)
        {
            auto pNewBtn = gcnew System::Windows::Forms::RadioButton();

            pNewBtn->Appearance = System::Windows::Forms::Appearance::Button;
            pNewBtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            pNewBtn->FlatAppearance->BorderColor = System::Drawing::Color::White;
            pNewBtn->FlatAppearance->BorderSize = 0;
            pNewBtn->Location = System::Drawing::Point(
                pPalBtn->Location.X + (j % 2) * 120 + i * 22, 
                pPalBtn->Location.Y + (j / 2) * 24);
            pNewBtn->Size = System::Drawing::Size(20, 20);
            pNewBtn->TabIndex = 0;
            pNewBtn->UseVisualStyleBackColor = false;
            pNewBtn->CheckedChanged += gcnew System::EventHandler(&OnPalBtnChanged);

            auto pColorBind = gcnew ColorBind();
            if (pFic)
            {
                uint8_t colorId;
                fread(&colorId, 1, 1, pFic);
                pColorBind->colorId = colorId;
            }
            else
            {
                pColorBind->colorId = 0;
            }
            pNewBtn->BackColor = System::Drawing::Color::FromArgb(NESpalette[pColorBind->colorId]);
            pNewBtn->FlatAppearance->CheckedBackColor = pNewBtn->BackColor;
            pColorBind->pBtn = pNewBtn;
            Program::colorBinds[pColorBind->pBtn] = pColorBind;

            pMiniPad->colorBtns[i] = pNewBtn;

            pPalBtn->Parent->Controls->Add(pNewBtn);
        }
        Program::miniPals[j] = pMiniPad;
    }
    pPalBtn->Hide();

    if (pFic) fclose(pFic);

    fopen_s(&pFic, "../assets/pattern.chr", "rb");
    if (pFic)
    {
        fseek(pFic, 4096, SEEK_SET);
        fread(chr, 1, 4096, pFic);
        fclose(pFic);
    }

    fopen_s(&pFic, "../tiles.nam", "rb");
    if (pFic)
    {
        fread(nam, 1, 960, pFic);
        fclose(pFic);
    }

    for (auto i = 0; i < 256; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            auto b1 = chr[i * 16 + j + 0];
            auto b2 = chr[i * 16 + j + 8];
            for (int ii = 0; ii < 8; ++ii)
            {
                auto shift = 7 - ii;
                if (shift == 0)
                {
                    chrFull[i * 64 + j * 8 + ii] = (b1 & 1) | ((b2 << 1) & 2);
                }
                else
                {
                    chrFull[i * 64 + j * 8 + ii] = ((b1 >> shift) & 1) | ((b2 >> (shift - 1)) & 2);
                }
            }
        }
    }

    for (int i = 0; i < sizeof(nam); ++i)
    {
        auto k = nam[i];
        auto pChr = chrFull + k * 64;
        for (int y = (i / 32) * 8; y < (i / 32) * 8 + 8; ++y)
        {
            for (int x = (i % 32) * 8; x < (i % 32) * 8 + 8; ++x)
            {
                screen[y * 32 * 8 + x] = *pChr++;
            }
        }
    }

    Program::pFrm->ShowDialog();

    UpdateScreen();

    return 0;
}

Bitmap ^MakeBitmap(MiniPal ^pMiniPal)
{
    if (pMiniPal)
    {
        uint32_t colors[4];
        for (int i = 0; i < 4; ++i)
        {
            auto pColorBind = Program::colorBinds[pMiniPal->colorBtns[i]];
            colors[i] = NESpalette[pColorBind->colorId];
        }
        for (int i = 0; i < sizeof(screen); ++i)
        {
            memcpy(screenReal + i * 4, colors + screen[i], sizeof(uint32_t));
        }
    }
    else
    {
        for (int i = 0; i < sizeof(screen); ++i)
        {
            screenReal[i * 4 + 0] = screen[i] * 85;
            screenReal[i * 4 + 1] = screen[i] * 85;
            screenReal[i * 4 + 2] = screen[i] * 85;
            screenReal[i * 4 + 3] = 255;
        }
    }

    auto pBitmap = gcnew Bitmap(256, 240);
    auto bitmapData = pBitmap->LockBits(System::Drawing::Rectangle(0, 0, 256, 240), System::Drawing::Imaging::ImageLockMode::WriteOnly, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
    auto ptr = bitmapData->Scan0;
    memcpy((void*)ptr, screenReal, sizeof(screenReal));
    pBitmap->UnlockBits(bitmapData);

    return pBitmap;
}

void UpdateScreen()
{
    if (Program::pSelected)
    {
        MiniPal ^pMiniPal = nullptr;
        for each (auto miniPal in Program::miniPals)
        {
            for each (auto btnPal in miniPal->colorBtns)
            {
                if (btnPal == Program::pSelected)
                {
                    pMiniPal = miniPal;
                    break;
                }
            }
            if (pMiniPal) break;
        }

        Program::picNam->Image = MakeBitmap(pMiniPal);
    }
    else
    {
        Program::picNam->Image = MakeBitmap(nullptr);
    }
}

void OnPalBtnChanged(System::Object ^sender, System::EventArgs ^e)
{
    System::Windows::Forms::RadioButton ^pBtn = dynamic_cast<System::Windows::Forms::RadioButton^>(sender);
    if (!pBtn->Checked) return;
    if (pBtn)
    {
        Program::pSelected = pBtn;
        auto pColorBind = Program::colorBinds[pBtn];
        Program::NESPal[pColorBind->colorId]->Checked = true;

        Program::pFrm->picPalOverlay->Location = System::Drawing::Point(
            pBtn->Location.X - 2,
            pBtn->Location.Y - 2);
        Program::pFrm->picPalOverlay->Show();
        Program::pFrm->picPalOverlay->SendToBack();
    }
}

void OnPalBtnChangedNES(System::Object ^sender, System::EventArgs ^e)
{
    System::Windows::Forms::RadioButton ^pBtn = dynamic_cast<System::Windows::Forms::RadioButton^>(sender);
    if (!pBtn->Checked) return;
    if (pBtn)
    {
        Program::pFrm->picOverlayNESPal->Location = System::Drawing::Point(
            pBtn->Location.X - 2,
            pBtn->Location.Y - 2);
        Program::pFrm->picOverlayNESPal->Show();
        Program::pFrm->picOverlayNESPal->SendToBack();

        if (Program::pSelected)
        {
            auto colorBind = Program::colorBinds[pBtn];
            auto colorBindSelected = Program::colorBinds[Program::pSelected];
            colorBindSelected->colorId = colorBind->colorId;

            Program::pSelected->BackColor = pBtn->BackColor;
            Program::pSelected->FlatAppearance->CheckedBackColor = pBtn->BackColor;

            UpdateScreen();
        }
    }
}

void exportTiles()
{
    FILE *pFic = nullptr;
    fopen_s(&pFic, "../palettes.bin", "wb");
    for each (auto miniPal in Program::miniPals)
    {
        for each (auto colorBtn in miniPal->colorBtns)
        {
            auto pColorBind = Program::colorBinds[colorBtn];
            auto color = static_cast<uint8_t>(pColorBind->colorId);
            fwrite(&color, 1, 1, pFic);
        }
    }
    fclose(pFic);

    // Export tilesets
    int i = 0;
    for (int j = 0; j < Program::miniPals->Length; j += 4)
    {
        auto pBitmap0 = MakeBitmap(Program::miniPals[j + 0]);
        auto pBitmap1 = MakeBitmap(Program::miniPals[j + 1]);
        auto pBitmap2 = MakeBitmap(Program::miniPals[j + 2]);
        auto pBitmap3 = MakeBitmap(Program::miniPals[j + 3]);

        auto pBitmap = gcnew Bitmap(512, 480);
        auto canvas = Graphics::FromImage(pBitmap);
        canvas->DrawImage(pBitmap0, System::Drawing::Rectangle(0, 0, 256, 240));
        canvas->DrawImage(pBitmap1, System::Drawing::Rectangle(256, 0, 256, 240));
        canvas->DrawImage(pBitmap2, System::Drawing::Rectangle(0, 240, 256, 240));
        canvas->DrawImage(pBitmap3, System::Drawing::Rectangle(256, 240, 256, 240));
        canvas->Save();

        pBitmap->Save("../tileset" + i + ".png");
        ++i;
    }
}
