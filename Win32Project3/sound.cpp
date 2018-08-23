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
	{/*"ファイルパス",ループ用かループ用じゃないか*/},
};
HRESULT EffectSound::InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2_, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2_->CreateMasteringVoice(&m_pMasteringVoice_);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (m_pXAudio2_)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2_->Release();
			m_pXAudio2_ = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL::SOUND_NONE; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio_[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio_[nCntSound] = (BYTE*)malloc(m_aSizeAudio_[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio_[nCntSound], m_aSizeAudio_[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = m_pXAudio2_->CreateSourceVoice(&m_apSourceVoice_[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio_[nCntSound];
		buffer.pAudioData = m_apDataAudio_[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_aParam[nCntSound].nCntLoop;

		// オーディオバッファの登録
		m_apSourceVoice_[nCntSound]->SubmitSourceBuffer(&buffer);
		hr = XAudio2CreateVolumeMeter(&m_pXAPO_);
		//エフェクト作成
		hr = XAudio2CreateReverb(&m_pXAPO_);


		m_descriptor_.InitialState = true;
		/*outputの場所を示している為、音響がおかしかったらまずここを疑う*/
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
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL::SOUND_NONE; nCntSound++)
	{
		if (m_apSourceVoice_[nCntSound])
		{
			// 一時停止
			m_apSourceVoice_[nCntSound]->Stop(0);

			// ソースボイスの破棄
			m_apSourceVoice_[nCntSound]->DestroyVoice();
			m_apSourceVoice_[nCntSound] = NULL;

			// オーディオデータの開放
			free(m_apDataAudio_[nCntSound]);
			m_apDataAudio_[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	m_pMasteringVoice_->DestroyVoice();
	m_pMasteringVoice_ = NULL;

	if (m_pXAudio2_)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2_->Release();
		m_pXAudio2_ = NULL;
	}

	// COMライブラリの終了処理
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

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio_[label];
	buffer.pAudioData = m_apDataAudio_[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aParam[label].nCntLoop;

	// 状態取得
	m_apSourceVoice_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_apSourceVoice_[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice_[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice_[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice_[label]->Start(0);

	return S_OK;
}

HRESULT EffectSound::SoundVolume(SOUND_LABEL label, float Volume)
{
	//Volumeの大きさ
	//-XAUDIO2_MAX_VOLUME_LEVEL  ~ XAUDIO2_MAX_VOLUME_LEVEL  -2^24 ~ 2^24まで
	//音割れするので！確認しながらセットする
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

	// 状態取得
	m_apSourceVoice_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_apSourceVoice_[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice_[label]->FlushSourceBuffers();
	}
}

void EffectSound::StopSound(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL::SOUND_NONE; nCntSound++)
	{
		if (m_apSourceVoice_[nCntSound])
		{
			// 一時停止
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
// [ チャンクのチェック ]
//
// ｢ 引数 ｣
//
// ･hFile              : ファイルハンドル
// ･format             : ファイルフォーマット
// ･pChunkSize         : チャンクサイズ
// ･pChunkDataPosition : チャンクデータの場所
//
// ｢ 戻り値 ｣
//
// ･HRESULT型 : エラーチェック用
//
//【 詳細 】
//
// ･チャンクデータをチェックする
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
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
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
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

