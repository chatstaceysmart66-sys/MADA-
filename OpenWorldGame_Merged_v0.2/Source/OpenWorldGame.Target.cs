using UnrealBuildTool;
using System.Collections.Generic;

public class OpenWorldGameTarget : TargetRules
{
    public OpenWorldGameTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.Add("OpenWorldGame");
    }
}
