#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "DitherSceneViewExtension.h"

class FDitherShaderModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
private:
    TSharedPtr< class FDitherSceneViewExtension, ESPMode::ThreadSafe > ViewExtension;
};