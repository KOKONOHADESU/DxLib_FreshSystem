#pragma once

#include <map>
#include <string>
#include <DxLib.h>


/// <summary>
/// �T�E���h�֘A
/// </summary>
namespace Sound
{
    /// <summary>
    /// DxLIb�̃T�E���h�V�X�e�������ǂ���
    /// </summary>
    /// <typeparam name="T">ID���w�肷��</typeparam>
    template <class T>
    class DxLibSoundFresh
    {
    private:
        // �T�E���h�Đ��ɕK�v�ȃf�[�^
        struct SoundData
        {
            std::string filePath;  // �t�@�C���p�X
            std::string extension; // �g���q
            int volume;            // ����
        };

    public:
        /// <summary>
        /// �R���X�g���N�^�̈����Ńt�@�C���p�X���w�肷��
        /// </summary>
        /// <param name="filePath">�t�@�C���p�X</param>
        DxLibSoundFresh(const std::string& filePath = "")
        {
            // �t�@�C���p�X�̋L�^
            m_filePath = filePath;
        };

        ~DxLibSoundFresh()
        {
            // ���������
            for (int i = 0; i < static_cast<int>(m_soundType.size()); i++)
            {
                DeleteSoundMem(m_soundType[i]);
            }
        };

        /// <summary>
        /// �T�E���h��ǉ�����
        /// </summary>
        /// <param name="id"        >ID�����߂�                                      </param>
        /// <param name="soundName" >�p�X���w�肷��                                  </param>
        /// <param name="extension" >�T�E���h�̊g���q���w�肷��                      </param>
        /// <param name="volume"    >�����{�����[�����w�肷��...�w�肵�Ȃ��ꍇ�� 255 </param>
        void Add(const T& id, const std::string& soundName, const std::string& extension, int volume = 255)
        {
            // �ǂݍ��ݗp�t�@�C���p�X���쐬
            std::string fileName = m_filePath + soundName + extension;
            // �T�E���h�̓ǂݍ���
            m_soundType[id] = LoadSoundMem(fileName.c_str());
            // ������Ԃ̃{�����[������
            ChangeVolumeSoundMem(m_soundType[id], volume);
        }

        /// <summary>
        /// �T�E���h�̍Đ�
        /// </summary>
        /// <param name="id"    >�T�E���h��ID            </param>
        /// <param name="isLoop">���[�v�Đ������邩�ǂ���</param>
        void Play(const T& id, bool isLoop = false)
        {
            if (!isLoop)
            {
                // �T�E���h�Đ�
                PlaySoundMem(m_soundType[id], DX_PLAYTYPE_BACK);
            }
            else
            {
                // �T�E���h���Đ�����Ă��邩�̊m�F����
                // �Đ�����Ă��Ȃ��ꍇ�͍Đ����J��Ԃ�
                if (CheckSoundMem(m_soundType[id]) == 0)
                {
                    // �T�E���h�Đ�
                    PlaySoundMem(m_soundType[id], DX_PLAYTYPE_BACK);
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
            // �{�����[����ύX����
            ChangeVolumeSoundMem(m_soundType[id], volume);
        }

    private:

        // �T�E���h�f�[�^���Ǘ�����
        std::map<T, int> m_soundType;

        // �t�@�C���p�X�L�^
        std::string m_filePath;
    };
}