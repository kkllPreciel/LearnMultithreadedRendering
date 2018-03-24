/**
 *  @file     pixel.hlsl
 *  @brief    �����x�[�X�����_�����O�p�s�N�Z���V�F�[�_�[
 *  @author   kkllPreciel
 *  @date     2018/03/20
 *  @version  1.0
 */

#include "common.hlsli"

/**
 *  @brief  �s�N�Z���V�F�[�_�[�̏o�͌`��
 */
struct PSOutput
{
    float4 color : SV_TARGET0; ///< �F
};

/**
 *  @brief  �A���x�h�}�b�v
 */
Texture2D g_albedo_map : register(t0);

/**
 *  @brief  ���^���b�N�}�b�v
 */
Texture2D g_metallic_map : register(t1);

/**
 *  @brief  ���t�l�X�}�b�v
 */
Texture2D g_roughness_map : register(t2);

/**
 *  @brief  �T���v���[
 */
SamplerState g_sampler : register(s0);

// defines
#define PI 3.14159265359
#define PI2 6.28318530718
#define RECIPROCAL_PI 0.31830988618
#define RECIPROCAL_PI2 0.15915494
#define LOG2 1.442695
#define EPSILON 1e-6

struct IncidentLight
{
    float3 color;
    float3 direction;
    bool visible;
};

struct ReflectedLight
{
    float3 directDiffuse;
    float3 directSpecular;
    float3 indirectDiffuse;
    float3 indirectSpecular;
};

struct GeometricContext
{
    float3 position;
    float3 normal;
    float3 viewDir;
};

struct Material
{
    float3 diffuseColor;
    float specularRoughness;
    float3 specularColor;
};

// lights

bool testLightInRange(const in float lightDistance, const in float cutoffDistance)
{
    return any(bool2(cutoffDistance == 0.0, lightDistance < cutoffDistance));
}

float punctualLightIntensityToIrradianceFactor(const in float lightDistance, const in float cutoffDistance, const in float decayExponent)
{
    if (decayExponent > 0.0)
    {
        return pow(saturate(-lightDistance / cutoffDistance + 1.0), decayExponent);
    }

    return 1.0;
}

// ���s����(���z)
struct DirectionalLight
{
    float3 direction;
    float3 color;
};

void getDirectionalDirectLightIrradiance(const in DirectionalLight directionalLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    directLight.color = directionalLight.color;
    directLight.direction = directionalLight.direction;
    directLight.visible = true;
}

// �|�C���g���C�g
struct PointLight
{
    float3 position;
    float3 color;
    float distance;
    float decay;
};

void getPointDirectLightIrradiance(const in PointLight pointLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    float3 L = pointLight.position - geometry.position;
    directLight.direction = normalize(L);

    float lightDistance = length(L);
    if (testLightInRange(lightDistance, pointLight.distance))
    {
        directLight.color = pointLight.color;
        directLight.color *= punctualLightIntensityToIrradianceFactor(lightDistance, pointLight.distance, pointLight.decay);
        directLight.visible = true;
    }
    else
    {
        directLight.color = float3(0, 0, 0);
        directLight.visible = false;
    }
}

// �X�|�b�g���C�g
struct SpotLight
{
    float3 position;
    float3 direction;
    float3 color;
    float distance;
    float decay;
    float coneCos;
    float penumbraCos;
};

void getSpotDirectLightIrradiance(const in SpotLight spotLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    float3 L = spotLight.position - geometry.position;
    directLight.direction = normalize(L);

    float lightDistance = length(L);
    float angleCos = dot(directLight.direction, spotLight.direction);

    if (all(bool2(angleCos > spotLight.coneCos, testLightInRange(lightDistance, spotLight.distance))))
    {
        float spotEffect = smoothstep(spotLight.coneCos, spotLight.penumbraCos, angleCos);
        directLight.color = spotLight.color;
        directLight.color *= spotEffect * punctualLightIntensityToIrradianceFactor(lightDistance, spotLight.distance, spotLight.decay);
        directLight.visible = true;
    }
    else
    {
        directLight.color = float3(0, 0, 0);
        directLight.visible = false;
    }
}

// BRDFs
// �o�������˗����z�֐�

// Normalized Lambert
// ���K�������o�[�g���g�p������Diffuse(�g�U����)��BRDF(�o�������˗����z�֐�)
// BRDF�͂����������̓��ˌ����e�����ւǂꂾ�����˂���邩�B
float3 DiffuseBRDF(float3 diffuseColor)
{
    return diffuseColor / PI;
}

