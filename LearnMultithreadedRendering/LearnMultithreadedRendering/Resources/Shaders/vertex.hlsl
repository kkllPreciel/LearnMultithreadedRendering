/**
 *  @file     vertex.hlsl
 *  @brief    �����x�[�X�����_�����O�p���_�V�F�[�_�[
 *  @author   kkllPreciel
 *  @date     2018/03/20
 *  @version  1.0
 */

#include "common.hlsli"

/**
 *  @brief  ���_�V�F�[�_�[�̓��͌`��
 */
struct VSInput
{
    float3 position : POSITION; ///< ���W
    float3 normal : NORMAL;     ///< �@���x�N�g��
    float2 uv : TEXCOORD;       ///< UV���W
    uint id : SV_InstanceID;    ///< �C���X�^���XID
};

/**
 *  @brief  �C���X�^���X���̃f�[�^
 */
struct InstanceBuffer
{
    float4x4 world; ///< ���[���h�s��
};

StructuredBuffer<InstanceBuffer> cbv : register(t0); ///< �C���X�^���X���̃f�[�^���X�g

/**
 *  @brief  �R���X�^���g�o�b�t�@
 */
cbuffer ConstantBuffer : register(b0)
{
    float4x4 view;          ///< �r���[�s��(��D��s��)
    float4x4 projection;    ///< �v���W�F�N�V�����s��(��D��s��)
};

/**
 *  @brief  �G���g���[�|�C���g
 */
VSOutput main(VSInput input)
{
    VSOutput result;

    float4 pos = float4(input.position, 1.0);

    pos = mul(cbv[input.id].world, pos);
    pos = mul(view, pos);
    pos = mul(projection, pos);

    result.position = pos;
    result.color = float4(1.0, 1.0, 1.0, 1.0);
    result.uv = input.uv;
    
    // ���[���h��Ԃł̖@���x�N�g��(�ߎ��l)
    result.normal = mul(cbv[input.id].world, float4(input.normal, 1.0));

    // ���[���h��Ԃł̍��W
    result.wPos = mul(cbv[input.id].world, float4(input.position, 1.0));

    return result;
}