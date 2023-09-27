#include "ShadingFunctions.hlsli"
#include "SkinnedMesh.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[3] : register(s0);
SamplerState shadowSamplerState : register(s4);

Texture2D textureMaps[2] : register(t0);
Texture2D shadowMap : register(t6);

float4 main(VS_OUT pin) : SV_TARGET
{
   float4 diffuseColor = textureMaps[0].Sample(samplerStates[ANISOTROPIC], pin.texcoord);
	#if 1
   //ガンマ補正(gamma process)
   const float GAMMA = 2.2;
   diffuseColor.rgb = pow(diffuseColor.rgb, GAMMA);
#endif
   
   float3 N = normalize(pin.worldNormal.xyz);

   //float3 T = normalize(pin.worldTangent.xyz);
   //float sigma = pin.worldTangent.w;
   //T = normalize(T - dot(N, T));
   //float3 B = normalize(cross(N, T) * sigma);

   //float4 normal = textureMaps[1].Sample(samplerStates[LINEAR], pin.texcoord);
   //normal = (normal * 2.0) - 1.0;
   //N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));

   float3 E = normalize(pin.worldPosition.xyz - cameraPosition.xyz);
   float3 L = normalize(lightDirection.xyz);
   float3 c= max(0, dot(L, N));
   //return float4(c, 1.0f);
#if 0
   //return float4(N, diffuseColor.a);
   float t = saturate(dot(N, -L));
   float3 diffuseLight = lightColor * t;
   diffuseColor.rgb *= diffuseLight;
   diffuseColor.rgb = pow(diffuseColor.rgb, 1.0f/GAMMA);
   return diffuseColor;
#endif

	float3 ambient = ambientColor.rgb * materialColor.rgb;
   //ambient += CalcHemiSphereLight(N, float3(0, 1, 0), skyColor.rgb, groundColor.rgb, hemisphereWeight);

   float3 directionalDiffuse = CalcHalfLambert(N, L, lightColor.rgb, materialColor.rgb);

   float3 directionalSpecular = CalcPhongSpecular(N, L, E, lightColor.rgb, materialColor.rgb);

   //点光源の処理
   float3 pointDiffuse = 0;
   float3 pointSpecular = 0;
   for (int i = 0; i < 8; ++i)
   {
	   float3 LP = pin.worldPosition.xyz - pointLights[i].pos.xyz;
	   float len = length(LP);
	   if (len >= pointLights[i].range)continue;

	   //距離による影響率を計算する
	   float attenuateLength = saturate(1.0f - len / pointLights[i].range);
	   float attenuation = pow(attenuateLength, 3.0f);
	   LP /= len;

	   pointDiffuse += CalcHalfLambert(N, LP, pointLights[i].color.rgb, materialColor.rgb) * attenuation;
	   pointSpecular += CalcPhongSpecular(N, LP, E, pointLights[i].color.rgb, materialColor.rgb) * attenuation;
   }

   // スポットライトの処理
   float3 spotDiffuse = 0;
   float3 spotSpecular = 0;
   for (int j = 0; j < 8; ++j)
   {
	   float3 LP = pin.worldPosition.xyz - spotLights[j].pos.xyz;
	   float len = length(LP);
	   if (len >= spotLights[j].range)continue;

	   float attenuateLength = saturate(1.0f / (1.0f + len * len) * spotLights[j].range);
	   float attenuation = attenuateLength * attenuateLength;
	   LP /= len;
	   float3 spotDirection = normalize(spotLights[j].direction.xyz);
	   float angle = dot(spotDirection, LP);
	   float area = spotLights[j].innerCorn - spotLights[j].outerCorn;
	   attenuation *= saturate(1.0f - (spotLights[j].innerCorn - angle) / area);

   	   spotDiffuse += CalcHalfLambert(N, LP, spotLights[j].color.rgb, materialColor.rgb) * attenuation;
	   spotSpecular += CalcPhongSpecular(N, LP, E, spotLights[j].color.rgb, materialColor.rgb) * attenuation;
   }

   float4 color = float4(ambient, 1);
   color.a = min(1.0, diffuseColor.a);
   color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse + spotDiffuse);
   color.rgb += (directionalSpecular + pointSpecular + spotSpecular);
   //color = CalcFog(color, fogColor, fogRange.xy, length(pin.worldPosition.xyz - cameraPosition.xyz));

   //シャドウマップから深度値取得
   //float depth = shadowMap.Sample(shadowSamplerState, pin.shadowTexcoord.xy).r;

   // 深度値を比較して影かどうかを判定する
   //if (pin.shadowTexcoord.z - depth > shadowBias)
   //{
	  // color.rgb *= shadowColor.rgb;
   //}

#if 1
//逆ガンマ補正(Inverse gamma process)
   color.rgb = pow(color.rgb, 1.0f/GAMMA);
#endif



   return color;

}