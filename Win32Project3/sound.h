#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <XAudio2fx.h>
#include <XAPO.h>

struct MyEffectParameter
{
	//ReflectionDelay:�_�C���N�g�p�X�ɑ΂��鏉�����˂̒x�����ԁ@�w��͈͉\�͈́@0 ~ 300�~���b
	int ReflectionsDelay;
	//ReverbDelay:�������˂ɑ΂��郊�o�[�u�̒x���A�w��͈͂�0�`85�~���b
	int ReverbDelay;
	//RearDelay:������o�͋y�щE����o�͂̒x���@�w��\�͈� 0�`85�~���b
	int RearDelay;
	//PositionLeft:�V�~�����[�V������Ԃɂ����鎋���҂ɑ΂��鍶���͂̈ʒu�B�@�w��\�͈͂� 0 ~ 30�@�i�P�ʂȂ��j
	int PositionLeft;
	//PositionRight: PositionLeft�Ɠ��l�E���͂ɂ̂݉e�����܂��B�@�w��\�͈͂� 0 ~ 30(�P�ʂȂ��j
	int PositionRight;
	//PositionMatrixRight&Left:�������王���҂܂ŋ����ɂ��C���v���b�V�����𑝌�������B�@�w��\�͈͂� 0 ~ 30�i�P�ʂȂ��j
	int PositionMatrixLeft;
	int PositionMatrixRight;
	//EarlyDiffusion:�X�̕ǂ̔��˓����𐧌䂵�܂��B��������ȕ\�ʃV�~�����[�g����ɂ͏����Ȓl��ݒ肵�A
	//�U�����̕\�ʂ��V�~�����[�g����ɂ͑傫�Ȓl��ݒ�B�@�w��\�͈� 0 ~15(�P�ʂȂ��j
	int EarlyDiffusion;
	//LateDiffusion:�X�̕ǂ̃��o�[�u�����𐧌�B�w��\�͈� 0 ~ 15(�P�ʂȂ��j
	int LateDiffusion;
	//LowEQGain: 1kHz�ɂ����錸�����Ԃ���ɂ��Ē���g���̌������Ԃ𒲐�. �w��\�͈� 0 ~ 12(�P�ʂȂ�)
	int LowEQGain;
	//LowEQCutoff:�p�����[�^�[�ɂ�萧�䂳��郍�[�p�X�t�B���^�[�̐ܓ_���g����ݒ�B�w�芿���͈� 0 ~ 9 (�P�ʂȂ�)
	int LowEQCutoff;
	//HighEQGain:�������Ԃ���ɂ��č����g���̌������Ԃ𒲐��B 0 �̏ꍇ�@�����g���̉���1khz�̏ꍇ�Ɠ��������Ō������܂��B�@�w��\�͈� 0 ~ 8(�P�ʂȂ�)
	int HighEQGain;
	//HighEQCutoff: HighEQGain�p�����[�^�[�ɂ�萧�䂳���n�C�p�X�t�B���^�[�̐ړ_���g����ݒ�B�@�w��\�͈� 0 ~ 14(�P�ʂȂ��j
	int HighEQCutoff;
	//RoomFilterFreq:�����G�t�F�N�g�̃��[�p�X�t�B���^�[�̐ړ_���g����ݒ�B�@�w��\�͈͂� 20 ~ 20,000Hz
	float RoomFilterFreq;
	//RoomFilterMain:�������˂ƌ���t�B�[���h�c���̗����ɓK�p����郍�[�p�X�t�B���^�[�̃p�X �o���h���x���x����ݒ�B�@�w��\�͈� -100 ~ 0dB
	float RoomFilterMain;
	//RoomfiletHF:�ړ_���g���ł̏������˂ƌ���t�B�[���h�c���̗����ɓK�p����郍�[�p�X�t�B���^�[�̃p�X�o���h���x���x����ݒ�B�w��\�͈� -100 ~ 0dB
	float RoomFilterHF;
	//ReflectionGain:�������˂̋��x/���x���𒲐����܂��B�w��\�͈͂�-100~20dB�B
	float ReflectionsGain;
	//ReverbGain:���o�[�u�̋��x/���x���𒲐����܂��B�w��\�͈͂� -100 ~ 20dB
	float ReverbGain;
	//DecayTime:1kHz�ɂ�����c���������Ԃł��B����́A�t���X�P�[���̓��͐M����60dB��������܂ł̎��Ԃł��B�w��\�͈͂�0.1 ~ �����i�b�j�ł��B
	float DecayTime;
	//����t�B�[���h�c���̃��[�h���x�𐧌䂵�܂��B���F (colorless) �̋�Ԃł́ADensity ���ő�l (100) �ɐݒ肷��K�v������܂��BDensity ������������ƁA
	//�T�E���h�͂����������� (�����`�t�B���^�[���K�p���ꂽ��) �ɂȂ�܂��B�T�C�����V�~�����[�V��������Ƃ��ɗL���ȃG�t�F�N�g�ł��B�w��\�͈͂� 0 �` 100 �ŁA�P�ʂ̓p�[�Z���g�ł��B
	float Density;
	//RoomSize:������Ԃ̌�������̃T�C�Y�ł��B�@�w��\�͈͂� 1(30.48cm) ~ 100�t�B�[�g(30.48m)
	float RoomSize;
	//WetDryMix:���̕ύX����ĂȂ��T�E���h�ƂȂ�o�͂̊����@���e�l�@0 ~ 100;
	float WetDryMix;
};

class EffectSound {
public:
	enum SOUND_LABEL {
		SOUND_ATTACK,
		SOUND_NONE,
	};
public:
	EffectSound(SOUND_LABEL LabelName) : m_SoundName_(LabelName){}
	~EffectSound(){}
public:
	static HRESULT InitSound(HWND hWnd);
	static MyEffectParameter InitEffectSound();
	static void UninitSound();
	static HRESULT SetEffectSound(SOUND_LABEL label, MyEffectParameter Effectinfo);
	static HRESULT PlaySound(SOUND_LABEL label);
	static HRESULT SoundVolume(SOUND_LABEL label, float Volume);
	static HRESULT DirectionSound(SOUND_LABEL label, float LeftVolume, float RightVolume);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
private:
	SOUND_LABEL m_SoundName_;
	static IXAudio2* m_pXAudio2_; // nullptr
	static IXAudio2MasteringVoice* m_pMasteringVoice_; // nullptr
	static IXAudio2SourceVoice* m_apSourceVoice_[SOUND_LABEL::SOUND_NONE]; //{}
	static BYTE* m_apDataAudio_[SOUND_LABEL::SOUND_NONE]; //{}
	static DWORD m_aSizeAudio_[SOUND_LABEL::SOUND_NONE]; //{}
	static IUnknown* m_pXAPO_;// nullptr
	static XAUDIO2_EFFECT_DESCRIPTOR m_descriptor_;
	static XAUDIO2_EFFECT_CHAIN m_chain_;

	typedef struct {
		char* pFilename;
		int nCntLoop;
	}SOUNDPARAM;

	static SOUNDPARAM m_aParam[SOUND_LABEL::SOUND_NONE];
};