shader "Lit/Standard" {
    litSurface
    properties {
        _ALBEDO_SOURCE("Albedo") : enum "Color;Texture" = "0" (shaderDefine)
        albedoColor("Albedo Color") : color3 = "1 1 1"
        albedoAlpha("Albedo Alpha") : float range 0 1.0 0.001 = "1"
        albedoMap("Albedo Map") : object TextureAsset = "_whiteTexture"
        _METALLIC_SOURCE("Metallic") : enum "Scale;Texture (R)" = "0" (shaderDefine)
        metallicMap("Metallic Map") : object TextureAsset = "_whiteTexture"
        metallicScale("Metallic Scale") : float range 0 1.0 0.01 = "1.0"
        _ROUGHNESS_SOURCE("Roughness") : enum "Scale;From Metallic Map (G);Texture (R);Inverted Texture (R)" = "0" (shaderDefine)
        roughnessMap("Roughness Map") : object TextureAsset = "_whiteTexture"
        roughnessScale("Roughness Scale") : float range 0 1.0 0.01 = "1.0"
        _NORMAL_SOURCE("Normal") : enum "Vertex;Texture;Texture + Detail Texture" = "0" (shaderDefine)
        normalMap("Normal Map") : object TextureAsset = "_flatNormalTexture"
        detailNormalMap("Detail Normal Map") : object TextureAsset = "_flatNormalTexture"
        detailRepeat("Detail Repeat") : float = "8"
        _PARALLAX_SOURCE("Parallax") : enum "None;Texture (R)" = "0" (shaderDefine)
        heightMap("Height Map") : object TextureAsset = "_whiteTexture"
        heightScale("Height Scale") : float range 0.01 1.0 0.001 = "0.008"
        _OCCLUSION_SOURCE("Occlusion") : enum "None;Texture (R);From Metallic Map (B)" = "0" (shaderDefine)
        occlusionMap("Occlusion Map") : object TextureAsset = "_whiteTexture"
        occlusionStrength("Occlusion Strength") : float range 0 1 0.001 = "1"
        _EMISSION_SOURCE("Emission") : enum "None;Color;Texture" = "0" (shaderDefine)
        emissionColor("Emission Color") : color3 = "1 1 1"
        emissionMap("Emission Map") : object TextureAsset = "_blackTexture"
        emissionScale("Emission Scale") : float range 0 16 0.001 = "1"
    }
    
    generatePerforatedVersion
    generatePremulAlphaVersion
    generateGpuSkinningVersion

    ambientLitVersion "StandardAmbientLit.shader"
    directLitVersion "StandardDirectLit.shader"
    ambientLitDirectLitVersion "StandardAmbientLitDirectLit.shader"
    
    glsl_vp {
        $include "StandardCore.vp"
    }
    glsl_fp {
        $include "StandardCore.fp"
    }
}
