#include "DitherSceneViewExtension.h"
#include "DitherShaders.h"

#include "FXRenderingUtils.h"
#include "PostProcess/PostProcessInputs.h"
#include "DynamicResolutionState.h"


static TAutoConsoleVariable<int32> CVarEnabled(
	TEXT("r.FSP"),
	1,
	TEXT("Controls DitherShader plugin\n")
	TEXT(" 0: disabled\n")
	TEXT(" 1: enabled (default)"));

// Strength of the ordered-dither effect (0 = off, 1 = full)
static TAutoConsoleVariable<float> CVarDitherAmount(
    TEXT("r.FSP.DitherAmount"),
    0.5f,
    TEXT("Strength of ordered-dither effect [0,1]."));

// Number of quantization levels (e.g. 8 = 8-bit). Set to 0 to disable.
static TAutoConsoleVariable<float> CVarQuantization(
    TEXT("r.FSP.Quantization"),
    16.0f,
    TEXT("Color quantization levels (0 disables quantization)."));

// Dither mode: 0 = Add, 1 = Multiply
static TAutoConsoleVariable<int32> CVarDitherMode(
    TEXT("r.FSP.DitherMode"),
    0,
    TEXT("Ordered-dither mode: 0=Add, 1=Multiply."));


FDitherSceneViewExtension::FDitherSceneViewExtension(const FAutoRegister& AutoRegister) :
	FSceneViewExtensionBase(AutoRegister)
{

}

void FDitherSceneViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	if (CVarEnabled->GetInt() == 0)
	{
		return;
	}

	Inputs.Validate();

	const FIntRect PrimaryViewRect = UE::FXRenderingUtils::GetRawViewRectUnsafe(View);

	FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, PrimaryViewRect);

	if (!SceneColor.IsValid())
	{
		return;
	}

	const FScreenPassTextureViewport Viewport(SceneColor);

	FRDGTextureDesc SceneColorDesc = SceneColor.Texture->Desc;

	SceneColorDesc.Format = PF_FloatRGBA;
	FLinearColor ClearColor(0., 0., 0., 0.);
	SceneColorDesc.ClearValue = FClearValueBinding(ClearColor);

	FRDGTexture* ResultTexture = GraphBuilder.CreateTexture(SceneColorDesc, TEXT("DitherShaderResult"));
	FScreenPassRenderTarget ResultRenderTarget = FScreenPassRenderTarget(ResultTexture, SceneColor.ViewRect, ERenderTargetLoadAction::EClear);

	FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
	TShaderMapRef<FDitherShaderVS> ScreenPassVS(GlobalShaderMap);
	TShaderMapRef<FDitherShaderPS> ScreenPassPS(GlobalShaderMap);

	FDitherShaderPS::FParameters* Parameters = GraphBuilder.AllocParameters<FDitherShaderPS::FParameters>();
	Parameters->View = View.ViewUniformBuffer;
	Parameters->SceneTexturesStruct = Inputs.SceneTextures;
	Parameters->DitherAmount = CVarDitherAmount.GetValueOnRenderThread();
	Parameters->Quantization = CVarQuantization.GetValueOnRenderThread();
	Parameters->DitherMode = CVarDitherMode.GetValueOnRenderThread();

	Parameters->RenderTargets[0] = ResultRenderTarget.GetRenderTargetBinding();

	AddDrawScreenPass(
		GraphBuilder,
		RDG_EVENT_NAME("DitherShader"),
		View,
		Viewport,
		Viewport,
		ScreenPassVS,
		ScreenPassPS,
		Parameters
	);

	AddCopyTexturePass(GraphBuilder, ResultTexture, SceneColor.Texture);
}