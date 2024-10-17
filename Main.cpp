#include "DxLibSoundFresh.h"
#include "CSVReader.h"
#include "DxLibGraphicFresh.h"

#include <DxLib.h>
#include <crtdbg.h>

using DxSound   = Sound::DxLibSoundFresh<int, int>;
using CSVReader = CSV::CSVReader<int>;
using DxGraph   = Graphic::DxLibGraphicFresh<int, int>;

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

    // サウンド管理 //
    DxSound::GetInstance()->SoundFolderPath("Data/Sound/");
    DxSound::GetInstance()->Add(0, 0, "Sound1", ".mp3");
    DxSound::GetInstance()->SceneInput(0);
    DxSound::GetInstance()->Volume(0, 255);
    DxSound::GetInstance()->Play(0);

    // CSV読み込み //
    CSVReader::GetInstance()->CSVFolderPath("Data/CSV/");
    CSVReader::GetInstance()->Load(0, "rrr", 1);
    printfDx("%s", CSVReader::GetInstance()->StringData<std::string>(0, 0, 0).c_str());

    // 画像管理 //
    DxGraph::GetInstance()->GraphicFolderPath("Data/Image/");
    DxGraph::GetInstance()->Add(0, "a.png", 0);
    DxGraph::GetInstance()->GetSize(0);
    DxGraph::GetInstance()->SceneInput(0);
    DrawGraph(0, 0, DxGraph::GetInstance()->GetHandle(0), true);

    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

#if false
        // 画面のクリア

        ClearDrawScreen();





        // 裏画面を表画面を入れ替える
        ScreenFlip();
#endif

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

    // メモリ解放
    Sound::DxLibSoundFresh<int, int>::GetInstance()->Destroy();
    CSV::CSVReader<int>::GetInstance()->Destroy();
    Graphic::DxLibGraphicFresh<int, int>::GetInstance()->Destroy();

    // ＤＸライブラリ使用の終了処理
    DxLib_End();

    // ソフトの終了 
    return 0;
}