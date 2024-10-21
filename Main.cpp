#include "FreshLib/DxLibSoundFresh.h"
#include "FreshLib/DxLibGraphicFresh.h"
#include "FreshLib/DxLibKeyFresh.h"
#include "FreshLib/DxLibMouseFresh.h"
#include "FreshLib/DxLibFontFresh.h"
#include "FreshLib/CSVReader.h"
#include "FreshLib/CSVWriter.h"
#include "FreshLib/MTRandom.h"
#include "FreshLib/DATWriter.h"
#include "FreshLib/DATReader.h"

#include <DxLib.h>
#include <crtdbg.h>

using DxSound   = Sound::DxLibSoundFresh<int, int>;
using DxGraph   = Graphic::DxLibGraphicFresh<int, int>;
using DxKey     = Key::DxLibKeyFresh;
using DxMouse   = Mouse::DxLibMouseFresh;
using CSVReader = CSV::CSVReader<int,int>;
using CSVWriter = CSV::CSVWriter;
using MTRand    = Rand::MTRandom;
using DxFont    = Font::DxLibFontFresh<int, int>;

// �v���O������WinMain����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif   

    // window���[�h�ݒ�
    ChangeWindowMode(true);

    // �E�C���h�E���ݒ�
    SetMainWindowText("DxLib_FreshSystem");

    // ��ʃT�C�Y�̐ݒ�
    SetGraphMode(1280, 720, 32);

    // ���O�o�͂��s�����ۂ�
    SetOutApplicationLogValidFlag(false);

    // �_�u���o�b�t�@���[�h
    SetDrawScreen(DX_SCREEN_BACK);

    // �c�w���C�u��������������
    if (DxLib_Init() == -1)
    {
        // �G���[���N�����璼���ɏI��
        return -1;
    }

    // �T�E���h�Ǘ� //
    DxSound::GetInstance()->SoundFolderPath("Data/Sound/");
    DxSound::GetInstance()->Add(0, 0, "Sound1", ".mp3");
    DxSound::GetInstance()->SceneInput(0);
    DxSound::GetInstance()->Volume(0, 255);
    DxSound::GetInstance()->Play(0);

    // �摜�Ǘ� //
    DxGraph::GetInstance()->GraphicFolderPath("Data/Image/");
    DxGraph::GetInstance()->Add(0, 0, "a", ".png");
    DxGraph::GetInstance()->GetSize(0);
    DxGraph::GetInstance()->SceneInput(0);
    DrawGraph(0, 0, DxGraph::GetInstance()->GetHandle(0), true);

    // CSV�ǂݍ��� //
    CSVReader::GetInstance()->CSVFolderPath("Data/CSV/");
    CSVReader::GetInstance()->Add(0,0, "rrr", 1);
    CSVReader::GetInstance()->Add(1,0, "SaveData", 1);
    CSVReader::GetInstance()->SceneInput(0);

    // CSV�������� //
    std::vector<std::vector<std::string>> data =
    {
        {"���O,�ŏI�n�_,�̗�,�G1,�G2,�G3"},
        {"name,japan,32,dead,dead,alive"}
    };
    CSVWriter::GetInstance()->CSVFolderPath("Data/CSV/");
    CSVWriter::GetInstance()->Write("SaveData", data);

    DxFont::GetInstance()->FontFolderPath("Data/Font/");
    DxFont::GetInstance()->Add(0, 0, "NikkyouSans-mLKax", ".ttf", "Nikkyou Sans");
    DxFont::GetInstance()->SceneInput(0);


    struct saveDat
    {
        int a;
        int b;
        std::string stringngngn;
    };

    saveDat datDatas{ 111,222 ,"daadadadadada" };

    Dat::DATWriter<saveDat>* dat = new Dat::DATWriter<saveDat>();
    dat->CSVFolderPath("");
    dat->Write("save", datDatas);

    Dat::DATReader<int,int, saveDat>* dat2 = new Dat::DATReader<int, int, saveDat>();
    dat2->Add(0, 0, "save");


    bool isButton = false;
    float rand = -1;

    int count = 0;
    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

        // ��ʂ̃N���A
        ClearDrawScreen();
#if true    

        DxKey::GetInstance()->Update();
        DxMouse::GetInstance()->Update();

        if (DxKey::GetInstance()->IsTrigger(KEY_INPUT_Z))
        {
            isButton = !isButton;
            rand = MTRand::GetInstance()->GetMTRand(0.0f, 100.0f);            
        }
        if (DxMouse::GetInstance()->IsTrigger(MOUSE_INPUT_1))
        {
            isButton = !isButton;
            rand = MTRand::GetInstance()->GetMTRand(0.0f, 100.0f);
        }

        if (count++ > 60 * 1)
        {
            count = 0;
            DxSound::GetInstance()->Play(0);
        }

        DxSound::GetInstance()->SceneInput(isButton);
        DxGraph::GetInstance()->SceneInput(isButton);        
        CSVReader::GetInstance()->SceneInput(isButton);
        DxSound::GetInstance()->SceneInput(isButton);
        DxFont::GetInstance()->SceneInput(isButton);
        dat2->SceneInput(isButton);

        DrawGraph(0, 0, DxGraph::GetInstance()->GetHandle(0), true);

        DrawFormatString(0, 0, 0xffffff, "rrr = %s", CSVReader::GetInstance()->StringData<std::string>(0, 0, 0).c_str());
        DrawFormatString(0,16, 0xffffff, "SaveData = %s", CSVReader::GetInstance()->StringData<std::string>(1, 0, 0).c_str());     
        DrawFormatString(0, 32, 0xffffff, "Mouse || X : %f , Y : %f", DxMouse::GetInstance()->GetPos<float>().x, DxMouse::GetInstance()->GetPos<float>().y);
        DrawFormatString(0, 48, 0xffffff, "MTRand = %f", rand);

        DrawFormatString(0, 64, 0xffff00, "%d", DxFont::GetInstance()->GetHandle(0));
        DrawFormatStringToHandle(0, 80, GetColor(255, 255, 0), DxFont::GetInstance()->GetHandle(0), "%d", DxFont::GetInstance()->GetHandle(0));

//        DrawFormatStringToHandle(0, 96, 0xffffff, DxFont::GetInstance()->GetHandle(0), "%d : %d : %s", dat2->GetData(0).a, dat2->GetData(0).b, dat2->GetData(0).stringngngn.c_str());
        DrawFormatString(0, 96, 0xffffff, "%d : %d : %s", dat2->GetData(0).a, dat2->GetData(0).b, dat2->GetData(0).stringngngn.c_str());

        

#endif
        // ����ʂ�\��ʂ����ւ���
        ScreenFlip();

        // esc�L�[����������I������
        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }

        // fps��60�ɌŒ�
        while (GetNowHiPerformanceCount() - time < 16667)
        {
        }
    }

    // ���������
    DxSound  ::GetInstance()->Destroy();
    DxGraph  ::GetInstance()->Destroy();
    DxKey    ::GetInstance()->Destroy();
    DxMouse  ::GetInstance()->Destroy();
    CSVReader::GetInstance()->Destroy();
    CSVWriter::GetInstance()->Destroy();
    MTRand   ::GetInstance()->Destroy();
    DxFont   ::GetInstance()->Destroy();    

    // �c�w���C�u�����g�p�̏I������
    DxLib_End();

    // �\�t�g�̏I�� 
    return 0;
}