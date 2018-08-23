//���[���h*�r���[*���e�ϊ��̍s��
float4x4 mWVP;

//�A���r�G���g�l
float4 gAmbient = {0.2f,0.2f,0.2f,1.0f};

//��{�F
float4 gBaseColor = {1.0f, 0.1f, 0.2f, 1.0f};

//----------------------//
//  ���_�̓��͏��
//----------------------//
struct VS_INPUT
{
  float4 vPosition    : POSITION;    //���_���W
  float4 vNormal      : NORMAL;      //�@���x�N�g��
  float2 vTexCoords   : TEXCOORD0;   //�e�N�X�`��UV
  float4 vDiffuse     : COLOR0;      //�f�t���[�Y�F
  float4 vSpecular    : COLOR1;      //�X�y�L�����F
};

//----------------------//
//  ���_�̏o�͏��
//----------------------//
struct VS_OUTPUT
{
  float4 vPosition    : POSITION;    //���_���W
  float4 vDiffuse     : COLOR0;      //�f�t���[�Y�F
  float4 vSpecular    : COLOR1;      //�X�y�L�����F
  float2 vTexCoords   : TEXCOORD0;   //�e�N�X�`��UV
};

//----------------------//
//  �s�N�Z���̓��͏��
//----------------------//
struct PS_INPUT
{
  float4 vDiffuse     : COLOR0;     //�f�t���[�Y�F
  float4 vSpecular    : COLOR1;     //�X�y�L�����F
  float2 vTexCoords   : TEXCOORD0;  //�e�N�X�`��UV
};

//----------------------//
//  �s�N�Z���̏o�͏��
//----------------------//
struct PS_OUTPUT
{
  float4 vColor       : COLOR0;     //�ŏI�I�ȏo�͐F
};

//------------------------//
// ���_�V�F�[�_�[����
//------------------------//
VS_OUTPUT VSBaseColor(VS_INPUT v)
{
  VS_OUTPUT o = (VS_OUTPUT)0;

  //���W�ϊ�
  o.vPosition = mul(v.vPosition , mWVP);

  //�f�t���[�Y�F�����̂܂܊i�[
  o.vDiffuse = v.vDiffuse;

  //�X�y�L�����F�����̂܂܊i�[
  o.vSpecular = v.vSpecular;

  return o;
}

//------------------------//
// �s�N�Z���V�F�[�_�[����
//------------------------//
PS_OUTPUT PSBaseColor(PS_INPUT p)
{
  PS_OUTPUT o = (PS_OUTPUT)0;

  o.vColor = (p.vDiffuse + gAmbient) * gBaseColor + p.vSpecular;

  return o;
}

//------------------------//
// �e�N�j�b�N�̋L�q
//------------------------//
technique BaseColor
{
  pass P0
  {
    VertexShader = compile vs_1_1 VSBaseColor();
    PixelShader  = compile ps_1_1 PSBaseColor();
  }
}