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
#include "FreshLib/DxLibKeyStrokeSlide.h"

#include <DxLib.h>
#include <crtdbg.h>
#include <string>

// プログラムはWinMainから始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif   

    // windowモード設定
    ChangeWindowMode(true);

    // ウインドウ名設定
    SetMainWindowText("FreshSystem");

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

    struct SaveDat
    {
        int a;
        int b;
        const char* name;
        const char* state;
    };
    using DxSound    = SOUND  ::DxLibSoundFresh<int, int>;
    using DxGraph    = Graphic::DxLibGraphicFresh<int, int>;
    using DxKey      = KEY    ::DxLibKeyFresh;
    using DxMouse    = MOUSE  ::DxLibMouseFresh;
    using CSVReader  = CSV    ::CSVReader<int, int>;
    using CSVWriter  = CSV    ::CSVWriter;
    using DATReader  = DAT    ::DATReader<int, int, SaveDat>;
    using DATWriter  = DAT    ::DATWriter<SaveDat>;
    using MTRand     = Rand   ::MTRandom;
    using DxFont     = Font   ::DxLibFontFresh<int, int>;

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

    // CSV書き込み //
    std::vector<std::vector<std::string>> data =
    {
        {"名前,最終地点,体力,敵1,敵2,敵3,敵4"},
        {"name,japan,32,dead,dead,alive,aliveaaad"}
    };
    CSVWriter::GetInstance()->CSVFolderPath("Data/CSV/");
    CSVWriter::GetInstance()->Write("SaveData", data);

    DxFont::GetInstance()->FontFolderPath("Data/Font/");
    DxFont::GetInstance()->Add(0, 0, "NikkyouSans-mLKax", ".ttf", "Nikkyou Sans");
    DxFont::GetInstance()->SceneInput(0);

    SaveDat datDatas
    {
        100,
        200,
        "名前",
        "無能力",
    };

    DATWriter::GetInstance()->DATFolderPath("Data/DAT/");
    DATWriter::GetInstance()->Write("SaveData", datDatas);

    DATReader::GetInstance()->DATFolderPath("Data/DAT/");
    DATReader::GetInstance()->Add(0, 0, "SaveData");

    KEY::DxLibKeyStrokeSlide* pKeyStrokSlide = new KEY::DxLibKeyStrokeSlide(10, KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_SPACE);

    bool isButton = false;
    float rand = -1;

    int count = 0;
    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

        // 画面のクリア
        ClearDrawScreen();
#if true    

        DxKey::GetInstance()->Update();
        DxMouse::GetInstance()->Update();
        pKeyStrokSlide->Update();

        if (DxKey::GetInstance()->IsTrigger(KEY_INPUT_Z))
        {
            isButton = !isButton;
            rand = MTRand::GetInstance()->GetMTRand(0.0f, 100.0f);            
            count = 60;
        }
        if (DxMouse::GetInstance()->IsTrigger(MOUSE_INPUT_1))
        {
            isButton = !isButton;
            rand = MTRand::GetInstance()->GetMTRand(0.0f, 100.0f);
            count = 60;
        }

        if (count++ > 60 * 1)
        {
            count = 0;
        //    DxSound::GetInstance()->Play(0);
        }

        DxSound::GetInstance()->SceneInput(isButton);
        DxGraph::GetInstance()->SceneInput(isButton);        
        CSVReader::GetInstance()->SceneInput(isButton);
        DxSound::GetInstance()->SceneInput(isButton);
        DxFont::GetInstance()->SceneInput(isButton);
        DATReader::GetInstance()->SceneInput(isButton);

        DrawGraph(0, 0, DxGraph::GetInstance()->GetHandle(0), true);

        DrawFormatString(0, 0, 0xffffff, "rrr = %s", CSVReader::GetInstance()->StringData<std::string>(0, 0, 0).c_str());
        DrawFormatString(0,16, 0xffffff, "SaveData = %s", CSVReader::GetInstance()->StringData<std::string>(1, 0, 0).c_str());     
        DrawFormatString(0, 32, 0xffffff, "Mouse || X : %f , Y : %f", DxMouse::GetInstance()->GetPos<float>().x, DxMouse::GetInstance()->GetPos<float>().y);
        DrawFormatString(0, 48, 0xffffff, "MTRand = %f", rand);

        DrawFormatString(0, 64, 0xffff00, "%d", DxFont::GetInstance()->GetHandle(0));
        DrawFormatStringToHandle(0, 80, GetColor(255, 255, 0), DxFont::GetInstance()->GetHandle(0), "%d", DxFont::GetInstance()->GetHandle(0));

        DrawFormatString(0, 96, 0xffffff, "%d : %d : %s : %s", 
            DATReader::GetInstance()->GetData(0).a,
            DATReader::GetInstance()->GetData(0).b,
            DATReader::GetInstance()->GetData(0).name,
            DATReader::GetInstance()->GetData(0).state);

        
        DrawFormatString(0, 112, 0xffff00, "現在の選択番号 : %d , 選択結果 : %d", pKeyStrokSlide->GetSelect(), pKeyStrokSlide->GetResult());
        

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
    DxSound  ::GetInstance()->Destroy();
    DxGraph  ::GetInstance()->Destroy();
    DxKey    ::GetInstance()->Destroy();
    DxMouse  ::GetInstance()->Destroy();
    CSVReader::GetInstance()->Destroy();
    CSVWriter::GetInstance()->Destroy();
    MTRand   ::GetInstance()->Destroy();
    DxFont   ::GetInstance()->Destroy();
    DATWriter::GetInstance()->Destroy();
    DATReader::GetInstance()->Destroy();

    delete pKeyStrokSlide;

    // ＤＸライブラリ使用の終了処理
    DxLib_End();

    // ソフトの終了 
    return 0;
}