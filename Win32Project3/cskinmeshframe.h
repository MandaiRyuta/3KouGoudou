#pragma once
#include "main.h"
#include <vector>
//�h���t���[���\���́i���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ��j
struct MYFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
	//�I�t�Z�b�g�s��i�C���f�b�N�X�t���`��p�j
	D3DXMATRIX OffsetMat;
	//�s��e�[�u���̃C���f�b�N�X�ԍ�
	DWORD OffsetID;
};

//�h�����b�V���R���e�i�[�\���́i
//�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTextures;

	DWORD dwWeight;
	DWORD dwBoneNum;
	LPD3DXBUFFER pBoneBuffer;
	D3DXMATRIX** ppBoneMatrix;
	D3DXMATRIX* pBoneOffsetMatrices;

	LPD3DXMESH pOriMesh;
	DWORD NumPaletteEntries;

	std::vector<D3DXMATRIX> m_WorkBoneMatArray;
	std::vector<MYFRAME*> BoneFrameArray;

	MYMESHCONTAINER() {
		ppBoneMatrix = NULL;
		pBoneOffsetMatrices = NULL;
	}

};

//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
//ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
		CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);

private:

};