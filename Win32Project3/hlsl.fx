// -------------------------------------------------------------
// �\�t�g�V���h�E
// 
// Copyright (c) 2002-2003 IMAGIRE Takashi. All rights reserved.
// -------------------------------------------------------------

// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------
float4x4 mWVP;		// ���[�J������ˉe��Ԃւ̍��W�ϊ�
float4x4 mWLP;		// ���[�J������ˉe��Ԃւ̍��W�ϊ�
float4x4 mWLPB;		// �e�N�X�`�����W�n�ւ̎ˉe
float4   vCol;		// ���b�V���̐F
float4	 vLightDir;	// ���C�g�̕���

static float MAP_SIZE = 512.0f;

// -------------------------------------------------------------
// �e�N�X�`��
// -------------------------------------------------------------
texture ShadowMap;
sampler ShadowMapSamp = sampler_state
{
    Texture = <ShadowMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};
// -------------------------------------------------------------
texture SrcMap;
sampler SrcSamp = sampler_state
{
    Texture = <SrcMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};
// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;
	float4 Diffuse		: COLOR0;
	float4 Ambient		: COLOR1;
	float4 Depth		: TEXCOORD0;
	float4 ShadowMapUV	: TEXCOORD1;
};
// -------------------------------------------------------------
struct VS_OUTPUT_QUAD
{
    float4 Pos			: POSITION;
	float2 Tex0			: TEXCOORD0;
	float2 Tex1			: TEXCOORD1;
	float2 Tex2			: TEXCOORD2;
	float2 Tex3			: TEXCOORD3;
	float2 Tex4			: TEXCOORD4;
	float2 Tex5			: TEXCOORD5;
	float2 Tex6			: TEXCOORD6;
	float2 Tex7			: TEXCOORD7;
};

// -------------------------------------------------------------
// 1�p�X�ځF�[�x�}�b�v�̍쐬
// -------------------------------------------------------------

// -------------------------------------------------------------
// ���_�V�F�[�_�v���O����
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos    : POSITION,          // ���f���̒��_
      float3 Normal : NORMAL	         // ���f���̖@��
){
    VS_OUTPUT Out = (VS_OUTPUT)0;        // �o�̓f�[�^
    
    // ���W�ϊ�
	float4 pos = mul( Pos, mWLP );
	
    // �ʒu���W
    Out.Pos = pos;
    
    // �J�������W�n�ł̐[�x���e�N�X�`���ɓ����
    Out.Depth = pos.zzzw;

    return Out;
}
// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{   
    float4 Color = In.Depth / In.Depth.w;

    return Color;
}  

// -------------------------------------------------------------
// �Q�p�X�ځF�G�b�W���o
// -------------------------------------------------------------

// -------------------------------------------------------------
// ���_�V�F�[�_�v���O����
// -------------------------------------------------------------
VS_OUTPUT_QUAD VS_pass1(
      float4 Pos    : POSITION,          // ���f���̒��_
      float4 Tex    : TEXCOORD0	         // �e�N�X�`�����W
){
    VS_OUTPUT_QUAD Out = (VS_OUTPUT_QUAD)0;        // �o�̓f�[�^
    
    // �ʒu���W
    Out.Pos = Pos;
    
    Out.Tex0 = Tex + float2(-0.5f/MAP_SIZE, -0.5f/MAP_SIZE);
    Out.Tex1 = Tex + float2(+0.5f/MAP_SIZE, +0.5f/MAP_SIZE);
    Out.Tex2 = Tex + float2(-0.5f/MAP_SIZE, +0.5f/MAP_SIZE);
    Out.Tex3 = Tex + float2(+0.5f/MAP_SIZE, -0.5f/MAP_SIZE);

    return Out;
}
// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float4 PS_pass1(VS_OUTPUT_QUAD In) : COLOR
{   
    float4 Color;
	
	float d0 = tex2D( SrcSamp, In.Tex0 ) - tex2D( SrcSamp, In.Tex1 );
	float d1 = tex2D( SrcSamp, In.Tex2 ) - tex2D( SrcSamp, In.Tex3 );
	
	Color = d0*d0 + d1*d1;
	
    return Color;
}
// -------------------------------------------------------------
// 3�p�X�ځF�ڂ���
// -------------------------------------------------------------

