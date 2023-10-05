#include "3D.hlsli"

VS_OUT main(VS_IN vin)
{

    vin.normal.w = 0;
    float sigma = vin.tangent.w;
    vin.tangent.w = 0;

    //頂点の位置と法線にボーン行列の影響を与える
    float4 blendedPosition = { 0, 0, 0, 1 };
    float4 blendedNormal = { 0, 0, 0, 0 };
    float4 blendedTangent = { 0, 0, 0, 0 };
    for (int boneIndex = 0; boneIndex < 4; ++boneIndex)
    {
        blendedPosition += vin.boneWeights[boneIndex]
            * mul(vin.position, boneTransforms[vin.boneIndices[boneIndex]]);
        blendedNormal += vin.boneWeights[boneIndex]
            * mul(vin.normal, boneTransforms[vin.boneIndices[boneIndex]]);
        blendedTangent += vin.boneWeights[boneIndex]
            * mul(vin.tangent, boneTransforms[vin.boneIndices[boneIndex]]);
    }
     vin.position = float4(blendedPosition.xyz, 1.0f);
     vin.normal = float4(blendedNormal.xyz, 0.0f);
     vin.tangent = float4(blendedNormal.xyz, 0.0f);


    VS_OUT vout;
    vout.position = mul(vin.position, mul(world, viewProjection));

	vout.worldPosition = mul(vin.position, world);
    //vin.normal.w = 0;
    vout.worldNormal = normalize(mul(vin.normal, world));
    vout.worldTangent = normalize(mul(vin.tangent, world));
    vout.worldTangent.w = sigma;

    vout.texcoord = vin.texcoord;

#if 1
    vout.color = materialColor;
#else
    vout.color = 0;
    const float4 boneColors[4] = {
        {1,0,0,1},
        {0,1,0,1},
        {0,0,1,1},
        {1,1,1,1},
    };
    for (int boneIndex = 0; boneIndex < 4; ++boneIndex)
    {
        vout.color += boneColors[vin.boneIndices[boneIndex] % 4] * vin.boneWeights[boneIndex];
    }

#endif

    //シャドウマップ用のパラメーター計算
    {
        //ライトから見たNDC座標を算出
        float4 wvpPos = mul(vin.position, mul(world, lightViewProjection));
        // NDC座標からUV座標を算出する
        wvpPos /= wvpPos.w; //プロジェクション行列を使って座標変換したなら必ずWで割る（同時座標）
        wvpPos.y = -wvpPos.y;
        wvpPos.xy = 0.5f * wvpPos.xy + 0.5f; //texture spaceに変換
        vout.shadowTexcoord = wvpPos.xyz;
    }

    return vout;
}
