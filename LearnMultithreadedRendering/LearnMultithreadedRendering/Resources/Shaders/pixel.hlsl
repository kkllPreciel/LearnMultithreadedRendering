/**
 *  @file     pixel.hlsl
 *  @brief    物理ベースレンダリング用ピクセルシェーダー
 *  @author   kkllPreciel
 *  @date     2018/03/20
 *  @version  1.0
 */

#include "common.hlsli"

/**
 *  @brief  ピクセルシェーダーの出力形式
 */
struct PSOutput
{
    float4 color : SV_TARGET0; ///< 色
};

/**
 *  @brief  アルベドマップ
 */
Texture2D g_albedo_map : register(t0);

/**
 *  @brief  メタリックマップ
 */
Texture2D g_metallic_map : register(t1);

/**
 *  @brief  ラフネスマップ
 */
Texture2D g_roughness_map : register(t2);

/**
 *  @brief  サンプラー
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

// 並行光源(太陽)
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

// ポイントライト
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

// スポットライト
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
// 双方向反射率分布関数

// Normalized Lambert
// 正規化ランバートを使用したのDiffuse(拡散反射)のBRDF(双方向反射率分布関数)
// BRDFはある方向からの入射光が各方向へどれだけ反射されるか。
float3 DiffuseBRDF(float3 diffuseColor)
{
    return diffuseColor / PI;
}

// フレネル反射
// Schlickの近似式を使用数
float3 F_Schlick(float3 specularColor, float3 H, float3 V)
{
    return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

// GGXを使用したマイクロファセット(微細表面)の分布関数
float D_GGX(float a, float dotNH)
{
    float a2 = a * a;
    float dotNH2 = dotNH * dotNH;
    float d = dotNH2 * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d);
}

// 幾何減衰率(マイクロファセットによる自己遮蔽)
// Smith モデルの Schlick 近似
float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL)
{
    float k = a * a * 0.5 + EPSILON;
    float gl = dotNL / (dotNL * (1.0 - k) + k);
    float gv = dotNV / (dotNV * (1.0 - k) + k);
    return gl * gv;
}

// Cook-Torrance
// クックトランスを使用したSpecular(鏡面反射)のBRDF(双方向反射率分布関数)
// BRDFはある方向からの入射光が各方向へどれだけ反射されるか。
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
 *  @brief  エントリーポイント
 */
PSOutput main(VSOutput input)
{
    PSOutput output = (PSOutput) 0;

    // 座標・向き・法線
    GeometricContext geometry;
    geometry.position = (float3) input.wPos; // ワールド空間での座標
    geometry.normal = (float3) normalize(input.normal); // ワールド空間での法線ベクトル
    geometry.viewDir = (float3) normalize(float4(0, 10, -30.5, 1) - input.wPos); // 視点への向きベクトル
    
    // 物体の表面情報(金属度・粗さ・アルベド)
    float metallic = g_metallic_map.Sample(g_sampler, input.uv).r;
    float roughness = g_roughness_map.Sample(g_sampler, input.uv).r;
    float3 albedo = (float3) g_albedo_map.Sample(g_sampler, input.uv); // TODO:ガンマ補正を行う

    // 物体の材質
    Material material;
    material.diffuseColor = lerp(albedo, (float3) 0, metallic); // 拡散反射光の割合
    material.specularColor = lerp((float3) 0.04, albedo, metallic); // 鏡面反射光の割合
    material.specularRoughness = roughness; // 表面の粗さ

    ReflectedLight reflectedLight = (ReflectedLight) 0;
    float3 emissive = (float3) 0;
    float opacity = 1.0;

    IncidentLight directLight;
    DirectionalLight directionalLight;

    // 並行光源
    directionalLight.direction = float3(0, 1, 0);
    directionalLight.color = float3(1, 1, 1);
    getDirectionalDirectLightIrradiance(directionalLight, geometry, directLight);
    RE_Direct(directLight, geometry, material, reflectedLight);

    float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;

    output.color = float4(outgoingLight, opacity);
    return output;
}