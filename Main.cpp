#include "DxLibSoundFresh.h"
#include "CSVReadOnly.h"

#include <DxLib.h>
#include <crtdbg.h>

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

    {
        Sound::DxLibSoundFresh<int>* sound = new Sound::DxLibSoundFresh<int>("Data/Sound/");

        sound->Add(0, "Sound1", ".mp3");

        sound->Volume(0,255);

        sound->Play(0);
    }

    {
        CSV::CSVReadOnly* csv = new CSV::CSVReadOnly("Data/CSV/");

        csv->Load("rrr",1);

        printfDx("%s", csv->StringData(0,1).c_str());
    }

    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

        // ��ʂ̃N���A
        ClearDrawScreen();

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

    // �c�w���C�u�����g�p�̏I������
    DxLib_End();

    // �\�t�g�̏I�� 
    return 0;
}