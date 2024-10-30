#pragma once

#include "SingletonBase.h"

#include <vector>
#include <string>
#include <DxLib.h>


/// <summary>
/// �T�E���h�֘A
/// </summary>
namespace SOUND
{
    /// <summary>
    /// DxLIb�̃T�E���h�V�X�e�������ǂ���
    /// </summary>
    /// <typeparam name="T">�摜��ID������ꍇ�̌^                    </typeparam>
    /// <typeparam name="U">�ǂ̃V�[���ŉ摜��ǂݍ��ނ������Ă��邷��^</typeparam>
    template <typename T, typename U>
    class DxLibSoundFresh : public SingletonBase<DxLibSoundFresh<T,U>>
    {
    private:
        // �T�E���h�Đ��ɕK�v�ȃf�[�^
        struct SoundData
        {
            T id{};                // ID
            U scene{};             // �g�p����V�[��
            int handle = -1;       // �n���h��
            std::string filePath;  // �t�@�C���p�X
            int volume = 0;        // ����
            bool isNoEnd = false;  // �ǂ̃V�[���ł���������������Ȃ��ꍇ
        };

    public:
        // SingletonBase�N���X�̃A�N�Z�X��������
        friend class SingletonBase<DxLibSoundFresh<T, U>>;

    private:
        DxLibSoundFresh(){};

    public:
        ~DxLibSoundFresh()
        {
            // ���������
            for (int i = 0; i < static_cast<int>(m_soundType.size()); i++)
            {
                DeleteSoundMem(m_soundType[i].handle);
            }
        };

        /// <summary>
        /// �摜������K�w�̃t�H���_�[���w��
        /// </summary>
        /// <param name="grahicFolderPath"></param>
        void SoundFolderPath(const char* grahicFolderPath)
        {
            m_folderPath = grahicFolderPath;
        }
        
        /// <summary>
        /// �T�E���h��ǉ�����
        /// </summary>
        /// <param name="id"        >ID�����߂�                                      </param>
        /// <param name="scene"     >�g�p����V�[��                                  </param>
        /// <param name="soundName" >�T�E���h�̖��O                                  </param>
        /// <param name="extension" >�T�E���h�̊g���q���w�肷��                      </param>
        /// <param name="volume"    >�����{�����[�����w�肷��...�w�肵�Ȃ��ꍇ�� 255 </param>
        /// <param name="isNoEnd"   >���ׂẴV�[���Ŏg�p���邩�ǂ���                </param>
        void Add(const T& id, const U& scene, const char* soundName, const char* extension, int volume = 255 , bool isNoEnd = false)
        {
            SoundData data{};

            // ID
            data.id = id;

            // �g�p����V�[��
            data.scene = scene;
            
            // �ǂݍ��ݗp�t�@�C���p�X���쐬
            data.filePath = m_folderPath + soundName + extension;

            // �����{�����[��
            data.volume = volume;

            // ���̃V�[���ł��g�p���邩�ǂ���
            data.isNoEnd = isNoEnd;

            // �������m�ۂ��s�����ǂ���
            if (isNoEnd)
            {
                // �T�E���h�̓ǂݍ���
                data.handle = LoadSoundMem(m_folderPath.c_str());
            }
            else
            {
                // �ǂݍ��܂Ȃ�
                data.handle = -1;
            }
            
            // �f�[�^�̒ǉ�            
            m_soundType.push_back(data);
        }

        /// <summary>
        /// ���݂̃V�[�����m�F����
        /// </summary>
        /// <param name="scene">���݂̃V�[��</param>
        void SceneInput(const T& scene)
        {
            // ���ׂẲ摜�f�[�^���m�F
            for (int i = 0; i < m_soundType.size(); i++)
            {
                // ����̃f�[�^���m�F
                // ���[�h���Ă��Ȃ��f�[�^���m�F
                if (m_soundType[i].scene == scene && m_soundType[i].handle == -1)
                {
                    // �T�E���h�̃��[�h
                    m_soundType[i].handle = LoadSoundMem(m_soundType[i].filePath.c_str());

                    // �ǂݍ��ݎ��s������
                    if (m_soundType[i].handle == -1)return;

                    continue;
                }
                // ���V�[���Ŏg�p����摜�̏ꍇ
                else if (m_soundType[i].scene != scene && !m_soundType[i].isNoEnd)
                {
                    // �T�E���h�f�[�^�������Ă����ꍇ
                    if (m_soundType[i].handle != -1)
                    {
                        // �������̉��
                        DeleteSoundMem(m_soundType[i].handle);
                        m_soundType[i].handle = -1;

                        continue;
                    }
                }
            }
        }

        /// <summary>
        /// �T�E���h�̍Đ�
        /// </summary>
        /// <param name="id"    >�T�E���h��ID            </param>
        /// <param name="isLoop">���[�v�Đ������邩�ǂ���</param>
        void Play(const T& id, bool isLoop = false)
        {            
            // ���ׂĂ̊m�F
            for (int i = 0; i < m_soundType.size(); i++)
            {
                // ���[�v���邩�ǂ���
                if (!isLoop)
                {
                    // ID�̊m�F
                    if (m_soundType[i].id == id)
                    {
                        // �T�E���h�Đ�
                        PlaySoundMem(m_soundType[i].handle, DX_PLAYTYPE_BACK);
                        break;
                    }
                    else
                    {
                        // �T�E���h���Đ�����Ă��邩�̊m�F����
                        // �Đ�����Ă��Ȃ��ꍇ�͍Đ����J��Ԃ�
                        if (CheckSoundMem(m_soundType[i].handle) == 0)
                        {
                            // �T�E���h�Đ�
                            PlaySoundMem(m_soundType[i].handle, DX_PLAYTYPE_BACK);
                            break;
                        }
                    }
                }
            }            
        }

        /// <summary>
        /// �{�����[���̒���
        /// </summary>
        /// <param name="id"    >�T�E���h��ID                  </param>
        /// <param name="volume">�{�����[���� 0�`255 �Őݒ肷��</param>
        void Volume(const T& id, int volume)
        {
            // ���ׂĂ̊m�F
            for (int i = 0; i < m_soundType.size(); i++)
            {
                // �{�����[����ύX����
                ChangeVolumeSoundMem(m_soundType[i].handle, volume);
                break;
            }
        }

    private:

        // �T�E���h�f�[�^���Ǘ�����
        std::vector<SoundData> m_soundType;

        // �t�H���_�p�X�L�^
        std::string m_folderPath;
    };
}