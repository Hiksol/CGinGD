#include "PostProcess/DitherPostProcessPass.h"
#include "RenderGraphBuilder.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"
#include "ScreenPass.h"
#include "SceneTextures.h"

class FDitherPS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FDitherPS);
    SHADER_USE_PARAMETER_STRUCT(FDitherPS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneTexture)
        SHADER_PARAMETER_SAMPLER(SamplerState, SceneTextureSampler)
        RENDER_TARGET_BINDING_SLOTS()
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};

IMPLEMENT_GLOBAL_SHADER(FDitherPS, "/DitherShader/DitherPostProcess.usf", "MainPS", SF_Pixel);

void AddDitherPostProcessPass(
    FRDGBuilder& GraphBuilder,
    const FSceneView& View,
    FRDGTextureRef SceneColorTexture,
    FRDGTextureRef& OutTexture)
{
    RDG_EVENT_SCOPE(GraphBuilder, "DitherPostProcess");

    const FIntPoint Extent = SceneColorTexture->Desc.Extent;

    FRDGTextureDesc OutputDesc = FRDGTextureDesc::Create2D(
        Extent,
        PF_FloatRGBA,
        FClearValueBinding(FLinearColor::Black),
        TexCreate_ShaderResource | TexCreate_RenderTargetable);

    OutTexture = GraphBuilder.CreateTexture(OutputDesc, TEXT("DitherOutput"));

    FDitherPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FDitherPS::FParameters>();
    PassParameters->SceneTexture = SceneColorTexture;
    PassParameters->SceneTextureSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
    PassParameters->RenderTargets[0] = FRenderTargetBinding(OutTexture, ERenderTargetLoadAction::ENoAction);

    TShaderMapRef<FDitherPS> PixelShader(View.ShaderMap);

    FScreenPassRenderTarget Output(OutTexture, FIntRect(0, 0, Extent.X, Extent.Y), EScreenPassDrawFlags::None);

    AddDrawScreenPass(
        GraphBuilder,
        RDG_EVENT_NAME("DitherApply"),
        View,
        FScreenPassTextureViewport(SceneColorTexture),
        FScreenPassTextureViewport(Output),
        PixelShader,
        PassParameters
    );
}