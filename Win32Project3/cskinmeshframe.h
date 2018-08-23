#pragma once
#include "main.h"
#include <vector>
//派生フレーム構造体（それぞれのメッシュ用の最終ワールド行列を追加）
struct MYFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
	//オフセット行列（インデックス付き描画用）
	D3DXMATRIX OffsetMat;
	//行列テーブルのインデックス番号
	DWORD OffsetID;
};

//派生メッシュコンテナー構造体（
//コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する）
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

//Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる。
//ID3DXAllocateHierarchyは派生すること想定して設計されている。
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