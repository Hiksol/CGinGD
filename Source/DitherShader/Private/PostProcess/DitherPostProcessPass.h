#pragma once

#include "CoreMinimal.h"
#include "RenderGraphBuilder.h"
#include "SceneView.h"

void AddDitherPostProcessPass(
    FRDGBuilder& GraphBuilder,
    const FSceneView& View,
    FRDGTextureRef SceneColorTexture,
    FRDGTextureRef& OutTexture
);