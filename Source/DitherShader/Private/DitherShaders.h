#pragma once
#include "CoreMinimal.h"
#include "ScreenPass.h"
#include "SceneTexturesConfig.h"

class FDitherShaderPS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FDitherShaderPS);
    SHADER_USE_PARAMETER_STRUCT(FDitherShaderPS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        RENDER_TARGET_BINDING_SLOTS()
        SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
        SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSceneTextureUniformParameters, SceneTexturesStruct)
        SHADER_PARAMETER(float, DitherAmount)
        SHADER_PARAMETER(float, Quantization)
        SHADER_PARAMETER(int, DitherMode)
    END_SHADER_PARAMETER_STRUCT()
};