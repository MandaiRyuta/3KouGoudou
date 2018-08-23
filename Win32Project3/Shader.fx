//ワールド*ビュー*投影変換の行列
float4x4 mWVP;

//アンビエント値
float4 gAmbient = {0.2f,0.2f,0.2f,1.0f};

//基本色
float4 gBaseColor = {1.0f, 0.1f, 0.2f, 1.0f};

//----------------------//
//  頂点の入力情報
//----------------------//
struct VS_INPUT
{
  float4 vPosition    : POSITION;    //頂点座標
  float4 vNormal      : NORMAL;      //法線ベクトル
  float2 vTexCoords   : TEXCOORD0;   //テクスチャUV
  float4 vDiffuse     : COLOR0;      //デフューズ色
  float4 vSpecular    : COLOR1;      //スペキュラ色
};

//----------------------//
//  頂点の出力情報
//----------------------//
struct VS_OUTPUT
{
  float4 vPosition    : POSITION;    //頂点座標
  float4 vDiffuse     : COLOR0;      //デフューズ色
  float4 vSpecular    : COLOR1;      //スペキュラ色
  float2 vTexCoords   : TEXCOORD0;   //テクスチャUV
};

//----------------------//
//  ピクセルの入力情報
//----------------------//
struct PS_INPUT
{
  float4 vDiffuse     : COLOR0;     //デフューズ色
  float4 vSpecular    : COLOR1;     //スペキュラ色
  float2 vTexCoords   : TEXCOORD0;  //テクスチャUV
};

//----------------------//
//  ピクセルの出力情報
//----------------------//
struct PS_OUTPUT
{
  float4 vColor       : COLOR0;     //最終的な出力色
};

//------------------------//
// 頂点シェーダー処理
//------------------------//
VS_OUTPUT VSBaseColor(VS_INPUT v)
{
  VS_OUTPUT o = (VS_OUTPUT)0;

  //座標変換
  o.vPosition = mul(v.vPosition , mWVP);

  //デフューズ色をそのまま格納
  o.vDiffuse = v.vDiffuse;

  //スペキュラ色をそのまま格納
  o.vSpecular = v.vSpecular;

  return o;
}

//------------------------//
// ピクセルシェーダー処理
//------------------------//
PS_OUTPUT PSBaseColor(PS_INPUT p)
{
  PS_OUTPUT o = (PS_OUTPUT)0;

  o.vColor = (p.vDiffuse + gAmbient) * gBaseColor + p.vSpecular;

  return o;
}

//------------------------//
// テクニックの記述
//------------------------//
technique BaseColor
{
  pass P0
  {
    VertexShader = compile vs_1_1 VSBaseColor();
    PixelShader  = compile ps_1_1 PSBaseColor();
  }
}