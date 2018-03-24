/**
 *  @file     common.hlsli
 *  @brief    �����x�[�X�����_�����O�p���ʃV�F�[�_�[
 *  @author   kkllPreciel
 *  @date     2018/03/20
 *  @version  1.0
 */

/**
 *  @brief  ���_�V�F�[�_�[�̏o�͌`��
 */
struct VSOutput
{
    float4 position : SV_POSITION;  ///< ���W
    float4 wPos : COLOR0;           ///< ���[���h��Ԃł̍��W
    float4 normal : COLOR1;         ///< ���[���h��Ԃł̖@���x�N�g��
    float2 uv : TEXCOORD;           ///< UV���W
    float4 color : COLOR2;          ///< �F
};