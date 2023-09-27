
#include "2D.hlsli"

Texture2D colorMap : register(t0);

SamplerState pointSamplerState : register(s0);
SamplerState linearSamplerState : register(s1);
SamplerState anisotropicSamplerState : register(s2);

float4 main(PS_IN input) : SV_TARGET
{
	 float4 color = colorMap.Sample(linearSamplerState, input.texcoord);

#if 1
	 //ãtÉKÉìÉ}ï‚ê≥(Inverse gamma process)
	 const float GAMMA = 2.2;
	 color.rgb = pow(color.rgb, GAMMA);
	 color.rgb = pow(color.rgb, 1.0f/GAMMA);
#endif

	 return color * input.color;
}
