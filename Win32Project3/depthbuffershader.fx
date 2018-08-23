float4x4 m_WVP; // ���[���h���W�n �~ �r���[(���C�g�) �~ ���ˉe���W�n
//float4x4 m_WVPT;             // ���[���h���W�n �~ �r���[(���C�g�) �~ ���ˉe���W�n �~ �e�N�X�`���[���W�n

sampler tex0 : register(s0); // ���b�V���̃e�N�X�`���[
//sampler tex1 : register(s1); // �f�v�X�o�b�t�@�T�[�t�F�C�X�̃e�N�X�`���[

//****************************************************************
// �J���[�L�[���l�����Ȃ��ꍇ�̃��b�V����Z�l���擾����V�F�[�_�[
//****************************************************************
struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float2 Depth : TEXCOORD0;
};
VS_OUTPUT VS(float4 Pos : POSITION,
              float4 Normal : NORMAL,
              float2 Tex : TEXCOORD0)
{
    VS_OUTPUT Out;
   
    Out.Pos = mul(Pos, m_WVP);
    Out.Depth.xy = Out.Pos.zw;
      
    return Out;
}

float4 PS(VS_OUTPUT In) : COLOR0
{
    float4 Out;
   
    Out = In.Depth.x / In.Depth.y;
   
    return Out;
}

//****************************************************************
// �J���[�L�[���l������ꍇ�̃��b�V����Z�l���擾����V�F�[�_�[�i���ӂP�j
//****************************************************************

//�A���t�@�u�����h�ɂ��}�X�N��������悤�ɕύX����B
/*
struct VS_OUTPUT1
{
   float4 Pos     : POSITION;
   float2 Tex     : TEXCOORD0;
   float2 Depth   : TEXCOORD1;
   float4 DepthUV : TEXCOORD2;   //�f�v�X�o�b�t�@�̃e�N�Z�����W
};
VS_OUTPUT1 VS1( float4 Pos     : POSITION,
                float4 Normal  : NORMAL,
                float2 Tex     : TEXCOORD0 )
{
   VS_OUTPUT1 Out;
   
   Out.Pos      = mul( Pos, m_WVP );
   Out.Tex      = Tex;
   Out.Depth.xy = Out.Pos.zw;
   
   // �f�v�X�o�b�t�@�̐[�x�����擾���邽�߂Ƀe�N�Z�����W���Z�b�g
   Out.DepthUV  = mul( Pos, m_WVPT );   
   
   return Out;
}

float4 PS1( VS_OUTPUT1 In ) : COLOR0
{  
   float4 Out;
   
   //�����_�����O����s�N�Z���ʒu�ɑΉ�����f�v�X�o�b�t�@��̃e�N�Z���̈ʒu�̃J���[�����擾����
   float4 ZMapColor = tex2Dproj( tex1, In.DepthUV );
   float z = In.Depth.x / In.Depth.y;
   
   //���b�V���̃e�N�X�`���[�̃��l��0���傫���Ƃ��́A���b�V����Z�l���o�͂���
   if( tex2D( tex0, In.Tex ).a > 0.0f )
   {
      //�f�v�X�o�b�t�@��Z�l > ���b�V����Z�l �̂Ƃ��̓��b�V����Z�l���o�͂���
      if( ZMapColor.r > z )
         Out = z;
      else
         Out = ZMapColor;
   }

   //���b�V���̃e�N�X�`���[�̃��l��0�̂Ƃ��́A�f�v�X�o�b�t�@��Z�l���o�͂���
   else
      Out = ZMapColor;
   
   return Out;
}
*/

struct VS1_OUTPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0; //�e�N�Z��
    float2 Depth : TEXCOORD1;
};
VS1_OUTPUT VS1(float4 Pos : POSITION,
                float4 Normal : NORMAL,
                float2 Tex : TEXCOORD0)
{
    VS1_OUTPUT Out;
   
    Out.Pos = mul(Pos, m_WVP);
    Out.Tex = Tex;
    Out.Depth.xy = Out.Pos.zw;
      
    return Out;
}

float4 PS1(VS1_OUTPUT In) : COLOR0
{
    float4 Out;

   //RGB�����ɂ�Z�l���o�͂���
    Out.rgb = In.Depth.x / In.Depth.y;

   //A�����ɂ̓A���t�@�u�����h�̐��`�����Ŏg�p����A���t�@�l���o�͂���
    Out.a = tex2D(tex0, In.Tex).a;
   
    return Out;
}

technique TShader
{
   //�J���[�L�[���l�����Ȃ�
    pass P0
    {
        VertexShader = compile vs_1_1 VS();
        PixelShader = compile ps_2_0 PS();
    }

   //�J���[�L�[���l������
    pass P1
    {
        VertexShader = compile vs_1_1 VS1();
        PixelShader = compile ps_2_0 PS1();
    }
}