// �t���l������
// Schlick�̋ߎ������g�p��
float3 F_Schlick(float3 specularColor, float3 H, float3 V)
{
    return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

// GGX���g�p�����}�C�N���t�@�Z�b�g(���ו\��)�̕��z�֐�
float D_GGX(float a, float dotNH)
{
    float a2 = a * a;
    float dotNH2 = dotNH * dotNH;
    float d = dotNH2 * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d);
}

// �􉽌�����(�}�C�N���t�@�Z�b�g�ɂ�鎩�ȎՕ�)
// Smith ���f���� Schlick �ߎ�
float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL)
{
    float k = a * a * 0.5 + EPSILON;
    float gl = dotNL / (dotNL * (1.0 - k) + k);
    float gv = dotNV / (dotNV * (1.0 - k) + k);
    return gl * gv;
}

// Cook-Torrance
// �N�b�N�g�����X���g�p����Specular(���ʔ���)��BRDF(�o�������˗����z�֐�)
// BRDF�͂����������̓��ˌ����e�����ւǂꂾ�����˂���邩�B
float3 SpecularBRDF(const in IncidentLight directLight, const in GeometricContext geometry, float3 specularColor, float roughnessFactor)
{

    float3 N = geometry.normal;
    float3 V = geometry.viewDir;
    float3 L = directLight.direction;

    float dotNL = saturate(dot(N, L));
    float dotNV = saturate(dot(N, V));
    float3 H = normalize(L + V);
    float dotNH = saturate(dot(N, H));
    float dotVH = saturate(dot(V, H));
    float dotLV = saturate(dot(L, V));
    float a = roughnessFactor * roughnessFactor;

    float D = D_GGX(a, dotNH);
    float G = G_Smith_Schlick_GGX(a, dotNV, dotNL);
    float3 F = F_Schlick(specularColor, V, H);
    return (F * (G * D)) / (4.0 * dotNL * dotNV + EPSILON);
}

// RenderEquations(RE)
void RE_Direct(const in IncidentLight directLight, const in GeometricContext geometry, const in Material material, inout ReflectedLight reflectedLight)
{
    float dotNL = saturate(dot(geometry.normal, directLight.direction));
    float3 irradiance = dotNL * directLight.color;

    // Punctual Light
    irradiance *= PI;

    reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
    reflectedLight.directSpecular += irradiance * SpecularBRDF(directLight, geometry, material.specularColor, material.specularRoughness);
}

/**
 *  @brief  �G���g���[�|�C���g
 */
PSOutput main(VSOutput input)
{
    PSOutput output = (PSOutput) 0;

    // ���W�E�����E�@��
    GeometricContext geometry;
    geometry.position = (float3) input.wPos; // ���[���h��Ԃł̍��W
    geometry.normal = (float3) normalize(input.normal); // ���[���h��Ԃł̖@���x�N�g��
    geometry.viewDir = (float3) normalize(float4(0, 10, -30.5, 1) - input.wPos); // ���_�ւ̌����x�N�g��
    
    // ���̂̕\�ʏ��(�����x�E�e���E�A���x�h)
    float metallic = g_metallic_map.Sample(g_sampler, input.uv).r;
    float roughness = g_roughness_map.Sample(g_sampler, input.uv).r;
    float3 albedo = (float3) g_albedo_map.Sample(g_sampler, input.uv); // TODO:�K���}�␳���s��

    // ���̂̍ގ�
    Material material;
    material.diffuseColor = lerp(albedo, (float3) 0, metallic); // �g�U���ˌ��̊���
    material.specularColor = lerp((float3) 0.04, albedo, metallic); // ���ʔ��ˌ��̊���
    material.specularRoughness = roughness; // �\�ʂ̑e��

    ReflectedLight reflectedLight = (ReflectedLight) 0;
    float3 emissive = (float3) 0;
    float opacity = 1.0;

    IncidentLight directLight;
    DirectionalLight directionalLight;

    // ���s����
    directionalLight.direction = float3(0, 1, 0);
    directionalLight.color = float3(1, 1, 1);
    getDirectionalDirectLightIrradiance(directionalLight, geometry, directLight);
    RE_Direct(directLight, geometry, material, reflectedLight);

    float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;

    output.color = float4(outgoingLight, opacity);
    return output;
}