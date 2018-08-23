#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <XAudio2fx.h>
#include <XAPO.h>

struct MyEffectParameter
{
	//ReflectionDelay:ダイレクトパスに対する初期反射の遅延時間　指定範囲可能範囲　0 ~ 300ミリ秒
	int ReflectionsDelay;
	//ReverbDelay:初期反射に対するリバーブの遅延、指定範囲は0～85ミリ秒
	int ReverbDelay;
	//RearDelay:左後方出力及び右後方出力の遅延　指定可能範囲 0～85ミリ秒
	int RearDelay;
	//PositionLeft:シミュレーション空間における視聴者に対する左入力の位置。　指定可能範囲は 0 ~ 30　（単位なし）
	int PositionLeft;
	//PositionRight: PositionLeftと同様右入力にのみ影響します。　指定可能範囲は 0 ~ 30(単位なし）
	int PositionRight;
	//PositionMatrixRight&Left:音源から視聴者まで距離によるインプレッションを増減させる。　指定可能範囲は 0 ~ 30（単位なし）
	int PositionMatrixLeft;
	int PositionMatrixRight;
	//EarlyDiffusion:個々の壁の反射特性を制御します。堅く平らな表面シミュレートするには小さな値を設定し、
	//散乱性の表面をシミュレートするには大きな値を設定。　指定可能範囲 0 ~15(単位なし）
	int EarlyDiffusion;
	//LateDiffusion:個々の壁のリバーブ特性を制御。指定可能範囲 0 ~ 15(単位なし）
	int LateDiffusion;
	//LowEQGain: 1kHzにおける減衰時間を基準にして低周波数の減衰時間を調整. 指定可能範囲 0 ~ 12(単位なし)
	int LowEQGain;
	//LowEQCutoff:パラメーターにより制御されるローパスフィルターの折点周波数を設定。指定漢方範囲 0 ~ 9 (単位なし)
	int LowEQCutoff;
	//HighEQGain:減衰時間を基準にして高周波数の減衰時間を調整。 0 の場合　高周波数の音が1khzの場合と同じ割合で減衰します。　指定可能範囲 0 ~ 8(単位なし)
	int HighEQGain;
	//HighEQCutoff: HighEQGainパラメーターにより制御されるハイパスフィルターの接点周波数を設定。　指定可能範囲 0 ~ 14(単位なし）
	int HighEQCutoff;
	//RoomFilterFreq:室内エフェクトのローパスフィルターの接点周波数を設定。　指定可能範囲は 20 ~ 20,000Hz
	float RoomFilterFreq;
	//RoomFilterMain:初期反射と後期フィールド残響の両方に適用されるローパスフィルターのパス バンド強度レベルを設定。　指定可能範囲 -100 ~ 0dB
	float RoomFilterMain;
	//RoomfiletHF:接点周波数での初期反射と後期フィールド残響の両方に適用されるローパスフィルターのパスバンド強度レベルを設定。指定可能範囲 -100 ~ 0dB
	float RoomFilterHF;
	//ReflectionGain:初期反射の強度/レベルを調整します。指定可能範囲は-100~20dB。
	float ReflectionsGain;
	//ReverbGain:リバーブの強度/レベルを調整します。指定可能範囲は -100 ~ 20dB
	float ReverbGain;
	//DecayTime:1kHzにおける残響減衰時間です。これは、フルスケールの入力信号が60dB減衰するまでの時間です。指定可能範囲は0.1 ~ 無限（秒）です。
	float DecayTime;
	//後期フィールド残響のモード密度を制御します。無色 (colorless) の空間では、Density を最大値 (100) に設定する必要があります。Density を小さくすると、
	//サウンドはくぐもった音 (くし形フィルターが適用された音) になります。サイロをシミュレーションするときに有効なエフェクトです。指定可能範囲は 0 ～ 100 で、単位はパーセントです。
	float Density;
	//RoomSize:音響空間の見かけ上のサイズです。　指定可能範囲は 1(30.48cm) ~ 100フィート(30.48m)
	float RoomSize;
	//WetDryMix:元の変更されてないサウンドとなる出力の割合　許容値　0 ~ 100;
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