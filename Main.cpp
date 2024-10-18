#include "DxFresh/DxLibSoundFresh.h"
#include "DxFresh/DxLibGraphicFresh.h"
#include "DxFresh/DxLibKeyFresh.h"
#include "DxFresh/DxLibMouseFresh.h"
#include "DxFresh/CSVReader.h"
#include "DxFresh/CSVWriter.h"

#include <DxLib.h>
#include <crtdbg.h>

using DxSound   = Sound::DxLibSoundFresh<int, int>;
using DxGraph   = Graphic::DxLibGraphicFresh<int, int>;
using DxKey     = Key::DxLibKeyFresh;
using DxMouse   = Mouse::DxLibMouseFresh;
using CSVReader = CSV::CSVReader<int,int>;
using CSVWriter = CSV::CSVWriter;

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
//    printfDx("rrr = %s\n", CSVReader::GetInstance()->StringData<std::string>(0, 0, 0).c_str());
//    printfDx("SaveData = %s\n", CSVReader::GetInstance()->StringData<std::string>(1, 0, 0).c_str());

    // CSV�������� //
    std::vector<std::vector<std::string>> data =
    {
        {"���O,�ŏI�n�_,�̗�"},
        {"name,japan,32"}
    };
    CSVWriter::GetInstance()->CSVFolderPath("Data/CSV/");
    CSVWriter::GetInstance()->Write("SaveData", data);
    
    bool isButton = false;
    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

#if true    
        // ��ʂ̃N���A

        ClearDrawScreen();


        DxKey::GetInstance()->Update();
        DxMouse::GetInstance()->Update();

        if (DxKey::GetInstance()->IsTrigger(KEY_INPUT_Z))
        {
            isButton = !isButton;
        }
        if (DxMouse::GetInstance()->IsTrigger(MOUSE_INPUT_1))
        {
            isButton = !isButton;
        }

        DxSound::GetInstance()->SceneInput(isButton);
        DxGraph::GetInstance()->SceneInput(isButton);        
        CSVReader::GetInstance()->SceneInput(isButton);

        DrawGraph(0, 0, DxGraph::GetInstance()->GetHandle(0), true);

        DrawFormatString(0, 0, 0xffffff, "rrr = %s\n", CSVReader::GetInstance()->StringData<std::string>(0, 0, 0).c_str());
        DrawFormatString(0,16, 0xffffff, "SaveData = %s\n", CSVReader::GetInstance()->StringData<std::string>(1, 0, 0).c_str());     

        DrawFormatString(0, 32, 0xffffff, "Mouse|| X : %f , Y : %f", DxMouse::GetInstance()->GetPos<float>().x, DxMouse::GetInstance()->GetPos<float>().y);

        // ����ʂ�\��ʂ����ւ���
        ScreenFlip();
#endif

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
    Sound::DxLibSoundFresh<int, int>::GetInstance()->Destroy();
    CSV::CSVReader<int, int>::GetInstance()->Destroy();
    Graphic::DxLibGraphicFresh<int, int>::GetInstance()->Destroy();

    // �c�w���C�u�����g�p�̏I������
    DxLib_End();

    // �\�t�g�̏I�� 
    return 0;
}