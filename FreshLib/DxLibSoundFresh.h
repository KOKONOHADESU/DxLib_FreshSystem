#pragma once

#include "SingletonBase.h"

#include <vector>
#include <string>
#include <DxLib.h>


/// <summary>
/// サウンド関連
/// </summary>
namespace SOUND
{
    /// <summary>
    /// DxLIbのサウンドシステムを改良する
    /// </summary>
    /// <typeparam name="T">画像にIDをつける場合の型                    </typeparam>
    /// <typeparam name="U">どのシーンで画像を読み込むかをしているする型</typeparam>
    template <typename T, typename U>
    class DxLibSoundFresh : public SingletonBase<DxLibSoundFresh<T,U>>
    {
    private:
        // サウンド再生に必要なデータ
        struct SoundData
        {
            T id{};                // ID
            U scene{};             // 使用するシーン
            int handle = -1;       // ハンドル
            std::string filePath;  // ファイルパス
            int volume = 0;        // 音量
            bool isNoEnd = false;  // どのシーンでもメモリを解放しない場合
        };

    public:
        // SingletonBaseクラスのアクセスを許可する
        friend class SingletonBase<DxLibSoundFresh<T, U>>;

    private:
        DxLibSoundFresh(){};

    public:
        ~DxLibSoundFresh()
        {
            // メモリ解放
            for (int i = 0; i < static_cast<int>(m_soundType.size()); i++)
            {
                DeleteSoundMem(m_soundType[i].handle);
            }
        };

        /// <summary>
        /// 画像がある階層のフォルダーを指定
        /// </summary>
        /// <param name="grahicFolderPath"></param>
        void SoundFolderPath(const char* grahicFolderPath)
        {
            m_folderPath = grahicFolderPath;
        }
        
        /// <summary>
        /// サウンドを追加する
        /// </summary>
        /// <param name="id"        >IDを決める                                      </param>
        /// <param name="scene"     >使用するシーン                                  </param>
        /// <param name="soundName" >サウンドの名前                                  </param>
        /// <param name="extension" >サウンドの拡張子を指定する                      </param>
        /// <param name="volume"    >初期ボリュームを指定する...指定しない場合は 255 </param>
        /// <param name="isNoEnd"   >すべてのシーンで使用するかどうか                </param>
        void Add(const T& id, const U& scene, const char* soundName, const char* extension, int volume = 255 , bool isNoEnd = false)
        {
            SoundData data{};

            // ID
            data.id = id;

            // 使用するシーン
            data.scene = scene;
            
            // 読み込み用ファイルパスを作成
            data.filePath = m_folderPath + soundName + extension;

            // 初期ボリューム
            data.volume = volume;

            // 他のシーンでも使用するかどうか
            data.isNoEnd = isNoEnd;

            // メモリ確保を行うかどうか
            if (isNoEnd)
            {
                // サウンドの読み込み
                data.handle = LoadSoundMem(m_folderPath.c_str());
            }
            else
            {
                // 読み込まない
                data.handle = -1;
            }
            
            // データの追加            
            m_soundType.push_back(data);
        }

        /// <summary>
        /// 現在のシーンを確認する
        /// </summary>
        /// <param name="scene">現在のシーン</param>
        void SceneInput(const T& scene)
        {
            // すべての画像データを確認
            for (int i = 0; i < m_soundType.size(); i++)
            {
                // 特定のデータを確認
                // ロードしていないデータを確認
                if (m_soundType[i].scene == scene && m_soundType[i].handle == -1)
                {
                    // サウンドのロード
                    m_soundType[i].handle = LoadSoundMem(m_soundType[i].filePath.c_str());

                    // 読み込み失敗したら
                    if (m_soundType[i].handle == -1)return;

                    continue;
                }
                // 他シーンで使用する画像の場合
                else if (m_soundType[i].scene != scene && !m_soundType[i].isNoEnd)
                {
                    // サウンドデータが入っていた場合
                    if (m_soundType[i].handle != -1)
                    {
                        // メモリの解放
                        DeleteSoundMem(m_soundType[i].handle);
                        m_soundType[i].handle = -1;

                        continue;
                    }
                }
            }
        }

        /// <summary>
        /// サウンドの再生
        /// </summary>
        /// <param name="id"    >サウンドのID            </param>
        /// <param name="isLoop">ループ再生させるかどうか</param>
        void Play(const T& id, bool isLoop = false)
        {            
            // すべての確認
            for (int i = 0; i < m_soundType.size(); i++)
            {
                // ループするかどうか
                if (!isLoop)
                {
                    // IDの確認
                    if (m_soundType[i].id == id)
                    {
                        // サウンド再生
                        PlaySoundMem(m_soundType[i].handle, DX_PLAYTYPE_BACK);
                        break;
                    }
                    else
                    {
                        // サウンドが再生されているかの確認する
                        // 再生されていない場合は再生を繰り返す
                        if (CheckSoundMem(m_soundType[i].handle) == 0)
                        {
                            // サウンド再生
                            PlaySoundMem(m_soundType[i].handle, DX_PLAYTYPE_BACK);
                            break;
                        }
                    }
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
            // すべての確認
            for (int i = 0; i < m_soundType.size(); i++)
            {
                // ボリュームを変更する
                ChangeVolumeSoundMem(m_soundType[i].handle, volume);
                break;
            }
        }

    private:

        // サウンドデータを管理する
        std::vector<SoundData> m_soundType;

        // フォルダパス記録
        std::string m_folderPath;
    };
}