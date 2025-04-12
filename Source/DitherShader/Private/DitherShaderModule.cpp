#include "DitherShader.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"
#include "PostProcess/DitherPostProcessPass.h"
#include "RendererInterface.h"
#include "SceneViewExtension.h"

#define LOCTEXT_NAMESPACE "FDitherShaderModule"

void FDitherShaderModule::StartupModule()
{
    FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("DitherShader"))->GetBaseDir(), TEXT("Shaders"));
    AddShaderSourceDirectoryMapping(TEXT("/DitherShader"), PluginShaderDir);

    PostOpaqueRenderDelegateHandle = GetRendererModule().RegisterPostOpaqueRenderDelegate(
        FPostOpaqueRenderDelegate::CreateRaw(this, &FDitherShaderModule::OnPostOpaqueRender));
}

void FDitherShaderModule::ShutdownModule()
{
    if (PostOpaqueRenderDelegateHandle.IsValid())
    {
        GetRendererModule().RemovePostOpaqueRenderDelegate(PostOpaqueRenderDelegateHandle);
        PostOpaqueRenderDelegateHandle.Reset();
    }
}

void FDitherShaderModule::OnPostOpaqueRender(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostOpaqueRenderParameters& Parameters)
{
    if (View.bIsSceneCapture || View.bIsReflectionCapture || View.bIsPlanarReflection)
    {
        return;
    }

    if (Parameters.SceneTextures)
    {
        FRDGTextureRef SceneColorTexture = Parameters.SceneTextures->SceneColorTexture;
        FRDGTextureRef OutputTexture = nullptr;

        AddDitherPostProcessPass(GraphBuilder, View, SceneColorTexture, OutputTexture);

        if (OutputTexture)
        {
            Parameters.SceneTextures->SceneColorTexture = OutputTexture;
        }
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDitherShaderModule, DitherShader)