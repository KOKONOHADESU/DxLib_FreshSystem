#include "DxLibSoundFresh.h"
#include "CSVReader.h"
#include "DxLibGraphicFresh.h"

#include <DxLib.h>
#include <crtdbg.h>

using DxSound   = Sound::DxLibSoundFresh<int, int>;
using CSVReader = CSV::CSVReader<int>;
using DxGraph   = Graphic::DxLibGraphicFresh<int, int>;

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

    // CSV�ǂݍ��� //
    CSVReader::GetInstance()->CSVFolderPath("Data/CSV/");
    CSVReader::GetInstance()->Load(0, "rrr", 1);
    printfDx("%s", CSVReader::GetInstance()->StringData<std::string>(0, 0, 0).c_str());

    // �摜�Ǘ� //
    DxGraph::GetInstance()->GraphicFolderPath("Data/Image/");
    DxGraph::GetInstance()->Add(0, "a.png", 0);
    DxGraph::GetInstance()->GetSize(0);
    DxGraph::GetInstance()->SceneInput(0);
    DrawGraph(0, 0, DxGraph::GetInstance()->GetHandle(0), true);

    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

#if false
        // ��ʂ̃N���A

        ClearDrawScreen();





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
    CSV::CSVReader<int>::GetInstance()->Destroy();
    Graphic::DxLibGraphicFresh<int, int>::GetInstance()->Destroy();

    // �c�w���C�u�����g�p�̏I������
    DxLib_End();

    // �\�t�g�̏I�� 
    return 0;
}