#include "2D.hlsli"

PS_IN main( VS_IN input)
{
	PS_IN output;
	output.pos = input.pos;
	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}