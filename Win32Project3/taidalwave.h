#pragma once
#include "main.h"

class Wave {
public:
	Wave(){}
	~Wave(){}
private:
	struct MY_VERTEX_1 {
		D3DXVECTOR3 p;
		DWORD color;
		D3DXVECTOR2 t;
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVBEffect;
	LPDIRECT3DINDEXBUFFER9 m_pMyIB;
	LPDIRECT3DTEXTURE9 m_pTextureEffect;
};