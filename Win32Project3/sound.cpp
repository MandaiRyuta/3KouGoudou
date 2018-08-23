#include "sound.h"

IXAudio2* EffectSound::m_pXAudio2_ = nullptr;
IXAudio2MasteringVoice* EffectSound::m_pMasteringVoice_ = nullptr;
IXAudio2SourceVoice* EffectSound::m_apSourceVoice_[SOUND_LABEL::SOUND_NONE] = {};
BYTE* EffectSound::m_apDataAudio_[SOUND_LABEL::SOUND_NONE] = {};
DWORD EffectSound::m_aSizeAudio_[SOUND_LABEL::SOUND_NONE] = {};
IUnknown* EffectSound::m_pXAPO_ = nullptr;
XAUDIO2_EFFECT_DESCRIPTOR EffectSound::m_descriptor_;
XAUDIO2_EFFECT_CHAIN EffectSound::m_chain_;
EffectSound::SOUNDPARAM EffectSound::m_aParam[SOUND_LABEL::SOUND_NONE] = {
	{/*"�t�@�C���p�X",���[�v�p�����[�v�p����Ȃ���*/},
};
HRESULT EffectSound::InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2_, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2_->CreateMasteringVoice(&m_pMasteringVoice_);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (m_pXAudio2_)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2_->Release();
			m_pXAudio2_ = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SOUND_LABEL::SOUND_NONE; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(m_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio_[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio_[nCntSound] = (BYTE*)malloc(m_aSizeAudio_[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio_[nCntSound], m_aSizeAudio_[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �\�[�X�{�C�X�̐���
		hr = m_pXAudio2_->CreateSourceVoice(&m_apSourceVoice_[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio_[nCntSound];
		buffer.pAudioData = m_apDataAudio_[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_aParam[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice_[nCntSound]->SubmitSourceBuffer(&buffer);
		hr = XAudio2CreateVolumeMeter(&m_pXAPO_);
		//�G�t�F�N�g�쐬
		hr = XAudio2CreateReverb(&m_pXAPO_);


		m_descriptor_.InitialState = true;
		/*output�̏ꏊ�������Ă���ׁA��������������������܂��������^��*/
		m_descriptor_.OutputChannels = 2;
		m_descriptor_.pEffect = m_pXAPO_;


		m_chain_.EffectCount = 1;
		m_chain_.pEffectDescriptors = &m_descriptor_;

		m_apSourceVoice_[nCntSound]->SetEffectChain(&m_chain_);
		m_pXAPO_->Release();

	}

	return S_OK;
}

MyEffectParameter EffectSound::InitEffectSound()
{
	MyEffectParameter Effectinfo;
	Effectinfo.ReflectionsDelay = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY;
	Effectinfo.ReverbDelay = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY;
	Effectinfo.RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
	Effectinfo.PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	Effectinfo.PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	Effectinfo.PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	Effectinfo.PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	Effectinfo.EarlyDiffusion = XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION;
	Effectinfo.LateDiffusion = XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION;
	Effectinfo.LowEQGain = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN;
	Effectinfo.LowEQCutoff = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF;
	Effectinfo.HighEQGain = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN;
	Effectinfo.HighEQCutoff = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF;
	Effectinfo.RoomFilterFreq = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ;
	Effectinfo.RoomFilterMain = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN;
	Effectinfo.RoomFilterHF = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF;
	Effectinfo.ReflectionsGain = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN;
	Effectinfo.ReverbGain = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN;
	Effectinfo.DecayTime = XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME;
	Effectinfo.Density = XAUDIO2FX_REVERB_DEFAULT_DENSITY;
	Effectinfo.RoomSize = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE;
	Effectinfo.WetDryMix = XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX;

	return Effectinfo;
}

void EffectSound::UninitSound()
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL::SOUND_NONE; nCntSound++)
	{
		if (m_apSourceVoice_[nCntSound])
		{
			// �ꎞ��~
			m_apSourceVoice_[nCntSound]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			m_apSourceVoice_[nCntSound]->DestroyVoice();
			m_apSourceVoice_[nCntSound] = NULL;

			// �I�[�f�B�I�f�[�^�̊J��
			free(m_apDataAudio_[nCntSound]);
			m_apDataAudio_[nCntSound] = NULL;
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice_->DestroyVoice();
	m_pMasteringVoice_ = NULL;

	if (m_pXAudio2_)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2_->Release();
		m_pXAudio2_ = NULL;
	}

	// COM���C�u�����̏I������
	CoUninitialize();
}

HRESULT EffectSound::SetEffectSound(SOUND_LABEL label, MyEffectParameter Effectinfo)
{
	HRESULT hr;

	XAUDIO2FX_REVERB_PARAMETERS reverbParameters;

	reverbParameters.ReflectionsDelay = Effectinfo.ReflectionsDelay;
	reverbParameters.ReverbDelay = Effectinfo.ReverbDelay;
	reverbParameters.RearDelay = Effectinfo.RearDelay;
	reverbParameters.PositionLeft = Effectinfo.PositionLeft;
	reverbParameters.PositionRight = Effectinfo.PositionRight;
	reverbParameters.PositionMatrixLeft = Effectinfo.PositionMatrixLeft;
	reverbParameters.PositionMatrixRight = Effectinfo.PositionMatrixRight;
	reverbParameters.EarlyDiffusion = Effectinfo.EarlyDiffusion;
	reverbParameters.LateDiffusion = Effectinfo.LateDiffusion;
	reverbParameters.LowEQGain = Effectinfo.LowEQGain;
	reverbParameters.LowEQCutoff = Effectinfo.LowEQCutoff;
	reverbParameters.HighEQGain = Effectinfo.HighEQGain;
	reverbParameters.HighEQCutoff = Effectinfo.HighEQCutoff;
	reverbParameters.RoomFilterFreq = Effectinfo.RoomFilterFreq;
	reverbParameters.RoomFilterMain = Effectinfo.RoomFilterMain;
	reverbParameters.RoomFilterHF = Effectinfo.RoomFilterHF;
	reverbParameters.ReflectionsGain = Effectinfo.ReflectionsGain;
	reverbParameters.ReverbGain = Effectinfo.ReverbGain;
	reverbParameters.DecayTime = Effectinfo.DecayTime;
	reverbParameters.Density = Effectinfo.Density;
	reverbParameters.RoomSize = Effectinfo.RoomSize;
	reverbParameters.WetDryMix = Effectinfo.WetDryMix;

	hr = m_apSourceVoice_[label]->SetEffectParameters(0, &reverbParameters, sizeof(reverbParameters));

	return S_OK;
}

HRESULT EffectSound::PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio_[label];
	buffer.pAudioData = m_apDataAudio_[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aParam[label].nCntLoop;

	// ��Ԏ擾
	m_apSourceVoice_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		m_apSourceVoice_[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice_[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_apSourceVoice_[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	m_apSourceVoice_[label]->Start(0);

	return S_OK;
}

HRESULT EffectSound::SoundVolume(SOUND_LABEL label, float Volume)
{
	//Volume�̑傫��
	//-XAUDIO2_MAX_VOLUME_LEVEL  ~ XAUDIO2_MAX_VOLUME_LEVEL  -2^24 ~ 2^24�܂�
	//�����ꂷ��̂ŁI�m�F���Ȃ���Z�b�g����
	m_apSourceVoice_[label]->SetVolume(Volume);

	return S_OK;
}

HRESULT EffectSound::DirectionSound(SOUND_LABEL label, float LeftVolume, float RightVolume)
{
	/*Left,RightSoundSet*/
	float outputMatrix[2] = { LeftVolume ,RightVolume };
	/* Setting */
	m_apSourceVoice_[label]->SetChannelVolumes(3, outputMatrix);
	return S_OK;
}

void EffectSound::StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_apSourceVoice_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		m_apSourceVoice_[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice_[label]->FlushSourceBuffers();
	}
}

void EffectSound::StopSound(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL::SOUND_NONE; nCntSound++)
	{
		if (m_apSourceVoice_[nCntSound])
		{
			// �ꎞ��~
			m_apSourceVoice_[nCntSound]->Stop(0);
		}
	}
}
HRESULT EffectSound::CheckChunk(HANDLE hFile, DWORD format, DWORD * pChunkSize, DWORD * pChunkDataPosition)
{
	return E_NOTIMPL;
}
HRESULT EffectSound::ReadChunkData(HANDLE hFile, void * pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	return E_NOTIMPL;
}
//================================================================================
//
// [ �`�����N�̃`�F�b�N ]
//
// � ���� �
//
// �hFile              : �t�@�C���n���h��
// �format             : �t�@�C���t�H�[�}�b�g
// �pChunkSize         : �`�����N�T�C�Y
// �pChunkDataPosition : �`�����N�f�[�^�̏ꏊ
//
// � �߂�l �
//
// �HRESULT�^ : �G���[�`�F�b�N�p
//
//�y �ڍ� �z
//
// ��`�����N�f�[�^���`�F�b�N����
//
//================================================================================

HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

