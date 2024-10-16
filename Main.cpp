#include "DxLibSoundFresh.h"
#include "CSVReader.h"
#include "DxLibGraphicFresh.h"

#include <DxLib.h>
#include <crtdbg.h>

// プログラムはWinMainから始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // windowモード設定
    ChangeWindowMode(true);

    // ウインドウ名設定
    SetMainWindowText("DxLib_FreshSystem");

    // 画面サイズの設定
    SetGraphMode(1280, 720, 32);

    // ログ出力を行うか否か
    SetOutApplicationLogValidFlag(false);

    // ダブルバッファモード
    SetDrawScreen(DX_SCREEN_BACK);

    // ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1)
    {
        // エラーが起きたら直ちに終了
        return -1;
    }

    // サウンド管理
    {
        Sound::DxLibSoundFresh<int>* sound = new Sound::DxLibSoundFresh<int>("Data/Sound/");

        sound->Add(0, "Sound1", ".mp3");

        sound->Volume(0,255);

        sound->Play(0);

    }

    // CSV読み込み
    {
        CSV::CSVReader<int>* csv = new CSV::CSVReader<int>("Data/CSV/");

        csv->Load(0, "rrr", 1);

        printfDx("%s", csv->StringData<std::string>(0, 0, 0).c_str());

    }

    DxLibGraphicFresh* graph = new DxLibGraphicFresh();

    graph->LoadSoftGrahic("Data/Image/image.png");

    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

        // 画面のクリア
        ClearDrawScreen();

        graph->DrawGraphic();

        // 裏画面を表画面を入れ替える
        ScreenFlip();

        // escキーを押したら終了する
        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }

        // fpsを60に固定
        while (GetNowHiPerformanceCount() - time < 16667)
        {
        }
    }

    graph->EndGrahics();

    // ＤＸライブラリ使用の終了処理
    DxLib_End();

    // ソフトの終了 
    return 0;
}