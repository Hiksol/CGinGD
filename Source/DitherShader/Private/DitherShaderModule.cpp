#include "DitherShaderModule.h"
#include "DitherShaderLog.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "SceneViewExtension.h"

#define LOCTEXT_NAMESPACE "FDitherShaderModule"

void FDitherShaderModule::StartupModule()
{
    UE_LOG(DitherShader, Log, TEXT("DitherShaderModule startup"));

    FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("DitherShader"))->GetBaseDir(), TEXT("Shaders"));
    AddShaderSourceDirectoryMapping(TEXT("/DitherShader"), PluginShaderDir);

    FCoreDelegates::OnPostEngineInit.AddLambda([this]() {
        ViewExtension = FSceneViewExtensions::NewExtension<FDitherSceneViewExtension>();
    });
}

void FDitherShaderModule::ShutdownModule()
{
    UE_LOG(DitherShader, Log, TEXT("DitherShaderModule shutdown"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDitherShaderModule, DitherShader)