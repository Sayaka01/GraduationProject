//--------------------------------------------
//	法線を算出
//--------------------------------------------
// N:法線
// T:Tangent
float3 CalcNormalVec(float3 N, float4 T, float4 normalMap)
{
	float sigma = T.w;
	N = normalize(N);
	float3 T2 = normalize(T.xyz);
	T2 = normalize(T2 - dot(N, T2) * sigma);

	float3 B = normalize(cross(N, T2) * sigma);
	normalMap = (normalMap * 2.0) - 1.0;
	N = normalize((normalMap.x * T2) + (normalMap.y * B) + (normalMap.z * N));
	return N;
}

//--------------------------------------------
//	ランバート拡散反射計算関数
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// K:反射率
float3 CalcLambert(float3 N, float3 L, float3 C, float3 K)
{
	float power = saturate(dot(N, -L));
	return C * power * K;
}

//--------------------------------------------
//	フォンの鏡面反射計算関数
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// E:視線ベクトル(正規化済み)(worldPosition - cameraPosition)
// C:入射光(色・強さ)
// K:反射率
float3 CalcPhongSpecular(float3 N, float3 L, float3 E, float3 C, float3 K)
{
	float3 R = reflect(L, N);
	float power = max(dot(-E, R), 0);
	power = pow(power, 32);//鏡面反射の強さを絞る
	return C * power * K;
}

//--------------------------------------------
//	ハーフランバート拡散反射計算関数
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// K:反射率
float3 CalcHalfLambert(float3 N, float3 L, float3 C, float3 K)
{
	float D = saturate(dot(N, -L) * 0.5f + 0.5f);
	return C * D * K;
}

//--------------------------------------------
// リムライト
//--------------------------------------------
// N:法線(正規化済み)
// E:視点方向ベクトル(正規化済み)
// L:入射ベクトル(正規化済み)
// C :ライト色
// RimPower : リムライトの強さ(初期値はテキトーなので自分で設定するが吉)
float3 CalcRimLight(float3 N, float3 E, float3 L, float3 C, float RimPower = 3.0f)
{
	float rim = 1.0f - saturate(dot(N, -E));
	return  C * pow(rim, RimPower) * saturate(dot(L, -E));
}

//--------------------------------------------
// ランプシェーディング
//--------------------------------------------
// tex:ランプシェーディング用テクスチャ
// samp:ランプシェーディング用サンプラステート
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// K:反射率
float3 CalcRampShading(Texture2D tex, SamplerState samp, float3 N, float3 L, float3 C, float3 K)
{
	float D = saturate(dot(N, -L) * 0.5f + 0.5f);
	float ramp = tex.Sample(samp, float2(D, 0.5f)).r;
	return  C * ramp * K.rgb;
}

//--------------------------------------------
// 球体環境マッピング
//--------------------------------------------
// tex:環境マッピング用テクスチャ
// samp:環境マッピング用サンプラステート
// color: 現在のピクセル色
// N:法線(正規化済み)
// C:入射光(色・強さ)
// value:適応率
float3 CalcSphereEnvironment(Texture2D tex, SamplerState samp, in float3 color, float3 N, float3 E, float value)
{
	float3 R = reflect(E, N);
	float2 texcoord = R.xy * 0.5f + 0.5f;
	return	lerp(color.rgb, tex.Sample(samp, texcoord).rgb, value);
}

//--------------------------------------------
// 半球ライティング
//--------------------------------------------
// normal:法線(正規化済み)
// up:上方向（片方）
// sky_color:空(上)色
// ground_color:地面(下)色
// hemisphere_weight:重み
float3 CalcHemiSphereLight(float3 normal, float3 up, float3 skyColor, float3 groundColor, float4 hemisphereWeight)
{
	float factor = dot(normal, up) * 0.5f + 0.5f;
	return lerp(groundColor, skyColor, factor) * hemisphereWeight.x;
}

//--------------------------------------------
//	フォグ
//--------------------------------------------
//color:現在のピクセル色
//fog_color:フォグの色
//fog_range:フォグの範囲情報
//eye_length:視点からの距離
float4 CalcFog(in float4 color, float4 fogColor, float2 fogRange, float eyeLength)
{
	float fogAlpha = saturate((eyeLength - fogRange.x) / (fogRange.y - fogRange.x));
	return lerp(color, fogColor, fogAlpha);
}

// ベックマン分布を計算する
float Beckmann(float m, float t)
{
	float t2 = t * t;
	float t4 = t * t * t * t;
	float m2 = m * m;
	float D = 1.0f / (4.0f * m2 * t4);
	D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
	return D;
}

// フレネルを計算。Schlick近似を使用
float SpcFresnel(float f0, float u)
{
	// from Schlick
	return f0 + (1 - f0) * pow(1 - u, 5);
}

//YIQ変換(グレースケールの値に変換)
float GetBrightness(float3 color)
{
	float3 y = { 0.299,0.587,0.114 };
	float bright = dot(y, color);
	return bright;
}