// -------------------------------------------------------------
// ���_�V�F�[�_�v���O����
// -------------------------------------------------------------
VS_OUTPUT_QUAD VS_pass2(
      float4 Pos    : POSITION,          // ���f���̒��_
      float4 Tex    : TEXCOORD0	         // �e�N�X�`�����W
){
    VS_OUTPUT_QUAD Out = (VS_OUTPUT_QUAD)0;        // �o�̓f�[�^
    
    // �ʒu���W
    Out.Pos = Pos;
    
    Out.Tex0 = Tex + float2(-3.0f/MAP_SIZE, -3.0f/MAP_SIZE);
    Out.Tex1 = Tex + float2(-3.0f/MAP_SIZE, -1.0f/MAP_SIZE);
    Out.Tex2 = Tex + float2(-3.0f/MAP_SIZE,  1.0f/MAP_SIZE);
    Out.Tex3 = Tex + float2(-3.0f/MAP_SIZE,  3.0f/MAP_SIZE);
    Out.Tex4 = Tex + float2(-1.0f/MAP_SIZE, -3.0f/MAP_SIZE);
    Out.Tex5 = Tex + float2(-1.0f/MAP_SIZE, -1.0f/MAP_SIZE);
    Out.Tex6 = Tex + float2(-1.0f/MAP_SIZE,  1.0f/MAP_SIZE);
    Out.Tex7 = Tex + float2(-1.0f/MAP_SIZE,  3.0f/MAP_SIZE);

    return Out;
}

// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float4 PS_pass2(VS_OUTPUT_QUAD In) : COLOR
{   
    float4 Color;
    float2 dvu = float2( 4.0f/MAP_SIZE, 0 );
	
	Color =   tex2D( SrcSamp, In.Tex0 )
			+ tex2D( SrcSamp, In.Tex1 )
			+ tex2D( SrcSamp, In.Tex2 )
			+ tex2D( SrcSamp, In.Tex3 )
			+ tex2D( SrcSamp, In.Tex4 )
			+ tex2D( SrcSamp, In.Tex5 )
			+ tex2D( SrcSamp, In.Tex6 )
			+ tex2D( SrcSamp, In.Tex7 )
			+ tex2D( SrcSamp, In.Tex0 + dvu)
			+ tex2D( SrcSamp, In.Tex1 + dvu)
			+ tex2D( SrcSamp, In.Tex2 + dvu)
			+ tex2D( SrcSamp, In.Tex3 + dvu)
			+ tex2D( SrcSamp, In.Tex4 + dvu)
			+ tex2D( SrcSamp, In.Tex5 + dvu)
			+ tex2D( SrcSamp, In.Tex6 + dvu)
			+ tex2D( SrcSamp, In.Tex7 + dvu)
			;
	
    return 0.7f * Color;
}
// -------------------------------------------------------------
// 4�p�X�ځF����
// -------------------------------------------------------------

// -------------------------------------------------------------
// ���_�V�F�[�_�v���O����
// -------------------------------------------------------------
VS_OUTPUT VS_pass3(
      float4 Pos    : POSITION,          // ���f���̒��_
      float3 Normal : NORMAL	         // ���f���̖@��
){
    VS_OUTPUT Out = (VS_OUTPUT)0;        // �o�̓f�[�^
	float4	uv;
	
	// ���W�ϊ�
    Out.Pos = mul(Pos, mWVP);
	// �F
	Out.Diffuse = vCol * max( dot(vLightDir, Normal), 0);// �g�U�F
	Out.Ambient = vCol * 0.3f;                     // ���F
	
	// �e�N�X�`�����W
	uv = mul(Pos, mWLPB);
	Out.ShadowMapUV = uv;
	uv = mul(Pos, mWLP);
	Out.Depth       = uv.zzzw;
		
    return Out;
}
// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float4 PS_pass3(VS_OUTPUT In) : COLOR
{   
    float4 Color = In.Ambient;
    
	float  shadow_map = tex2Dproj( ShadowMapSamp, In.ShadowMapUV ).x;
    
    Color += In.Diffuse*((shadow_map < In.Depth.z/In.Depth.w-0.01)
					 ? tex2Dproj( SrcSamp, In.ShadowMapUV) : 1 );

    return Color;
}  
// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique TShader
{
    pass P0
    {
        // �V�F�[�_
        VertexShader = compile vs_1_1 VS_pass0();
        PixelShader  = compile ps_2_0 PS_pass0();
    }
    pass P1
    {
        // �V�F�[�_
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
        
		Sampler[0] = (SrcSamp);
    }
    pass P2
    {
        // �V�F�[�_
        VertexShader = compile vs_1_1 VS_pass2();
        PixelShader  = compile ps_2_0 PS_pass2();
        
		Sampler[0] = (SrcSamp);
    }
    pass P3
    {
        // �V�F�[�_
        VertexShader = compile vs_1_1 VS_pass3();
        PixelShader  = compile ps_2_0 PS_pass3();
        
		Sampler[0] = (ShadowMapSamp);
		Sampler[1] = (SrcSamp);
    }
}
