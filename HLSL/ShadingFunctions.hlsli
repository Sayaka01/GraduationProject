//--------------------------------------------
//	�@�����Z�o
//--------------------------------------------
// N:�@��
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
//	�����o�[�g�g�U���ˌv�Z�֐�
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcLambert(float3 N, float3 L, float3 C, float3 K)
{
	float power = saturate(dot(N, -L));
	return C * power * K;
}

//--------------------------------------------
//	�t�H���̋��ʔ��ˌv�Z�֐�
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// E:�����x�N�g��(���K���ς�)(worldPosition - cameraPosition)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcPhongSpecular(float3 N, float3 L, float3 E, float3 C, float3 K)
{
	float3 R = reflect(L, N);
	float power = max(dot(-E, R), 0);
	power = pow(power, 32);//���ʔ��˂̋������i��
	return C * power * K;
}

//--------------------------------------------
//	�n�[�t�����o�[�g�g�U���ˌv�Z�֐�
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcHalfLambert(float3 N, float3 L, float3 C, float3 K)
{
	float D = saturate(dot(N, -L) * 0.5f + 0.5f);
	return C * D * K;
}

//--------------------------------------------
// �������C�g
//--------------------------------------------
// N:�@��(���K���ς�)
// E:���_�����x�N�g��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C :���C�g�F
// RimPower : �������C�g�̋���(�����l�̓e�L�g�[�Ȃ̂Ŏ����Őݒ肷�邪�g)
float3 CalcRimLight(float3 N, float3 E, float3 L, float3 C, float RimPower = 3.0f)
{
	float rim = 1.0f - saturate(dot(N, -E));
	return  C * pow(rim, RimPower) * saturate(dot(L, -E));
}

//--------------------------------------------
// �����v�V�F�[�f�B���O
//--------------------------------------------
// tex:�����v�V�F�[�f�B���O�p�e�N�X�`��
// samp:�����v�V�F�[�f�B���O�p�T���v���X�e�[�g
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcRampShading(Texture2D tex, SamplerState samp, float3 N, float3 L, float3 C, float3 K)
{
	float D = saturate(dot(N, -L) * 0.5f + 0.5f);
	float ramp = tex.Sample(samp, float2(D, 0.5f)).r;
	return  C * ramp * K.rgb;
}

//--------------------------------------------
// ���̊��}�b�s���O
//--------------------------------------------
// tex:���}�b�s���O�p�e�N�X�`��
// samp:���}�b�s���O�p�T���v���X�e�[�g
// color: ���݂̃s�N�Z���F
// N:�@��(���K���ς�)
// C:���ˌ�(�F�E����)
// value:�K����
float3 CalcSphereEnvironment(Texture2D tex, SamplerState samp, in float3 color, float3 N, float3 E, float value)
{
	float3 R = reflect(E, N);
	float2 texcoord = R.xy * 0.5f + 0.5f;
	return	lerp(color.rgb, tex.Sample(samp, texcoord).rgb, value);
}

//--------------------------------------------
// �������C�e�B���O
//--------------------------------------------
// normal:�@��(���K���ς�)
// up:������i�Е��j
// sky_color:��(��)�F
// ground_color:�n��(��)�F
// hemisphere_weight:�d��
float3 CalcHemiSphereLight(float3 normal, float3 up, float3 skyColor, float3 groundColor, float4 hemisphereWeight)
{
	float factor = dot(normal, up) * 0.5f + 0.5f;
	return lerp(groundColor, skyColor, factor) * hemisphereWeight.x;
}

//--------------------------------------------
//	�t�H�O
//--------------------------------------------
//color:���݂̃s�N�Z���F
//fog_color:�t�H�O�̐F
//fog_range:�t�H�O�͈̔͏��
//eye_length:���_����̋���
float4 CalcFog(in float4 color, float4 fogColor, float2 fogRange, float eyeLength)
{
	float fogAlpha = saturate((eyeLength - fogRange.x) / (fogRange.y - fogRange.x));
	return lerp(color, fogColor, fogAlpha);
}

// �x�b�N�}�����z���v�Z����
float Beckmann(float m, float t)
{
	float t2 = t * t;
	float t4 = t * t * t * t;
	float m2 = m * m;
	float D = 1.0f / (4.0f * m2 * t4);
	D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
	return D;
}

// �t���l�����v�Z�BSchlick�ߎ����g�p
float SpcFresnel(float f0, float u)
{
	// from Schlick
	return f0 + (1 - f0) * pow(1 - u, 5);
}

//YIQ�ϊ�(�O���[�X�P�[���̒l�ɕϊ�)
float GetBrightness(float3 color)
{
	float3 y = { 0.299,0.587,0.114 };
	float bright = dot(y, color);
	return bright;
}