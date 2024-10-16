#pragma once

#include <map>
#include <string>
#include <DxLib.h>


/// <summary>
/// サウンド関連
/// </summary>
namespace Sound
{
    /// <summary>
    /// DxLIbのサウンドシステムを改良する
    /// </summary>
    /// <typeparam name="T">IDを指定する</typeparam>
    template <class T>
    class DxLibSoundFresh
    {
    private:
        // サウンド再生に必要なデータ
        struct SoundData
        {
            std::string filePath;  // ファイルパス
            std::string extension; // 拡張子
            int volume;            // 音量
        };

    public:
        /// <summary>
        /// コンストラクタの引数でファイルパスを指定する
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        DxLibSoundFresh(const std::string& filePath = "")
        {
            // ファイルパスの記録
            m_filePath = filePath;
        };

        ~DxLibSoundFresh()
        {
            // メモリ解放
            for (int i = 0; i < static_cast<int>(m_soundType.size()); i++)
            {
                DeleteSoundMem(m_soundType[i]);
            }
        };

        /// <summary>
        /// サウンドを追加する
        /// </summary>
        /// <param name="id"        >IDを決める                                      </param>
        /// <param name="soundName" >パスを指定する                                  </param>
        /// <param name="extension" >サウンドの拡張子を指定する                      </param>
        /// <param name="volume"    >初期ボリュームを指定する...指定しない場合は 255 </param>
        void Add(const T& id, const std::string& soundName, const std::string& extension, int volume = 255)
        {
            // 読み込み用ファイルパスを作成
            std::string fileName = m_filePath + soundName + extension;
            // サウンドの読み込み
            m_soundType[id] = LoadSoundMem(fileName.c_str());
            // 初期状態のボリューム調整
            ChangeVolumeSoundMem(m_soundType[id], volume);
        }

        /// <summary>
        /// サウンドの再生
        /// </summary>
        /// <param name="id"    >サウンドのID            </param>
        /// <param name="isLoop">ループ再生させるかどうか</param>
        void Play(const T& id, bool isLoop = false)
        {
            if (!isLoop)
            {
                // サウンド再生
                PlaySoundMem(m_soundType[id], DX_PLAYTYPE_BACK);
            }
            else
            {
                // サウンドが再生されているかの確認する
                // 再生されていない場合は再生を繰り返す
                if (CheckSoundMem(m_soundType[id]) == 0)
                {
                    // サウンド再生
                    PlaySoundMem(m_soundType[id], DX_PLAYTYPE_BACK);
                }
            }
        }

        /// <summary>
        /// ボリュームの調整
        /// </summary>
        /// <param name="id"    >サウンドのID                  </param>
        /// <param name="volume">ボリュームを 0～255 で設定する</param>
        void Volume(const T& id, int volume)
        {
            // ボリュームを変更する
            ChangeVolumeSoundMem(m_soundType[id], volume);
        }

    private:

        // サウンドデータを管理する
        std::map<T, int> m_soundType;

        // ファイルパス記録
        std::string m_filePath;
    };
}