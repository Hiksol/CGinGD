using UnrealBuildTool;

public class DitherShader : ModuleRules
{
    public DitherShader(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "RenderCore",
            "RHI"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Renderer",
            "RenderCore",
            "RHI",
            "Projects",
            "Slate",
            "SlateCore"
        });

        string PluginShaderDir = System.IO.Path.Combine(ModuleDirectory, "../../Shaders/Private");
        AddEngineThirdPartyPrivateStaticDependencies(Target, "DX11");
    }
}