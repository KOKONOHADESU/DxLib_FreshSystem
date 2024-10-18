#include "FreshLib/DxLibSoundFresh.h"
#include "FreshLib/DxLibGraphicFresh.h"
#include "FreshLib/DxLibKeyFresh.h"
#include "FreshLib/DxLibMouseFresh.h"
#include "FreshLib/CSVReader.h"
#include "FreshLib/CSVWriter.h"
#include "FreshLib/MTRandom.h"

#include <DxLib.h>
#include <crtdbg.h>

using DxSound   = Sound::DxLibSoundFresh<int, int>;
using DxGraph   = Graphic::DxLibGraphicFresh<int, int>;
using DxKey     = Key::DxLibKeyFresh;
using DxMouse   = Mouse::DxLibMouseFresh;
using CSVReader = CSV::CSVReader<int,int>;
using CSVWriter = CSV::CSVWriter;
using MTRand    = Rand::MTRandom;

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

    // 画像管理 //
    DxGraph::GetInstance()->GraphicFolderPath("Data/Image/");
    DxGraph::GetInstance()->Add(0, 0, "a", ".png");
    DxGraph::GetInstance()->GetSize(0);
    DxGraph::GetInstance()->SceneInput(0);
    DrawGraph(0, 0, DxGraph::GetInstance()->GetHandle(0), true);

    // CSV読み込み //
    CSVReader::GetInstance()->CSVFolderPath("Data/CSV/");
    CSVReader::GetInstance()->Add(0,0, "rrr", 1);
    CSVReader::GetInstance()->Add(1,0, "SaveData", 1);
    CSVReader::GetInstance()->SceneInput(0);
//    printfDx("rrr = %s\n", CSVReader::GetInstance()->StringData<std::string>(0, 0, 0).c_str());
//    printfDx("SaveData = %s\n", CSVReader::GetInstance()->StringData<std::string>(1, 0, 0).c_str());

    // CSV書き込み //
    std::vector<std::vector<std::string>> data =
    {
        {"名前,最終地点,体力"},
        {"name,japan,32"}
    };
    CSVWriter::GetInstance()->CSVFolderPath("Data/CSV/");
    CSVWriter::GetInstance()->Write("SaveData", data);
    
    bool isButton = false;
    float rand = -1;
    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

        // 画面のクリア
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

        DxSound::GetInstance()->SceneInput(isButton);
        DxGraph::GetInstance()->SceneInput(isButton);        
        CSVReader::GetInstance()->SceneInput(isButton);

        DrawGraph(0, 0, DxGraph::GetInstance()->GetHandle(0), true);

        DrawFormatString(0, 0, 0xffffff, "rrr = %s", CSVReader::GetInstance()->StringData<std::string>(0, 0, 0).c_str());
        DrawFormatString(0,16, 0xffffff, "SaveData = %s", CSVReader::GetInstance()->StringData<std::string>(1, 0, 0).c_str());     
        DrawFormatString(0, 32, 0xffffff, "Mouse || X : %f , Y : %f", DxMouse::GetInstance()->GetPos<float>().x, DxMouse::GetInstance()->GetPos<float>().y);
        DrawFormatString(0, 48, 0xffffff, "MTRand = %f", rand);

#endif
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

    // メモリ解放
    Sound::DxLibSoundFresh<int, int>::GetInstance()->Destroy();
    CSV::CSVReader<int, int>::GetInstance()->Destroy();
    Graphic::DxLibGraphicFresh<int, int>::GetInstance()->Destroy();

    // ＤＸライブラリ使用の終了処理
    DxLib_End();

    // ソフトの終了 
    return 0;
}