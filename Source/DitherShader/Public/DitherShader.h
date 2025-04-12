#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "PostProcess/RenderTargetPool.h"
#include "PostProcess/SceneRenderTargets.h"

class FDitherShaderModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void OnPostOpaqueRender(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostOpaqueRenderParameters& Parameters);

    FDelegateHandle PostOpaqueRenderDelegateHandle;
};