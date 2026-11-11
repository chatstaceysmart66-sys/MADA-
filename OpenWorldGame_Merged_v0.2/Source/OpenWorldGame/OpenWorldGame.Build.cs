using UnrealBuildTool;

public class OpenWorldGame : ModuleRules
{
    public OpenWorldGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "RHI",
            "PhysicsCore",
            "InputCore",
            "UMG",
            "Slate",
            "SlateCore"
        });
    }
}
