#pragma once

#include "main.h"
#include "cscene.h"
#include "colision.h"
#include <string>

class CModel : public CScene
{
private:
	MATRIX3DMANAGER::Model* model_;
	D3DXMATRIX mtxrotation = {};
	D3DXMATRIX mtxtranslasion = {};
	D3DXMATRIX mtxscaling = {};
	D3DXMATRIX setmtx;
	static D3DXMATRIX view_;
	static D3DXMATRIX proj_;
	std::string filename_;
	SphereInfo modelinfo_;

	LPD3DXEFFECT effect_;
	D3DXHANDLE	 technique_;
	D3DXHANDLE   wvp_;
	D3DXHANDLE	 wlp_;
	D3DXHANDLE	 wlpb_;
	D3DXHANDLE	 color_;
	D3DXHANDLE	 direction_;
	D3DXHANDLE	 shadowmap_;
	D3DXHANDLE	 srcmap_;

	LPDIRECT3DSURFACE9 pshadowmapz_;
	LPDIRECT3DTEXTURE9 pshadowmap_;
	LPDIRECT3DSURFACE9 pshadowmapsurf_;
	LPDIRECT3DTEXTURE9 pedgemap_;
	LPDIRECT3DSURFACE9 pedgemapsurf_;
	LPDIRECT3DTEXTURE9 psoftmap_[2];
	LPDIRECT3DSURFACE9 psoftmapsurf_[2];
public:
	CModel(int Priority,MATRIX3DMANAGER::Model* model, std::string filename)
		: model_(model), filename_(filename), CScene(Priority){}
	~CModel(){}
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
	void SetMtxView(D3DXMATRIX view) override;
	void SetMtxProj(D3DXMATRIX proj) override;
	void SetMtxModel(D3DXMATRIX model) override;
	void SetMtxRot(D3DXMATRIX Rot) override;
	SphereInfo Set();
public:
	static CModel* Create(MATRIX3DMANAGER::Model* model, std::string filename);
};