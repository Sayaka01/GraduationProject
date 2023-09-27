
#include "DebugPrimitive.hlsli"

VS_OUT main(float4 position : POSITION)
{
	VS_OUT vout;
	vout.position = mul(position, mul(world, viewProjection));
	vout.color = materialColor;

	return vout;
}
